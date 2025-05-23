/*
 * Copyright (c) 2019 - 2025 Geode-solutions
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

#include <geode/inspector/criterion/adjacency/surface_adjacency.hpp>

#include <geode/basic/pimpl_impl.hpp>
#include <geode/basic/uuid.hpp>

#include <geode/mesh/core/surface_mesh.hpp>

namespace geode
{
    template < index_t dimension >
    class SurfaceMeshAdjacency< dimension >::Impl
    {
    public:
        Impl( const SurfaceMesh< dimension >& mesh ) : mesh_( mesh ) {}

        bool mesh_has_wrong_adjacencies() const
        {
            for( const auto polygon_id : Range{ mesh_.nb_polygons() } )
            {
                for( const auto edge_id :
                    LRange{ mesh_.nb_polygon_edges( polygon_id ) } )
                {
                    const PolygonEdge polygon_edge{ polygon_id, edge_id };
                    if( !mesh_.is_edge_on_border( polygon_edge )
                        && !mesh_polygon_edge_has_right_adjacency(
                            polygon_edge ) )
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        InspectionIssues< PolygonEdge >
            polygon_edges_with_wrong_adjacency() const
        {
            InspectionIssues< PolygonEdge > wrong_adjacency_edges{ absl::StrCat(
                "Polygon edges with wrong adjacencies." ) };
            for( const auto polygon_id : Range{ mesh_.nb_polygons() } )
            {
                for( const auto edge_id :
                    LRange{ mesh_.nb_polygon_edges( polygon_id ) } )
                {
                    const PolygonEdge polygon_edge{ polygon_id, edge_id };
                    if( !mesh_.is_edge_on_border( polygon_edge )
                        && !mesh_polygon_edge_has_right_adjacency(
                            polygon_edge ) )
                    {
                        wrong_adjacency_edges.add_issue(
                            polygon_edge, absl::StrCat( "Local edge ", edge_id,
                                              " of polygon ", polygon_id,
                                              " has wrong adjacencies." ) );
                    }
                }
            }
            return wrong_adjacency_edges;
        }

    private:
        bool mesh_polygon_edge_has_right_adjacency(
            const PolygonEdge& polygon_edge ) const
        {
            const auto adjacent_polygon =
                mesh_.polygon_adjacent( polygon_edge );
            const auto polygon_adj_id = adjacent_polygon.value();
            const auto v0 = mesh_.polygon_edge_vertex( polygon_edge, 0 );
            const auto v1 = mesh_.polygon_edge_vertex( polygon_edge, 1 );
            for( const auto edge_id :
                LRange{ mesh_.nb_polygon_edges( polygon_adj_id ) } )
            {
                const PolygonEdge adj_edge{ polygon_adj_id, edge_id };
                const auto adj_v0 =
                    mesh_.polygon_vertex( PolygonVertex{ adj_edge } );
                if( adj_v0 == v1 )
                {
                    const auto adj_v1 =
                        mesh_.polygon_edge_vertex( adj_edge, 1 );
                    if( adj_v1 == v0
                        && mesh_.polygon_adjacent( adj_edge )
                               == polygon_edge.polygon_id )
                    {
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        const SurfaceMesh< dimension >& mesh_;
    };

    template < index_t dimension >
    SurfaceMeshAdjacency< dimension >::SurfaceMeshAdjacency(
        const SurfaceMesh< dimension >& mesh )
        : impl_( mesh )
    {
    }

    template < index_t dimension >
    SurfaceMeshAdjacency< dimension >::~SurfaceMeshAdjacency() = default;

    template < index_t dimension >
    bool SurfaceMeshAdjacency< dimension >::mesh_has_wrong_adjacencies() const
    {
        return impl_->mesh_has_wrong_adjacencies();
    }

    template < index_t dimension >
    InspectionIssues< PolygonEdge >
        SurfaceMeshAdjacency< dimension >::polygon_edges_with_wrong_adjacency()
            const
    {
        return impl_->polygon_edges_with_wrong_adjacency();
    }

    template class opengeode_inspector_inspector_api SurfaceMeshAdjacency< 2 >;
    template class opengeode_inspector_inspector_api SurfaceMeshAdjacency< 3 >;
} // namespace geode
