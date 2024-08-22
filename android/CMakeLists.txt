cmake_minimum_required(VERSION 3.18)
project(sk_guitar_tuner VERSION 0.0.1 )
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANDROID_PLATFORM=android-24")
add_subdirectory (oboe)

include_directories (oboe/include)

file (GLOB SRC_FILES
        ../src/sk_guitar_tuner.cpp
)

add_library(sk_guitar_tuner SHARED
  ${SRC_FILES}
)

#set_target_properties(sk_guitar_tuner PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sk_guitar_tuner oboe)