cmake_minimum_required(VERSION 3.20) # Bumped version for better Python support
project(LOBMatcher VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)

# --- CRITICAL FIX: Find Python explicitly before PyBind11 ---
# This forces CMake to find the headers (Python.h) correctly
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# --- 1. Fetch PyBind11 ---
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# --- 2. Define the Core Library ---
add_library(lob_lib src/OrderBook.cpp)

# --- 3. Build the Python Module ---
pybind11_add_module(lob_python src/PyBindWrapper.cpp)
target_link_libraries(lob_python PRIVATE lob_lib)

# --- 4. Google Benchmark ---
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(lob_benchmark benchmarks/main_bench.cpp)
target_link_libraries(lob_benchmark lob_lib benchmark::benchmark)