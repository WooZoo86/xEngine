add_subdirectory(EASTL)

include_directories(mongoose)
add_library(mongoose mongoose/mongoose.c)