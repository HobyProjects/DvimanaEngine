cmake_minimum_required(VERSION 3.15)
project(DviCore VERSION 1.0.0 LANGUAGES CXX C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(EnTT REQUIRED)
find_package(ImGuiDocking REQUIRED)
find_package(StbImage REQUIRED)


set(DVIMANA_CORE DviCore)
set(DVICORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)

set(
	DVICORE_HEADERS
	${DVICORE_DIR}/Core/Assert.hpp
	${DVICORE_DIR}/Core/Base.hpp
	${DVICORE_DIR}/Core/Platform.hpp
	${DVICORE_DIR}/Core/Log.hpp
	${DVICORE_DIR}/Core/KeyCodes.hpp
	${DVICORE_DIR}/Core/Window.hpp
	${DVICORE_DIR}/Core/Layer.hpp
	${DVICORE_DIR}/Core/LayerStack.hpp
	${DVICORE_DIR}/Core/TimeSteps.hpp
	${DVICORE_DIR}/Debug/Instrument.hpp
	${DVICORE_DIR}/Event/Event.hpp
	${DVICORE_DIR}/Event/EventReceiver.hpp
	${DVICORE_DIR}/Event/KeyboardEvent.hpp
	${DVICORE_DIR}/Event/MouseEvent.hpp
	${DVICORE_DIR}/Event/WindowEvent.hpp
	${DVICORE_DIR}/Event/Inputs.hpp
	${DVICORE_DIR}/Renderer/OpenGL.hpp
	${DVICORE_DIR}/ImGui/ImGuiKeyCodes.hpp
	${DVICORE_DIR}/ImGui/ImGuiLayer.hpp
	${DVICORE_DIR}/DviCore.hpp
)

set(
	DVICORE_SOURCES
	${DVICORE_DIR}/Core/Log.cpp
	${DVICORE_DIR}/Core/Window.cpp
	${DVICORE_DIR}/Core/LayerStack.cpp
	${DVICORE_DIR}/Event/EventReceiver.cpp
	${DVICORE_DIR}/Event/Inputs.cpp
	${DVICORE_DIR}/Renderer/OpenGL.cpp
	${DVICORE_DIR}/ImGui/ImGuiLayer.cpp
)

add_library(
	${DVIMANA_CORE} STATIC
	${DVICORE_HEADERS}
	${DVICORE_SOURCES}
)
	
target_link_libraries(
	${DVIMANA_CORE} 
		PUBLIC
			glfw
			spdlog::spdlog
			OpenGL::GL
			glm::glm
			EnTT::EnTT
			ImGui::ImGuiDocking
			Stb::StbImage
)

add_library(Dvimana::DviCore ALIAS ${DVIMANA_CORE})

set_target_properties(
	${DVIMANA_CORE} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	C_STANDARD 17
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)

target_include_directories(
	${DVIMANA_CORE} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
	TARGETS ${DVIMANA_CORE}
	EXPORT DviCoreTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} 
)

install(
	FILES ${DVICORE_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Dvimana
)

install(
	EXPORT DviCoreTargets
	FILE DviCoreTargets.cmake
	NAMESPACE Dvimana::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dvimana
)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Config/${DVIMANA_CORE}Config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/${DVIMANA_CORE}Config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dvimana
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/${DVIMANA_CORE}Config.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dvimana
)

configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/Config/${DVIMANA_CORE}.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${DVIMANA_CORE}.pc"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)


install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${DVIMANA_CORE}.pc"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)


# write_basic_package_version_file(
# 	${CMAKE_CURRENT_BINARY_DIR}/DviCoreConfigVersion.cmake
# 	VERSION 1.0.0
# 	COMPATIBILITY 1.0.0
# )

# install(
# 	FILES ${CMAKE_CURRENT_BINARY_DIR}/DviCoreConfigVersion.cmake
# 	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dvimana
# )