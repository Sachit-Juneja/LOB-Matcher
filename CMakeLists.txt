cmake_minimum_required(VERSION 3.14)
project(LOBMatcher VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)

# 1. Define the Core Library
add_library(lob_lib src/OrderBook.cpp)

# 2. The Main Executable
add_executable(lob_engine src/main.cpp)
target_link_libraries(lob_engine lob_lib)

# 3. Fetch Google Benchmark (FIXED CONFIGURATION)
include(FetchContent)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppress benchmark testing" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Suppress benchmark install" FORCE)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
FetchContent_MakeAvailable(googlebenchmark)

# 4. The Benchmark Executable
add_executable(lob_benchmark benchmarks/main_bench.cpp)
target_link_libraries(lob_benchmark lob_lib benchmark::benchmark)