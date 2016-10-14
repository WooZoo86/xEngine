add_library(xEngineNuklear NuklearGUI.cc)

target_link_libraries(xEngineNuklear xEngine EASTL)

set_target_properties(xEngineNuklear PROPERTIES FOLDER "plugin")