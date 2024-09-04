set(DVIMANA_APPLICATION Dvimana)
set(DVIMANA_APP_SRCDIR ${PROJECT_SOURCE_DIR}/Src/${DVIMANA_APPLICATION})

set(
    DVIMANA_APP_HEADERS
        ${DVIMANA_APP_SRCDIR}/Application/Application.hpp
        ${DVIMANA_APP_SRCDIR}/Editor/EditorLayer.hpp
        

)

set(
    DVIMANA_APP_SOURCES
        ${DVIMANA_APP_SRCDIR}/Application/Application.cpp
        ${DVIMANA_APP_SRCDIR}/Editor/EditorLayer.cpp
        ${DVIMANA_APP_SRCDIR}/EntryPoint/Dvimana.cpp

)

add_executable(${DVIMANA_APPLICATION} ${DVIMANA_APP_HEADERS} ${DVIMANA_APP_SOURCES})

add_custom_target(
    DVIMANA_RESOURCES_COPY
        ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/Resources
                ${PROJECT_BINARY_DIR}/Src/${DVIMANA_APPLICATION}/${CMAKE_BUILD_TYPE}/
            COMMENT "Copying resources to binary directory"
)

add_dependencies(${DVIMANA_APPLICATION} DVIMANA_RESOURCES_COPY)


target_include_directories(
    ${DVIMANA_APPLICATION} 
        PRIVATE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Src/${DVIMANA_APPLICATION}/Application>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Src/${DVIMANA_APPLICATION}/Editor>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Src/${DVIMANA_APPLICATION}/EntryPoint>
)

target_link_libraries(${DVIMANA_APPLICATION} PRIVATE Dvimana::Core)

install(
	TARGETS
		${DVIMANA_APPLICATION}
			RUNTIME DESTINATION ${PROJECT_INSTALL_DIR}/bin 
)