cmake_minimum_required(VERSION 3.26)
project(project)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TMP} )

# fileName, cpp file, all libraries
add_executable(testLinkedList ./tests/linkedListTest.c ./src/linkedList.c)
