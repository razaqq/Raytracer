# set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
	imgui
	STATIC
	"src/imgui.cpp"
	"src/imgui_demo.cpp"
	"src/imgui_draw.cpp"
	"src/imgui_tables.cpp"
	"src/imgui_widgets.cpp"
	"src/backends/imgui_impl_dx11.cpp"
	"src/backends/imgui_impl_win32.cpp"
)
target_include_directories(
	imgui
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

set(IMGUI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
