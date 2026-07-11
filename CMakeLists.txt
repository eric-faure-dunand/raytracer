cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

project(raytracer CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)
pkg_check_modules(SFML QUIET sfml-graphics sfml-window sfml-system)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
endif()

file(GLOB_RECURSE PROJECT_SRCS
    ${CMAKE_SOURCE_DIR}/project/src/*.cpp
)

add_executable(raytracer ${PROJECT_SRCS})

target_include_directories(raytracer
    PUBLIC
        project/includes
        project/includes/Display
        project/includes/Display/printer
        project/includes/Threading
        project/includes/core
        project/includes/core/ToFile
        project/includes/core/reader
        project/includes/core/reader/cfgReader
        project/includes/core/reader/libld
        project/includes/Manager
        project/includes/math
        project/includes/render
        project/includes/Manager/default
        project/includes/Manager/client
        project/includes/Manager/server
        project/includes/security
        project/includes/Error
        project/includes/Lights
        ${LIBCONFIG_INCLUDE_DIRS}
        ${SFML_INCLUDE_DIRS}
        shared
        shared/LIght
        shared/Object
        shared/Texture
        shared/math
        shared/Error
        shared/printer
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
)

target_link_libraries(raytracer
    PRIVATE
        ${LIBCONFIG_LIBRARIES}
    sfml-graphics
    sfml-window
    sfml-system
)

# Plugin compilation system
# Automatically compile all plugins from the plugins/ directory
# Each plugin directory should contain:
#   - One or more .cpp files implementing the plugin
#   - Optionally, custom includes (will use shared/ headers)
# Output: raytracer_<plugin_name>.so in the plugins/ directory


file(GLOB PLUGIN_DIRS "${CMAKE_SOURCE_DIR}/plugins/*")
foreach(PLUGIN_DIR ${PLUGIN_DIRS})
    if(IS_DIRECTORY "${PLUGIN_DIR}")
        get_filename_component(PLUGIN_NAME ${PLUGIN_DIR} NAME)
        file(GLOB_RECURSE PLUGIN_SRCS "${PLUGIN_DIR}/*.cpp")
        if(PLUGIN_SRCS)
            add_library(raytracer_${PLUGIN_NAME} SHARED ${PLUGIN_SRCS})
            set(PLUGIN_OUTPUT_NAME ${PLUGIN_NAME})
            if(PLUGIN_NAME STREQUAL "spheres")
                set(PLUGIN_OUTPUT_NAME "sphere")
            elseif(PLUGIN_NAME STREQUAL "planes")
                set(PLUGIN_OUTPUT_NAME "plane")
            elseif(PLUGIN_NAME STREQUAL "triangles")
                set(PLUGIN_OUTPUT_NAME "triangle")
            elseif(PLUGIN_NAME STREQUAL "cylinders")
                set(PLUGIN_OUTPUT_NAME "cylinder")
            endif()
            set_target_properties(raytracer_${PLUGIN_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "raytracer_${PLUGIN_OUTPUT_NAME}")
            target_include_directories(raytracer_${PLUGIN_NAME}
                PUBLIC
                    shared
                    shared/LIght
                    shared/Object
                    shared/Texture
                    shared/math
                    shared/Error
                    shared/printer
            )
            target_compile_options(raytracer_${PLUGIN_NAME}
                PRIVATE
                    -Wall
                    -Wextra
                    -Werror
            )
            message(STATUS "Building plugin: raytracer_${PLUGIN_NAME}.so")
        endif()
    endif()
endforeach()
