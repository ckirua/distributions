cmake_minimum_required(VERSION 3.20)
project(distributions LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++26>)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14")
    message(FATAL_ERROR "C++26 requires GCC 14+ (found ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})")
endif()

option(DISTRIBUTIONS_ENABLE_NATIVE "Use -march=native" OFF)

add_library(distributions INTERFACE)
target_include_directories(distributions INTERFACE "${CMAKE_SOURCE_DIR}/include")

function(apply_release_flags target)
    target_compile_options(${target} PRIVATE -O3 -fno-math-errno)
    if(DISTRIBUTIONS_ENABLE_NATIVE)
        target_compile_options(${target} PRIVATE -march=native)
    endif()
endfunction()

set(ISPC_EXECUTABLE "${CMAKE_SOURCE_DIR}/.tools/ispc-v1.22.0-linux/bin/ispc")
if(EXISTS "${ISPC_EXECUTABLE}")
    set(ISPC_OBJ "${CMAKE_BINARY_DIR}/kernels_ispc.o")
    set(ISPC_HDR "${CMAKE_BINARY_DIR}/kernels_ispc.h")
    add_custom_command(
        OUTPUT "${ISPC_OBJ}" "${ISPC_HDR}"
        COMMAND "${ISPC_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/ispc/kernels.ispc"
            -h "${ISPC_HDR}"
            -o "${ISPC_OBJ}"
            --target=avx2-i32x8
        DEPENDS
            "${CMAKE_SOURCE_DIR}/ispc/kernels.ispc"
            "${CMAKE_SOURCE_DIR}/ispc/pcg.ispc"
        COMMENT "Compiling ISPC kernels"
    )
    add_custom_target(distributions_ispc_obj DEPENDS "${ISPC_OBJ}" "${ISPC_HDR}")
endif()

function(link_ispc target)
    if(TARGET distributions_ispc_obj)
        target_include_directories(${target} PRIVATE "${CMAKE_BINARY_DIR}")
        target_compile_definitions(${target} PRIVATE DISTRIBUTIONS_HAS_ISPC=1)
        add_dependencies(${target} distributions_ispc_obj)
        target_sources(${target} PRIVATE "${ISPC_OBJ}")
        set_source_files_properties("${ISPC_OBJ}" PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
    endif()
endfunction()

add_executable(run_bench bench/run_bench.cpp)
target_link_libraries(run_bench PRIVATE distributions)
apply_release_flags(run_bench)
link_ispc(run_bench)

add_executable(sample_dump bench/sample_dump.cpp)
target_link_libraries(sample_dump PRIVATE distributions)
apply_release_flags(sample_dump)
link_ispc(sample_dump)

macro(add_dist_test name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE distributions)
    target_include_directories(${name} PRIVATE "${CMAKE_SOURCE_DIR}/bench")
    apply_release_flags(${name})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

add_dist_test(bernoulli_test tests/cpp/bernoulli_test.cpp)
add_dist_test(smoke_all_test tests/cpp/smoke_all_test.cpp)

option(DISTRIBUTIONS_BUILD_EXAMPLES "Build examples/ targets" ON)
if(DISTRIBUTIONS_BUILD_EXAMPLES)
    add_executable(example_cpp_basic examples/cpp/basic.cpp)
    target_link_libraries(example_cpp_basic PRIVATE distributions)
    apply_release_flags(example_cpp_basic)

    add_executable(example_c_api examples/cpp/c_api.c cydist/cydist_shim.cpp)
    target_include_directories(example_c_api PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/cydist"
    )
    set_source_files_properties(cydist/cydist_shim.cpp PROPERTIES LANGUAGE CXX)
    apply_release_flags(example_c_api)
endif()

enable_testing()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++26" COMPILER_SUPPORTS_CXX26)
if(NOT COMPILER_SUPPORTS_CXX26)
    message(FATAL_ERROR "Compiler does not accept -std=c++26")
endif()
