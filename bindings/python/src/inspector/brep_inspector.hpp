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

#include <geode/model/representation/core/brep.hpp>

#include <geode/inspector/brep_inspector.hpp>

namespace geode
{
    void define_brep_inspector( pybind11::module& module )
    {
        pybind11::class_< BRepInspectionResult >(
            module, "BRepInspectionResult" )
            .def( pybind11::init<>() )
            .def_readwrite( "meshes", &BRepInspectionResult::meshes )
            .def_readwrite( "topology", &BRepInspectionResult::topology )
            .def( "string", &BRepInspectionResult::string )
            .def( "inspection_type", &BRepInspectionResult::inspection_type );

        pybind11::class_< BRepInspector, BRepMeshesInspector,
            BRepTopologyInspector >( module, "BRepInspector" )
            .def( pybind11::init< const BRep& >() )
            .def( "inspect_brep", &BRepInspector::inspect_brep );

        module.def( "inspect_brep", []( const BRep& brep ) {
            BRepInspector inspector{ brep };
            return inspector.inspect_brep();
        } );
    }
} // namespace geode
