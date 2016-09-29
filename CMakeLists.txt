cmake_minimum_required(VERSION 3.4)

project(xEngine)

set(CMAKE_CXX_STANDARD 11)

option(X_ENGINE_OPTION_OPENGL "Use OpenGL" OFF)
option(X_ENGINE_OPTION_DX11 "Use DirectX 11" OFF)

option(X_ENGINE_OPTION_USE_GLFW "Use GLFW for window module" OFF)

option(X_ENGINE_OPTION_GLAD_DEBUG "OpenGL Loader Debug Mode" ON)
option(X_ENGINE_OPTION_DEBUG "Debug Mode" ON)
option(X_ENGINE_OPTION_RTTI "Enable RTTI" OFF)

add_subdirectory(src)
add_subdirectory(third_party)
add_subdirectory(sample)