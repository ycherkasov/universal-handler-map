cmake_minimum_required(VERSION 3.2)
project(handler_map_test_app CXX)
set(TARGET handler_map_test_app)
 
cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

if(Boost_FOUND)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})                                                                                                                                                                                                                             
endif()
 
########################################################
# source files
file(GLOB project_SRCS *.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/${TARGET}/*.h)
  
########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
source_group("Sources" FILES ${SOURCES})
 
 
########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../include)
include_directories(${INCLUDE_DIRS})
 
########################################################
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})