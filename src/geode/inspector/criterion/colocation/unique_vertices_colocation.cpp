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

#include <geode/inspector/criterion/colocation/unique_vertices_colocation.h>

#include <geode/basic/logger.h>
#include <geode/basic/pimpl_impl.h>

#include <geode/geometry/point.h>

#include <geode/mesh/builder/point_set_builder.h>
#include <geode/mesh/core/edged_curve.h>
#include <geode/mesh/core/point_set.h>
#include <geode/mesh/core/solid_mesh.h>
#include <geode/mesh/core/surface_mesh.h>

#include <geode/model/mixin/core/block.h>
#include <geode/model/mixin/core/corner.h>
#include <geode/model/mixin/core/line.h>
#include <geode/model/mixin/core/surface.h>
#include <geode/model/representation/core/brep.h>
#include <geode/model/representation/core/section.h>

#include <geode/inspector/criterion/colocation/pointset_colocation.h>

namespace
{
    template < geode::index_t dimension, typename Model >
    bool model_cmv_is_colocated_on_point_base( const Model& model,
        const geode::ComponentMeshVertex& cmv,
        const geode::Point< dimension >& point )
    {
        if( cmv.component_id.type()
            == geode::Line< dimension >::component_type_static() )
        {
            return point.inexact_equal( model.line( cmv.component_id.id() )
                                            .mesh()
                                            .point( cmv.vertex ) );
        }
        else if( cmv.component_id.type()
                 == geode::Surface< dimension >::component_type_static() )
        {
            return point.inexact_equal( model.surface( cmv.component_id.id() )
                                            .mesh()
                                            .point( cmv.vertex ) );
        }
        return point.inexact_equal(
            model.corner( cmv.component_id.id() ).mesh().point( cmv.vertex ) );
    }

    bool model_cmv_is_colocated_on_point( const geode::Section& model,
        const geode::ComponentMeshVertex& cmv,
        const geode::Point2D& point )
    {
        return model_cmv_is_colocated_on_point_base( model, cmv, point );
    }

    bool model_cmv_is_colocated_on_point( const geode::BRep& model,
        const geode::ComponentMeshVertex& cmv,
        const geode::Point3D& point )
    {
        if( cmv.component_id.type() == geode::Block3D::component_type_static() )
        {
            return point.inexact_equal( model.block( cmv.component_id.id() )
                                            .mesh()
                                            .point( cmv.vertex ) );
        }
        return model_cmv_is_colocated_on_point_base( model, cmv, point );
    }

    template < geode::index_t dimension, typename Model >
    bool model_cmvs_are_colocated_on_point( const Model& model,
        const std::vector< geode::ComponentMeshVertex >& cmvs,
        const geode::Point< dimension >& point )
    {
        for( const auto& cmv : cmvs )
        {
            if( !model_cmv_is_colocated_on_point( model, cmv, point ) )
            {
                return false;
            }
        }
        return true;
    }

    template < geode::index_t dimension, typename Model >
    geode::Point< dimension > model_unique_vertex_point_base(
        const Model& model, const geode::ComponentMeshVertex& cmv )
    {
        if( cmv.component_id.type()
            == geode::Line< dimension >::component_type_static() )
        {
            const auto& mesh = model.line( cmv.component_id.id() ).mesh();
            return mesh.point( cmv.vertex );
        }
        if( cmv.component_id.type()
            == geode::Surface< dimension >::component_type_static() )
        {
            const auto& mesh = model.surface( cmv.component_id.id() ).mesh();
            return mesh.point( cmv.vertex );
        }
        const auto& mesh = model.corner( cmv.component_id.id() ).mesh();
        return mesh.point( cmv.vertex );
    }

    geode::Point2D model_unique_vertex_point(
        const geode::Section& model, const geode::ComponentMeshVertex& cmv )
    {
        return model_unique_vertex_point_base< 2, geode::Section >(
            model, cmv );
    }

    geode::Point3D model_unique_vertex_point(
        const geode::BRep& model, const geode::ComponentMeshVertex& cmv )
    {
        if( cmv.component_id.type() == geode::Block3D::component_type_static() )
        {
            const auto& mesh = model.block( cmv.component_id.id() ).mesh();
            return mesh.point( cmv.vertex );
        }
        return model_unique_vertex_point_base< 3, geode::BRep >( model, cmv );
    }
} // namespace

namespace geode
{
    std::string UniqueVerticesInspectionResult::string() const
    {
        std::string message;
        absl::StrAppend(
            &message, colocated_unique_vertices_groups.string(), "\n" );
        absl::StrAppend(
            &message, unique_vertices_linked_to_different_points.string() );
        return message;
    }

    template < geode::index_t dimension, typename Model >
    class UniqueVerticesColocation< dimension, Model >::Impl
    {
    public:
        Impl( const Model& model )
            : model_( model ),
              unique_vertices_{ PointSet< dimension >::create() }
        {
            auto builder =
                PointSetBuilder< dimension >::create( *unique_vertices_ );
            builder->create_vertices( model.nb_unique_vertices() );
            for( const auto unique_vertex_id :
                Range{ model.nb_unique_vertices() } )
            {
                const auto& cmvs =
                    model.component_mesh_vertices( unique_vertex_id );
                if( cmvs.empty() )
                {
                    continue;
                }
                builder->set_point( unique_vertex_id,
                    model_unique_vertex_point( model, cmvs.at( 0 ) ) );
            }
        }

        bool model_has_unique_vertices_linked_to_different_points() const
        {
            for( const auto unique_vertex_id :
                Range{ model_.nb_unique_vertices() } )
            {
                if( !model_cmvs_are_colocated_on_point( model_,
                        model_.component_mesh_vertices( unique_vertex_id ),
                        unique_vertices_->point( unique_vertex_id ) ) )
                {
                    return true;
                }
            }
            return false;
        }

        bool model_has_colocated_unique_vertices() const
        {
            const PointSetColocation< dimension > pointset_inspector{
                *unique_vertices_
            };
            const auto has_colocation =
                pointset_inspector.mesh_has_colocated_points();
            return has_colocation;
        }

        InspectionIssues< index_t >
            unique_vertices_linked_to_different_points() const
        {
            InspectionIssues< index_t > vertices{
                "Unique vertices linked to different points."
            };
            for( const auto unique_vertex_id :
                Range{ model_.nb_unique_vertices() } )
            {
                if( !model_cmvs_are_colocated_on_point( model_,
                        model_.component_mesh_vertices( unique_vertex_id ),
                        unique_vertices_->point( unique_vertex_id ) ) )
                {
                    vertices.add_issue( unique_vertex_id,
                        absl::StrCat( "Unique vertex with index ",
                            unique_vertex_id,
                            " has component mesh vertices which are not "
                            "on the same position." ) );
                }
            }
            return vertices;
        }

        InspectionIssues< std::vector< index_t > >
            colocated_unique_vertices_groups() const
        {
            const PointSetColocation< dimension > pointset_inspector{
                *unique_vertices_
            };
            InspectionIssues< std::vector< index_t > >
                colocated_unique_vertices_groups{
                    "Groups of colocated unique vertices."
                };
            const auto colocated_pts_groups =
                pointset_inspector.colocated_points_groups();
            for( const auto& point_group : colocated_pts_groups.issues() )
            {
                std::vector< index_t > fixed_point_group;
                std::string point_group_string;
                for( const auto point_index : Indices{ point_group } )
                {
                    if( model_
                            .component_mesh_vertices( point_group[point_index] )
                            .empty() )
                    {
                        continue;
                    }
                    fixed_point_group.push_back( point_group[point_index] );
                    absl::StrAppend(
                        &point_group_string, " ", point_group[point_index] );
                }
                if( !fixed_point_group.empty() )
                {
                    colocated_unique_vertices_groups.add_issue(
                        fixed_point_group,
                        absl::StrCat( "Unique vertices with indices",
                            point_group_string, " are colocated at position [",
                            unique_vertices_->point( fixed_point_group[0] )
                                .string(),
                            "]." ) );
                }
            }
            return colocated_unique_vertices_groups;
        }

    private:
        const Model& model_;
        std::unique_ptr< PointSet< dimension > > unique_vertices_;
    };

    template < geode::index_t dimension, typename Model >
    UniqueVerticesColocation< dimension, Model >::UniqueVerticesColocation(
        const Model& model )
        : impl_( model )
    {
    }

    template < geode::index_t dimension, typename Model >
    UniqueVerticesColocation< dimension, Model >::~UniqueVerticesColocation()
    {
    }

    template < geode::index_t dimension, typename Model >
    bool UniqueVerticesColocation< dimension,
        Model >::model_has_unique_vertices_linked_to_different_points() const
    {
        return impl_->model_has_unique_vertices_linked_to_different_points();
    }

    template < geode::index_t dimension, typename Model >
    bool UniqueVerticesColocation< dimension,
        Model >::model_has_colocated_unique_vertices() const
    {
        return impl_->model_has_colocated_unique_vertices();
    }

    template < geode::index_t dimension, typename Model >
    UniqueVerticesInspectionResult
        UniqueVerticesColocation< dimension, Model >::inspect_unique_vertices()
            const
    {
        UniqueVerticesInspectionResult result;
        result.colocated_unique_vertices_groups =
            impl_->colocated_unique_vertices_groups();
        result.unique_vertices_linked_to_different_points =
            impl_->unique_vertices_linked_to_different_points();
        return result;
    }

    template class opengeode_inspector_inspector_api
        UniqueVerticesColocation< 2, Section >;
    template class opengeode_inspector_inspector_api
        UniqueVerticesColocation< 3, BRep >;
} // namespace geode
