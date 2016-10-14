if(X_WINDOWS)
    add_executable(NuklearSample WIN32 NuklearSample.cc)
else()
    add_executable(NuklearSample NuklearSample.cc)
endif()

target_link_libraries(NuklearSample xEngine EASTL xEngineNuklear)

if(X_ENGINE_OPTION_USE_GLFW)
    target_link_libraries(NuklearSample glfw3)
endif()

if(X_ENGINE_OPTION_OPENGL)
    target_link_libraries(NuklearSample glad)
endif()

set_target_properties(NuklearSample PROPERTIES FOLDER "sample")