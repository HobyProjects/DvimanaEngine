###########################
## IMGUI DOCKING LIBRARY ##
###########################
cmake_minimum_required (VERSION 3.15)
project(ImGuiDocking C CXX)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
set(IMGUI_DOCKING_LIBEARY ImGuiDocking)

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
	${IMGUI_DOCKING_LIBEARY}
		STATIC 
			${IMGUI_HEADERS}
			${IMGUI_SOURCES}
)

target_include_directories(${IMGUI_DOCKING_LIBEARY} PRIVATE imgui imgui/backends)
target_link_libraries(${IMGUI_DOCKING_LIBEARY} PRIVATE glfw OpenGL::GL)
add_library(ImGui::ImGuiDocking ALIAS ${IMGUI_DOCKING_LIBEARY})

set_target_properties(
	${IMGUI_DOCKING_LIBEARY} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	C_STANDARD 17
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)

include(GNUInstallDirs)

target_include_directories(
	${IMGUI_DOCKING_LIBEARY} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
	TARGETS ${IMGUI_DOCKING_LIBEARY}
	EXPORT "${IMGUI_DOCKING_LIBEARY}Targets"
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} 
)

install(
	FILES ${IMGUI_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${IMGUI_DOCKING_LIBEARY}
)

install(
	EXPORT "${IMGUI_DOCKING_LIBEARY}Targets"
	FILE "${IMGUI_DOCKING_LIBEARY}Targets.cmake"
	NAMESPACE ImGui::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${IMGUI_DOCKING_LIBEARY}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/config/${IMGUI_DOCKING_LIBEARY}Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ImGuiDockingConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${IMGUI_DOCKING_LIBEARY}"
)

configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/config/${IMGUI_DOCKING_LIBEARY}.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${IMGUI_DOCKING_LIBEARY}.pc"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${IMGUI_DOCKING_LIBEARY}Config.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${IMGUI_DOCKING_LIBEARY}"
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${IMGUI_DOCKING_LIBEARY}.pc"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)