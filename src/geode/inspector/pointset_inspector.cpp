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

#include <geode/inspector/pointset_inspector.hpp>

#include <geode/mesh/core/point_set.hpp>

namespace geode
{
    index_t PointSetInspectionResult::nb_issues() const
    {
        return colocated_points_groups.nb_issues();
    }

    std::string PointSetInspectionResult::string() const
    {
        return colocated_points_groups.string();
    }

    std::string PointSetInspectionResult::inspection_type() const
    {
        return "PointSet Inspection";
    }

    template < index_t dimension >
    PointSetInspector< dimension >::PointSetInspector(
        const PointSet< dimension >& mesh )
        : AddInspectors< PointSet< dimension >,
              PointSetColocation< dimension > >{ mesh }
    {
    }

    template < index_t dimension >
    PointSetInspectionResult
        PointSetInspector< dimension >::inspect_point_set() const
    {
        PointSetInspectionResult result;
        result.colocated_points_groups = this->colocated_points_groups();
        return result;
    }

    template class opengeode_inspector_inspector_api PointSetInspector< 2 >;
    template class opengeode_inspector_inspector_api PointSetInspector< 3 >;
} // namespace geode
