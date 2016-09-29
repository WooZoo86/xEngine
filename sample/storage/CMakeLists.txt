add_executable(StorageSample StorageSample.cc)

target_link_libraries(StorageSample xEngine EASTL)

set_target_properties(StorageSample PROPERTIES FOLDER "sample")