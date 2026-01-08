cmake_minimum_required(VERSION 3.10)
project(LOBMatcher VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(lob_engine src/main.cpp src/OrderBook.cpp)