add_subdirectory(EASTL)

include_directories(mongoose)
add_library(mongoose mongoose/mongoose.c)

if(X_ENGINE_OPTION_GLAD_DEBUG)
    include_directories(glad-debug/include)
    add_library(glad glad-debug/src/glad.c)
else()
    include_directories(glad/include)
    add_library(glad glad/src/glad.c)
endif()

set_target_properties(EABase_ide PROPERTIES FOLDER "third_party")
set_target_properties(EASTL PROPERTIES FOLDER "third_party")
set_target_properties(mongoose PROPERTIES FOLDER "third_party")
set_target_properties(glad PROPERTIES FOLDER "third_party")