cmake_minimum_required(VERSION 3.23)

project(fmtlib-custom-types)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)

add_compile_options(-pedantic -Werror -Wall -Wno-missing-braces -fno-omit-frame-pointer "$<$<CONFIG:Debug>:-fsanitize=address>")

include(CTest)

