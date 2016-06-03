include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/third_party/EASTL/include
    ${PROJECT_SOURCE_DIR}/third_party/EASTL/test/packages/EABase/include/Common
    ${PROJECT_SOURCE_DIR}/third_party/mongoose
)

add_subdirectory(application)
add_subdirectory(http)
add_subdirectory(storage)
add_subdirectory(window)