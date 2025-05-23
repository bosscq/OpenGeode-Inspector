# Copyright (c) 2019 - 2025 Geode-solutions
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

add_geode_binary(
    SOURCE "geode-inspector-brep.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-IO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-cross-section.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-Geosciences::explicit
        OpenGeode-GeosciencesIO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-edgedcurve.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::mesh
        OpenGeode-IO::mesh
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-implicit-cross-section.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-Geosciences::implicit
        OpenGeode-GeosciencesIO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-implicit-structural-model.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-Geosciences::implicit
        OpenGeode-GeosciencesIO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-pointset.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::mesh
        OpenGeode-IO::mesh
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-section.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-IO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-solid.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::mesh
        OpenGeode-IO::mesh
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-structural-model.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::geometry
        OpenGeode::mesh
        OpenGeode::model
        OpenGeode-Geosciences::explicit
        OpenGeode-GeosciencesIO::model
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-surface.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::mesh
        OpenGeode-IO::mesh
        ${PROJECT_NAME}::inspector
)

add_geode_binary(
    SOURCE "geode-inspector-surface-curve-intersections.cpp"
    DEPENDENCIES
        Async++
        absl::flags
        absl::flags_parse
        absl::flags_usage
        OpenGeode::basic
        OpenGeode::mesh
        OpenGeode-IO::mesh
        ${PROJECT_NAME}::inspector
)