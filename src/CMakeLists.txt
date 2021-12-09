set(
	SRC
	Hittable.cpp
	Log.cpp
	Material.cpp
	Raytracer.cpp
	Renderer.cpp
)

add_executable(Raytracer ${SRC})

set_target_properties(Raytracer PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED true)
target_include_directories(
	Raytracer
	PRIVATE
	${IMGUI_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/ThirdParty/eigen
	${CMAKE_SOURCE_DIR}/ThirdParty/spdlog/include
)
target_link_libraries(Raytracer imgui D3D11 DXGI spdlog::spdlog Eigen3::Eigen)
