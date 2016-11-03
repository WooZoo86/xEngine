if(X_WINDOWS)
    add_executable(ShapeSample WIN32 ShapeSample.cc)
else()
    add_executable(ShapeSample ShapeSample.cc)
endif()

target_link_libraries(ShapeSample xEngine EASTL)

if(X_ENGINE_OPTION_OPENGL)
    target_link_libraries(ShapeSample glad)
endif()

set_target_properties(ShapeSample PROPERTIES FOLDER "sample")