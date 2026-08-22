cmake_minimum_required(VERSION 3.16)
project(raytracer CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

# GLFW : built from source, cross-platform (Win32 / X11 / Wayland).
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# glad : vendored, pre-generated GL core 4.3 loader (no tooling needed).
add_library(glad STATIC vendor/glad/src/gl.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Dear ImGui : vendored core + glfw/opengl3 backends (docking branch).
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------
file(GLOB_RECURSE PROJECT_SRCS ${CMAKE_SOURCE_DIR}/project/src/*.cpp)

add_executable(raytracer ${PROJECT_SRCS})

target_include_directories(raytracer
    PRIVATE
        project/includes
        project/includes/Error
        project/includes/printer
        project/includes/UI
        project/includes/Threading
        project/includes/core
        project/includes/core/scene
        project/includes/core/ToFile
        project/includes/core/reader
        project/includes/core/reader/cfgReader
        project/includes/core/reader/libld
        project/includes/Manager
        project/includes/Manager/default
        project/includes/Manager/client
        project/includes/Manager/server
        project/includes/render
        project/includes/render/GPULink
        project/includes/security
        ${LIBCONFIG_INCLUDE_DIRS}
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -g3
)

target_link_libraries(raytracer
    PRIVATE
        glfw
        glad
        imgui
        OpenGL::GL
        ${LIBCONFIG_LIBRARIES}
)
