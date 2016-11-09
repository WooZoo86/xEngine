file(GLOB_RECURSE mesh_convert_tool_source *.h *.cc)

if(X_WINDOWS)
    add_executable(MeshConvertTool WIN32 ${mesh_convert_tool_source})
else()
    add_executable(MeshConvertTool ${mesh_convert_tool_source})
endif()

target_link_libraries(MeshConvertTool xEngine EASTL xEngineNuklear assimp)

if(X_ENGINE_OPTION_OPENGL)
    target_link_libraries(MeshConvertTool glad)
endif()

set_target_properties(MeshConvertTool PROPERTIES FOLDER "tool")