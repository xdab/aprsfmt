cmake_minimum_required(VERSION 3.10)

project(aprsfmt
    VERSION 1.0.0
    DESCRIPTION "Command-line utility for producing and parsing APRS packets"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -O2)

add_executable(aprsfmt main.c sum.c)

install(TARGETS aprsfmt
    RUNTIME DESTINATION bin
)
