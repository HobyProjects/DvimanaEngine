cmake_minimum_required(VERSION 3.15)
project(Dvimana VERSION 1.0.0 LANGUAGES C CXX)

find_package(DviCore REQUIRED)
set(DVIMANA_APPLICATION Dvimana)

set(
    DVIMANA_APP_HEADERS
        Src/Application/Application.hpp
        Src/Editor/EditorLayer.hpp
        Src/Camera/Camera.hpp
)

set(
    DVIMANA_APP_SOURCES
        Src/Application/Application.cpp
        Src/Editor/EditorLayer.cpp
        Src/EntryPoint/Dvimana.cpp
        Src/Camera/Camera.cpp
)

add_executable(${DVIMANA_APPLICATION} ${DVIMANA_APP_HEADERS} ${DVIMANA_APP_SOURCES})

target_include_directories(
    ${DVIMANA_APPLICATION} 
        PRIVATE
            Src/Application
            Src/Editor
            Src/EntryPoint
            Src/Camera
)

target_link_libraries(${DVIMANA_APPLICATION} PRIVATE Dvimana::DviCore)