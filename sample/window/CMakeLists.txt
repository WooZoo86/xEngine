if(X_WINDOWS)
	add_executable(WindowSample WIN32 WindowSample.cc)
else()
	add_executable(WindowSample WindowSample.cc)
endif()

if(X_ENGINE_OPTION_USE_GLFW)
    target_link_libraries(WindowSample xEngine EASTL glfw3)
else()
    target_link_libraries(WindowSample xEngine EASTL)
endif()

set_target_properties(WindowSample PROPERTIES FOLDER "sample")