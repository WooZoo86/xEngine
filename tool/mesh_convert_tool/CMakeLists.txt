file(GLOB_RECURSE mesh_convert_tool_source *.h *.cc)

if(X_WINDOWS)
    add_executable(MeshConvertTool WIN32 ${mesh_convert_tool_source})
else()
    add_executable(MeshConvertTool ${mesh_convert_tool_source})
endif()

target_link_libraries(MeshConvertTool xEngine xEngineNuklear assimp)

set_target_properties(MeshConvertTool PROPERTIES FOLDER "tool")