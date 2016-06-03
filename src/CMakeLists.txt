if (X_ENGINE_OPTION_DEBUG)
    add_definitions("-DX_DEBUG=1")
endif ()

if (MSVC)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/windows.cmake)
elseif (APPLE)
    include(${PROJECT_SOURCE_DIR}/cmake/platform/osx.cmake)
else ()
    include(${PROJECT_SOURCE_DIR}/cmake/platform/linux.cmake)
endif ()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/third_party/EASTL/include
    ${PROJECT_SOURCE_DIR}/third_party/EASTL/test/packages/EABase/include/Common
    ${PROJECT_SOURCE_DIR}/third_party/mongoose
)

file(GLOB_RECURSE src *.h *.cc)

add_library(xEngine ${src})