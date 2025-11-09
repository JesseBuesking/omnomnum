cmake_minimum_required(VERSION 3.12)
project(OmNomNum VERSION 0.0.2 LANGUAGES C CXX)

# C/C++ Standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type defaults
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Options
option(SCANNER_FRACTIONS "Enable scanner-side fraction recognition" ON)
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(USE_LTO "Enable Link Time Optimization" OFF)

# Enable fraction scanning by default
if(SCANNER_FRACTIONS)
    add_compile_definitions(SCANNER_FRACTIONS=1)
endif()

# LTO support
if(USE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "LTO enabled")
    else()
        message(WARNING "LTO requested but not supported: ${ipo_error}")
    endif()
endif()

# Compiler flags
set(WARNING_FLAGS -Wall -W -Wno-missing-field-initializers)
add_compile_options(${WARNING_FLAGS})

# Architecture-specific flags (SSE4.2 for x86/x64)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
    add_compile_options(-msse4.2)
endif()

# Release/Debug flags
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O3)
    add_compile_definitions(NDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-g -ggdb -O1)
    add_compile_definitions(DEBUG print_errors)
endif()

# Find optional build tools (lemon, re2c) for regeneration
find_program(LEMON_EXECUTABLE lemon)
find_program(RE2C_EXECUTABLE re2c)

if(LEMON_EXECUTABLE AND RE2C_EXECUTABLE)
    message(STATUS "Found lemon: ${LEMON_EXECUTABLE}")
    message(STATUS "Found re2c: ${RE2C_EXECUTABLE}")
    set(CAN_REGENERATE TRUE)
else()
    message(STATUS "lemon/re2c not found; using checked-in generated files")
    set(CAN_REGENERATE FALSE)
endif()

# Custom targets for regenerating parser/scanner (if tools available)
if(CAN_REGENERATE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.c ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
        COMMAND ${LEMON_EXECUTABLE} -Tlempar.c parser.yy
        COMMAND ${CMAKE_COMMAND} -E copy parser.h parser.h.orig
        COMMAND awk -f fix_parser_h.awk parser.h.orig > parser.h
        COMMAND ${CMAKE_COMMAND} -E remove parser.h.orig
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS parser.yy lempar.c fix_parser_h.awk
        COMMENT "Regenerating parser with lemon"
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c
        COMMAND ${RE2C_EXECUTABLE} -o scanner.c scanner.re
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS scanner.re
        COMMENT "Regenerating scanner with re2c"
    )

    add_custom_target(regen
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.c ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c
        COMMENT "Regenerating parser and scanner"
    )
endif()

# Core library sources
set(CORE_SOURCES
    parser_compat.c
    omnomnum.c
    scanner.c
    scan.c
    sds.c
    itoa.c
    dtoa.c
    scanner.def.c
    util.c
    parser.c
    grisu2/grisu2.c
    branchlut/branchlut.c
)

# Create static library
add_library(omnomnum_lib STATIC ${CORE_SOURCES})
target_link_libraries(omnomnum_lib PUBLIC m)
target_include_directories(omnomnum_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(omnomnum main.c)
target_link_libraries(omnomnum PRIVATE omnomnum_lib)

# Installation
install(TARGETS omnomnum DESTINATION bin)
install(FILES omnomnum.h scanner.h scan.h parser.h sds.h itoa.h dtoa.h scanner.def.h
        DESTINATION include/omnomnum)

# Testing
if(BUILD_TESTS)
    enable_testing()

    # Try to find GTest
    find_package(GTest)
    find_package(yaml-cpp)

    if(GTest_FOUND AND yaml-cpp_FOUND)
        message(STATUS "Found GTest: ${GTEST_LIBRARIES}")
        message(STATUS "Found yaml-cpp: ${yaml-cpp_VERSION}")

        add_executable(test_omnomnum test/test_omnomnum.c)
        target_link_libraries(test_omnomnum PRIVATE
            omnomnum_lib
            GTest::gtest
            GTest::gtest_main
            yaml-cpp
            pthread
        )
        target_include_directories(test_omnomnum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

        add_test(NAME omnomnum_tests COMMAND test_omnomnum WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)

        # Utility tests
        add_executable(test_util test/test_util.c util.c)
        target_link_libraries(test_util PRIVATE
            GTest::gtest
            GTest::gtest_main
            yaml-cpp
            pthread
        )

        add_test(NAME util_tests COMMAND test_util WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    else()
        if(NOT GTest_FOUND)
            message(WARNING "GTest not found. Tests will not be built. Install with: sudo apt-get install libgtest-dev")
        endif()
        if(NOT yaml-cpp_FOUND)
            message(WARNING "yaml-cpp not found. Tests will not be built. Install with: sudo apt-get install libyaml-cpp-dev")
        endif()
    endif()
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    find_package(benchmark)

    if(benchmark_FOUND)
        message(STATUS "Found Google Benchmark")

        # Get git metadata
        execute_process(
            COMMAND git rev-parse --short=12 HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_SHA
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND git describe --always --dirty --tags
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_DESC
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        string(TIMESTAMP BUILD_TIME UTC)

        add_executable(test_benchmark test/test_benchmark.c)
        target_link_libraries(test_benchmark PRIVATE
            omnomnum_lib
            benchmark::benchmark
            benchmark::benchmark_main
            pthread
        )
        target_compile_definitions(test_benchmark PRIVATE
            GIT_SHA="${GIT_SHA}"
            GIT_DESC="${GIT_DESC}"
            BUILD_TIME="${BUILD_TIME}"
        )
    else()
        message(WARNING "Google Benchmark not found. Benchmarks will not be built.")
    endif()
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "OmNomNum Configuration Summary:")
message(STATUS "  Version:              ${PROJECT_VERSION}")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler:           ${CMAKE_C_COMPILER}")
message(STATUS "  CXX Compiler:         ${CMAKE_CXX_COMPILER}")
message(STATUS "  Scanner Fractions:    ${SCANNER_FRACTIONS}")
message(STATUS "  Build Tests:          ${BUILD_TESTS}")
message(STATUS "  Build Benchmarks:     ${BUILD_BENCHMARKS}")
message(STATUS "  LTO:                  ${USE_LTO}")
message(STATUS "  Can Regenerate:       ${CAN_REGENERATE}")
message(STATUS "")
