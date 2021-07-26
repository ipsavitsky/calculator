cmake_minimum_required(VERSION 3.16)
project(calculator C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_EXE calculator.c)

include_directories(src/arithm_func src/errors src/rec_desc src/RPN src/stack)

add_executable(calculator ${SOURCE_EXE})

add_subdirectory(src/arithm_func)
add_subdirectory(src/errors)
add_subdirectory(src/rec_desc)
add_subdirectory(src/RPN)
add_subdirectory(src/stack)

target_link_libraries(calculator arithm_func errors rec_desc RPN stack)