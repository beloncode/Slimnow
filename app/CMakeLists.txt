
cmake_minimum_required(VERSION 3.22.1)
project(slimcore CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SLIMCORE_DIR "${CMAKE_SOURCE_DIR}/src/main/cpp")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

add_library(slim MODULE)
target_sources(slim PRIVATE
        "${SLIMCORE_DIR}/SlimVM.cpp"

        # Emotion Engine related source files
        "${SLIMCORE_DIR}/ee/MIPSvi.cpp"

        # Emulator core marshalling and layers communication within/or between JVM
        "${SLIMCORE_DIR}/JNInterface.cpp"

        )

target_compile_options(slim PRIVATE
        "-fvisibility=hidden;-Wall;-Werror;-Wno-unused-command-line-argument"
        "$<$<CONFIG:RELEASE>:-O3;-fsanitize-minimal-runtime;-fno-stack-protector>"
        "$<$<CONFIG:DEBUG>:-O0;-g3;-glldb;-fsanitize=undefined;-fstack-protector-strong>")

target_link_options(slim PRIVATE "-rdynamic"
        "$<$<CONFIG:Release>:-s>")

if(supported)
    set_property(TARGET slim PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()


target_link_libraries(slim android)
