cmake_minimum_required(VERSION 3.4)

project(xEngine)

set(CMAKE_CXX_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(X_ENGINE_OPTION_OPENGL "Use OpenGL" ON)
option(X_ENGINE_OPTION_D3D11 "Use DirectX3D 11" OFF)

option(X_ENGINE_OPTION_GLAD_DEBUG "OpenGL Loader Debug Mode" ON)
option(X_ENGINE_OPTION_DEBUG "Debug Mode" ON)
option(X_ENGINE_OPTION_RTTI "Enable RTTI" OFF)

if(MSVC)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/windows.cmake)
elseif(APPLE)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/macOS.cmake)
else()
    include(${PROJECT_SOURCE_DIR}/cmake/platform/linux.cmake)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

add_subdirectory(runtime)
add_subdirectory(third_party)
add_subdirectory(plugin)
add_subdirectory(sample)
add_subdirectory(tool)