cmake_minimum_required(VERSION 3.10.2)


include_directories(${CMAKE_CURRENT_LIST_DIR}/include)


# Add FFmpeg libraries as imported targets
add_library(oboe SHARED IMPORTED GLOBAL)
set_target_properties(oboe PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/${ANDROID_ABI}/liboboe.so)

