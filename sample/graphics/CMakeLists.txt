add_executable(GraphicsSample GraphicsSample.cc)

target_link_libraries(GraphicsSample xEngine EASTL glad glfw3)

set_target_properties(GraphicsSample PROPERTIES FOLDER "sample")