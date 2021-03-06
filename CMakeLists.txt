cmake_minimum_required(VERSION 3.2)
project(universal_handler_map CXX)
set(TARGET universal_handler_map)

set(BOOST_MIN_VERSION "1.59.0")
set(Boost_ADDITIONAL_VERSIONS "1.59.0" "1.59")
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT ${MY_BOOST_DIR}) 

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED) 

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message("UNIX congiguration, Clang")
    set(CMAKE_CXX_FLAGS "-std=gnu++11 -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "-std=gnu++11")
elseif(${CMAKE_COMPILER_IS_GNUCC})
    message("UNIX congiguration, GCC")
    set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread")
elseif(${WIN32})
    message("Windows configuraion")
    set(CMAKE_CXX_FLAGS "/EHsc")
    set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})

    # define WIN32 macro version
    macro(get_WIN32_WINNT version)
        if (WIN32 AND CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REPLACE "." "" ver ${ver})
            string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

            set(${version} "0x${ver}")
        endif()
    endmacro()

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)

endif()

add_subdirectory(example)
add_subdirectory(test/func_tests)
