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
option(DISTRIBUTIONS_ENABLE_SIMD "Build AVX2 SIMD Tier-C paths (off by default)" OFF)
option(DISTRIBUTIONS_ENABLE_AVX512 "Build AVX-512 Tier-C paths for double continuous (requires SIMD)" OFF)

if(DISTRIBUTIONS_ENABLE_AVX512 AND NOT DISTRIBUTIONS_ENABLE_SIMD)
    message(FATAL_ERROR "DISTRIBUTIONS_ENABLE_AVX512 requires DISTRIBUTIONS_ENABLE_SIMD=ON")
endif()

add_library(distributions INTERFACE)
target_include_directories(distributions INTERFACE "${CMAKE_SOURCE_DIR}/include")

function(apply_release_flags target)
    target_compile_options(${target} PRIVATE -O3 -fno-math-errno)
    if(DISTRIBUTIONS_ENABLE_NATIVE)
        target_compile_options(${target} PRIVATE -march=native)
    endif()
    if(DISTRIBUTIONS_ENABLE_SIMD)
        target_compile_definitions(${target} PRIVATE DISTRIBUTIONS_HAS_AVX2=1)
        target_compile_options(${target} PRIVATE -mavx2 -mfma)
        target_link_libraries(${target} PRIVATE mvec)
    endif()
    if(DISTRIBUTIONS_ENABLE_AVX512)
        target_compile_definitions(${target} PRIVATE DISTRIBUTIONS_HAS_AVX512=1)
    endif()
endfunction()

add_executable(run_bench bench/run_bench.cpp)
target_link_libraries(run_bench PRIVATE distributions)
target_include_directories(run_bench PRIVATE "${CMAKE_SOURCE_DIR}/bench")
apply_release_flags(run_bench)

add_executable(sample_dump bench/sample_dump.cpp)
target_link_libraries(sample_dump PRIVATE distributions)
target_include_directories(sample_dump PRIVATE "${CMAKE_SOURCE_DIR}/bench")
apply_release_flags(sample_dump)

macro(add_dist_test name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE distributions)
    target_include_directories(${name} PRIVATE "${CMAKE_SOURCE_DIR}/bench")
    apply_release_flags(${name})
    add_test(NAME ${name} COMMAND ${name})
endmacro()

add_dist_test(bernoulli_test tests/cpp/bernoulli_test.cpp)
add_dist_test(counter_rng_test tests/cpp/counter_rng_test.cpp)
add_dist_test(concepts_test tests/cpp/concepts_test.cpp)
add_dist_test(param_validate_smoke_test tests/cpp/param_validate_smoke_test.cpp)
add_dist_test(simd_config_test tests/cpp/simd_config_test.cpp)
add_dist_test(simd_uniform_test tests/cpp/simd_uniform_test.cpp)
add_dist_test(smoke_all_test tests/cpp/smoke_all_test.cpp)

add_executable(cydist_shim_span_test tests/cpp/cydist_shim_span_test.cpp cydist/cydist_shim.cpp)
target_include_directories(cydist_shim_span_test PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/cydist"
)
set_source_files_properties(cydist/cydist_shim.cpp PROPERTIES LANGUAGE CXX)
apply_release_flags(cydist_shim_span_test)
add_test(NAME cydist_shim_span_test COMMAND cydist_shim_span_test)
if(DISTRIBUTIONS_ENABLE_SIMD)
    add_dist_test(simd_tier_repro_test tests/cpp/simd_tier_repro_test.cpp)
endif()
if(DISTRIBUTIONS_ENABLE_AVX512)
    add_dist_test(simd_avx512_config_test tests/cpp/simd_avx512_config_test.cpp)
endif()

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
