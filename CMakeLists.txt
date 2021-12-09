cmake_minimum_required (VERSION 3.8)

project ("Raytracer")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(BUILD_TESTING OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/eigen)
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/spdlog)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
