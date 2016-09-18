if(X_ENGINE_OPTION_DEBUG)
    add_definitions("-DX_DEBUG=1")
endif()

if(MSVC)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/windows.cmake)
elseif(APPLE)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/osx.cmake)
else()
    include(${PROJECT_SOURCE_DIR}/cmake/platform/linux.cmake)
endif()

if(X_ENGINE_OPTION_GLAD_DEBUG)
    include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/third_party/EASTL/include
        ${PROJECT_SOURCE_DIR}/third_party/EASTL/test/packages/EABase/include/Common
        ${PROJECT_SOURCE_DIR}/third_party/mongoose
        ${PROJECT_SOURCE_DIR}/third_party/glm/glm
        ${PROJECT_SOURCE_DIR}/third_party/glad-debug/include
    )
else()
    include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/third_party/EASTL/include
        ${PROJECT_SOURCE_DIR}/third_party/EASTL/test/packages/EABase/include/Common
        ${PROJECT_SOURCE_DIR}/third_party/mongoose
        ${PROJECT_SOURCE_DIR}/third_party/glm/glm
        ${PROJECT_SOURCE_DIR}/third_party/glad/include
    )
endif()

file(GLOB_RECURSE src *.h *.cc)

add_library(xEngine ${src})