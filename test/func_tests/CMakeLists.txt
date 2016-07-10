cmake_minimum_required(VERSION 2.8)
project(handler_map_functional_test CXX)
set(TARGET handler_map_functional_test)

cmake_policy(SET CMP0015 NEW)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")

find_package(Boost 1.59.0 COMPONENTS unit_test_framework REQUIRED) 
 
########################################################
# source files
file(GLOB SOURCES *.cpp)
 
########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../../include ${Boost_INCLUDE_DIRS})
include_directories(${INCLUDE_DIRS})
 
########################################################
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
