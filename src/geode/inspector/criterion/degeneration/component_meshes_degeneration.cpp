/*
 * Copyright (c) 2019 - 2024 Geode-solutions
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <geode/inspector/criterion/degeneration/component_meshes_degeneration.h>

#include <geode/basic/pimpl_impl.h>

#include <geode/mesh/core/edged_curve.h>
#include <geode/mesh/core/solid_mesh.h>
#include <geode/mesh/core/surface_mesh.h>

#include <geode/model/mixin/core/block.h>
#include <geode/model/mixin/core/line.h>
#include <geode/model/mixin/core/surface.h>
#include <geode/model/representation/core/brep.h>
#include <geode/model/representation/core/section.h>

#include <geode/inspector/criterion/degeneration/edgedcurve_degeneration.h>
#include <geode/inspector/criterion/degeneration/solid_degeneration.h>
#include <geode/inspector/criterion/degeneration/surface_degeneration.h>

namespace
{
    template < geode::index_t dimension, typename Model >
    absl::flat_hash_map< geode::uuid, geode::DegeneratedElements >
        model_components_degenerated_elements_base( const Model& model )
    {
        absl::flat_hash_map< geode::uuid, geode::DegeneratedElements >
            components_degenerated_elements;
        for( const auto& line : model.lines() )
        {
            const geode::EdgedCurveDegeneration< dimension > inspector{
                line.mesh()
            };
            geode::DegeneratedElements elements;
            elements.degenerated_edges = inspector.degenerated_edges();
            if( elements.degenerated_edges.nb_issues() != 0 )
            {
                components_degenerated_elements.emplace(
                    line.id(), std::move( elements ) );
            }
        }
        for( const auto& surface : model.surfaces() )
        {
            const geode::SurfaceMeshDegeneration< dimension > inspector{
                surface.mesh()
            };
            geode::DegeneratedElements elements;
            elements.degenerated_edges = inspector.degenerated_edges();
            elements.degenerated_polygons = inspector.degenerated_polygons();
            if( elements.degenerated_edges.nb_issues() != 0
                || elements.degenerated_polygons.nb_issues() != 0 )
            {
                components_degenerated_elements.emplace(
                    surface.id(), std::move( elements ) );
            }
        }
        return components_degenerated_elements;
    }

    absl::flat_hash_map< geode::uuid, geode::DegeneratedElements >
        model_components_degenerated_elements( const geode::Section& model )
    {
        return model_components_degenerated_elements_base< 2, geode::Section >(
            model );
    }

    absl::flat_hash_map< geode::uuid, geode::DegeneratedElements >
        model_components_degenerated_elements( const geode::BRep& model )
    {
        auto components_degenerated_elements =
            model_components_degenerated_elements_base< 3, geode::BRep >(
                model );
        for( const auto& block : model.blocks() )
        {
            const geode::SolidMeshDegeneration3D inspector{ block.mesh() };
            geode::DegeneratedElements elements;
            elements.degenerated_edges = inspector.degenerated_edges();
            elements.degenerated_polyhedra = inspector.degenerated_polyhedra();
            if( elements.degenerated_edges.nb_issues() != 0
                || elements.degenerated_polyhedra.nb_issues() != 0 )
            {
                components_degenerated_elements.emplace(
                    block.id(), std::move( elements ) );
            }
        }
        return components_degenerated_elements;
    }
} // namespace

namespace geode
{
    std::string DegeneratedElementsInspectionResult::string() const
    {
        std::string message;
        for( const auto& issue : elements )
        {
            const auto& degenerated_elements = issue.second;
            if( degenerated_elements.degenerated_edges.nb_issues() != 0 )
            {
                absl::StrAppend(
                    &message, issue.second.degenerated_edges.string(), "\n" );
            }
            if( degenerated_elements.degenerated_polygons.nb_issues() != 0 )
            {
                absl::StrAppend( &message,
                    issue.second.degenerated_polygons.string(), "\n" );
            }
            if( degenerated_elements.degenerated_polyhedra.nb_issues() != 0 )
            {
                absl::StrAppend( &message,
                    issue.second.degenerated_polyhedra.string(), "\n" );
            }
        }
        if( elements.empty() )
        {
            absl::StrAppend(
                &message, "No degenerated elements in model component meshes" );
        }
        return message;
    }

    template < geode::index_t dimension, typename Model >
    class ComponentMeshesDegeneration< dimension, Model >::Impl
    {
    public:
        Impl( const Model& model ) : model_( model ) {}

        absl::flat_hash_map< uuid, DegeneratedElements >
            components_degenerated_elements() const
        {
            return model_components_degenerated_elements( model_ );
        }

    private:
        const Model& model_;
    };

    template < geode::index_t dimension, typename Model >
    ComponentMeshesDegeneration< dimension,
        Model >::ComponentMeshesDegeneration( const Model& model )
        : impl_( model )
    {
    }

    template < geode::index_t dimension, typename Model >
    ComponentMeshesDegeneration< dimension,
        Model >::~ComponentMeshesDegeneration()
    {
    }

    template < geode::index_t dimension, typename Model >
    DegeneratedElementsInspectionResult
        ComponentMeshesDegeneration< dimension, Model >::inspect_elements()
            const
    {
        DegeneratedElementsInspectionResult result;
        result.elements = impl_->components_degenerated_elements();
        return result;
    }

    template class opengeode_inspector_inspector_api
        ComponentMeshesDegeneration< 2, Section >;
    template class opengeode_inspector_inspector_api
        ComponentMeshesDegeneration< 3, BRep >;
} // namespace geode
