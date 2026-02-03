cmake_minimum_required(VERSION 3.10)

project(aprsfmt
    VERSION 1.0.0
    DESCRIPTION "Command-line utility for producing and parsing APRS packets"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Option to enable debug builds
option(APRSFMT_DEBUG "Enable debugging symbols and disable optimizations" OFF)

include_directories(include)

# Set compiler flags based on debug option or build type
if(APRSFMT_DEBUG OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_FLAGS "-pg -O0 -DDEBUG")
    set(RELEASE_FLAGS "")
    set(RELEASE_LINK_FLAGS "")
else()
    set(DEBUG_FLAGS "")
    set(RELEASE_FLAGS "-O3 -DNDEBUG -flto")
    set(RELEASE_LINK_FLAGS "-Wl,--gc-sections")
endif()

# Apply flags to all targets
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${RELEASE_LINK_FLAGS}")

add_compile_options(-Wall -Wextra)

add_subdirectory(libs/libtnc)

add_executable(aprsfmt src/main.c src/arguments.c src/position_uncompressed.c src/position_compressed.c)
target_link_libraries(aprsfmt PRIVATE tnc m)
target_include_directories(aprsfmt PRIVATE libs/libtnc/include)

install(TARGETS aprsfmt
    RUNTIME DESTINATION bin
)

add_executable(aprsfmt_test test/main_test.c test/test.c src/position_uncompressed.c src/position_compressed.c)

target_link_libraries(aprsfmt_test m)
target_include_directories(aprsfmt_test PRIVATE include)

# Strip symbols in Release builds
if(NOT APRSFMT_DEBUG AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET aprsfmt POST_BUILD COMMAND ${CMAKE_STRIP} $<TARGET_FILE:aprsfmt>)
    add_custom_command(TARGET aprsfmt_test POST_BUILD COMMAND ${CMAKE_STRIP} $<TARGET_FILE:aprsfmt_test>)
endif()
