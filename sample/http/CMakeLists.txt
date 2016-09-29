add_executable(HTTPSample HTTPSample.cc)

target_link_libraries(HTTPSample xEngine EASTL mongoose)

set_target_properties(HTTPSample PROPERTIES FOLDER "sample")