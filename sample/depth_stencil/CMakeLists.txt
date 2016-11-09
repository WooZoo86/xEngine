if(X_WINDOWS)
  add_executable(DepthStencilSample WIN32 DepthStencilSample.cc)
else()
  add_executable(DepthStencilSample DepthStencilSample.cc)
endif()

target_link_libraries(DepthStencilSample xEngine EASTL)

if(X_ENGINE_OPTION_OPENGL)
    target_link_libraries(DepthStencilSample glad)
endif()

set_target_properties(DepthStencilSample PROPERTIES FOLDER "sample")