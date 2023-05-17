
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
        "${SLIMCORE_DIR}/ee/COP0.cpp"

        # Emulator core marshalling and layers communication within/or between JVM
        "${SLIMCORE_DIR}/JNInterface.cpp"
        "${SLIMCORE_DIR}/Executor.cpp"

        )

# -fsanitize-minimal-runtime It's suitable do use in a release build, but for performance reasons,
# we will only use inside of debug build
target_compile_options(slim PRIVATE
        "-fvisibility=hidden;-Wall;-Werror;-Wno-unused-command-line-argument"
        "$<$<CONFIG:RELEASE>:-O3;-fno-stack-protector;-fdata-sections;-ffunction-sections>"
        "$<$<CONFIG:DEBUG>:-O0;-g3;-glldb;-fsanitize-minimal-runtime;-fstack-protector-strong>")

target_link_options(slim PRIVATE "-rdynamic"
        "$<$<CONFIG:Release>:-s;-Wl,--gc-sections>")

if(supported)
    set_property(TARGET slim PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()


target_link_libraries(slim android)
