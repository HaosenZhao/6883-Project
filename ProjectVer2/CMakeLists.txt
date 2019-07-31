cmake_minimum_required(VERSION 3.13)
project(Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

add_executable(Project main.cpp Stock.h StockDic.cpp StockDic.h Matrix.cpp Matrix.h gnuplot.cpp gnuplot.h Calculation.h Calculation.cpp)
target_link_libraries(Project curl)