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
#include <string>

#include <geode/mesh/core/edged_curve.hpp>

#include <geode/inspector/edgedcurve_inspector.hpp>

namespace geode
{
    template < index_t dimension >
    void do_define_edged_curve_inspector( pybind11::module& module )
    {
        using EdgedCurve = EdgedCurve< dimension >;
        using EdgedCurveInspector = geode::EdgedCurveInspector< dimension >;

        const auto name = absl::StrCat( "EdgedCurveInspector", dimension, "D" );
        pybind11::class_< EdgedCurveInspector,
            EdgedCurveColocation< dimension >,
            EdgedCurveDegeneration< dimension > >( module, name.c_str() )
            .def( pybind11::init< const EdgedCurve& >() )
            .def( "inspect_edged_curve",
                &EdgedCurveInspector::inspect_edged_curve );

        const auto inspect_function_name =
            absl::StrCat( "inspect_edged_curve", dimension, "D" );
        module.def(
            inspect_function_name.c_str(), []( const EdgedCurve& edged_curve ) {
                EdgedCurveInspector inspector{ edged_curve };
                return inspector.inspect_edged_curve();
            } );
    }

    void define_edged_curve_inspector( pybind11::module& module )
    {
        pybind11::class_< EdgedCurveInspectionResult >(
            module, "EdgedCurveInspectionResult" )
            .def( pybind11::init<>() )
            .def_readwrite( "colocated_points_groups",
                &EdgedCurveInspectionResult::colocated_points_groups )
            .def_readwrite( "degenerated_edges",
                &EdgedCurveInspectionResult::degenerated_edges )
            .def( "string", &EdgedCurveInspectionResult::string )
            .def( "inspection_type",
                &EdgedCurveInspectionResult::inspection_type );

        do_define_edged_curve_inspector< 2 >( module );
        do_define_edged_curve_inspector< 3 >( module );
    }
} // namespace geode
