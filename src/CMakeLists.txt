# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(sk_guitar_tuner_library VERSION 0.0.1 LANGUAGES C)

add_library(sk_guitar_tuner SHARED
  "sk_guitar_tuner.c"
)

set_target_properties(sk_guitar_tuner PROPERTIES
  PUBLIC_HEADER sk_guitar_tuner.h
  OUTPUT_NAME "sk_guitar_tuner"
)

target_compile_definitions(sk_guitar_tuner PUBLIC DART_SHARED_LIB)
