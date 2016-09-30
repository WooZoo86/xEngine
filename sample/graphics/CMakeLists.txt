if(X_WINDOWS)
	add_executable(GraphicsSample WIN32 GraphicsSample.cc)
else()
	add_executable(GraphicsSample GraphicsSample.cc)
endif()

target_link_libraries(GraphicsSample xEngine EASTL)

if(X_ENGINE_OPTION_USE_GLFW)
    target_link_libraries(GraphicsSample xEngine EASTL glfw3)
endif()

if(X_ENGINE_OPTION_OPENGL)
    target_link_libraries(GraphicsSample xEngine glad)
endif()

set_target_properties(GraphicsSample PROPERTIES FOLDER "sample")