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

#pragma once

#include <absl/container/flat_hash_map.h>

#include <geode/inspector/common.hpp>
#include <geode/inspector/information.hpp>

namespace geode
{
    struct uuid;
    struct PolygonEdge;
} // namespace geode

namespace geode
{
    namespace internal
    {
        /*!
         * Class for inspecting the adjacency of edges in the Component Meshes
         * of a Model (BRep or Section).
         */
        template < typename Model >
        class ComponentMeshesAdjacency
        {
            OPENGEODE_DISABLE_COPY( ComponentMeshesAdjacency );

        public:
            void add_surfaces_edges_with_wrong_adjacencies(
                InspectionIssuesMap< PolygonEdge >& issues_map ) const;

        protected:
            explicit ComponentMeshesAdjacency( const Model& model );

            [[nodiscard]] const Model& model() const;

        private:
            const Model& model_;
        };
    } // namespace internal
} // namespace geode