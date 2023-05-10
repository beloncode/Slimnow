
cmake_minimum_required(VERSION 3.22.1)
project(slimcore CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(slimcore_DIR "${CMAKE_SOURCE_DIR}/src/main/cpp")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

add_library(slim MODULE)

if(supported)
    set_property(TARGET slim PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

target_sources(slim PRIVATE "${slimcore_DIR}/native-lib.cpp")

target_link_libraries(slim android)