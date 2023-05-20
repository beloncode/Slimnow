
cmake_minimum_required(VERSION 3.22.1)
project(slimcore CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SLIMCORE_DIR "${CMAKE_SOURCE_DIR}/src/main/cpp")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# -fsanitize-minimal-runtime It's suitable to use in a "release" build, but for performance reasons,
# we will only use inside of "debug" based builds
set(CMAKE_CXX_FLAGS "-fvisibility=hidden -Wall -Werror -Wno-unused-command-line-argument")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -fno-stack-protector -fdata-sections -ffunction-sections -flto=full -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -glldb -fsanitize-minimal-runtime -fno-omit-frame-pointer -fstack-protector-strong")

add_library(slim MODULE)
add_subdirectory("libraries/fmt" EXCLUDE_FROM_ALL)

target_sources(slim PRIVATE
        "${SLIMCORE_DIR}/slim_vm.cpp"
        "${SLIMCORE_DIR}/tasks/executor.cpp"
        "${SLIMCORE_DIR}/logger.cpp"

        # Emotion Engine related source files
        "${SLIMCORE_DIR}/ee/MIPS_vi.cpp"
        "${SLIMCORE_DIR}/ee/COP0.cpp"

        # Emulator core marshalling and layers communication within/or between JVM
        "${SLIMCORE_DIR}/jni_interface.cpp"


        )
target_compile_options(slim PRIVATE "$<$<CONFIG:RELEASE>:>" "$<$<CONFIG:DEBUG>:>")
target_link_libraries(slim fmt::fmt log)

target_include_directories(slim PRIVATE ${SLIMCORE_DIR})
target_link_options(slim PRIVATE "$<$<CONFIG:Release>:-s;-Wl,--gc-sections>" "$<$<CONFIG:DEBUG>:-rdynamic>")

if(supported)
    set_property(TARGET slim PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

