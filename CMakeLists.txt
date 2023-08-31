cmake_minimum_required (VERSION 2.8)

# 项目信息
project (SuperMario)
cmake_policy(SET CMP0015 NEW)
# set (CMAKE_CXX_COMPILER "/Library/Developer/CommandLineTools/usr/bin/g++")
set (CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SRCS)

# find_library(SFML_LIB_SYSTEM libsfml-system.dylib)
# message(STATUS "SFML_LIB_SYSTEM = " ${SFML_LIB_SYSTEM})
# find_library(SFML_LIB_GRAPHICS sfml-graphics)
# message(STATUS "SFML_LIB_GRAPHICS = " ${SFML_LIB_GRAPHICS})

link_directories(/Library/Frameworks/)
link_directories(/usr/local/lib)

include_directories(/usr/local/include)

add_executable(SuperMario ${DIR_SRCS})


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)

target_link_libraries(SuperMario sfml-system sfml-window sfml-graphics sfml-audio)