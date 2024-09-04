###########################
## IMGUI DOCKING LIBRARY ##
###########################
cmake_minimum_required (VERSION 3.15)
project(imgui_docking C CXX)

find_package(glfw3 REQUIRED)

set(
    IMGUI_HEADERS
        imgui/imconfig.h
		imgui/imgui.h
		imgui/imgui_internal.h
		imgui/imstb_rectpack.h
		imgui/imstb_textedit.h
		imgui/imstb_truetype.h
        imgui/backends/imgui_impl_glfw.h
        imgui/backends/imgui_impl_opengl3.h
)

set(
	IMGUI_SOURCES
		imgui/imgui.cpp
		imgui/imgui_draw.cpp
		imgui/imgui_widgets.cpp
		imgui/imgui_tables.cpp
		imgui/imgui_demo.cpp
		imgui/backends/imgui_impl_glfw.cpp
		imgui/backends/imgui_impl_opengl3.cpp
)

add_library(
	imgui_docking
		STATIC 
			${IMGUI_HEADERS}
			${IMGUI_SOURCES}
)

target_include_directories(imgui_docking PRIVATE imgui imgui/backends)
target_link_libraries(imgui_docking PRIVATE glfw3)
add_library(ImGui::ImGuiDocking ALIAS imgui_docking)


set_target_properties(
	imgui_docking PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	C_STANDARD 17
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)

include(GNUInstallDirs)

target_include_directories(
	imgui_docking PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
	TARGETS imgui_docking
	EXPORT ImGuiTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} 
)

install(
	FILES ${IMGUI_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
	EXPORT ImGuiTargets
	FILE ImGuiTargets.cmake
	NAMESPACE ImGui::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgui_docking
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_LIST_DIR}/ImGuiConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgui_docking
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/imgui_docking
)

