cmake_minimum_required(VERSION 3.16)
project(arithm_func)

set(CMAKE_C_STANDARD 11)

set(SOURCE_LIB arithm_func.c)

include_directories(../rec_desc)

add_library(arithm_func STATIC ${SOURCE_LIB})

target_link_libraries(arithm_func rec_desc)