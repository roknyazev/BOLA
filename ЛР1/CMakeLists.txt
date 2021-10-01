cmake_minimum_required(VERSION 3.20)
project(lw1)

set(CMAKE_CXX_STANDARD 14)

add_executable(client client/main.cpp ARINC429.h MIL1553.h Timer.h)
add_executable(server server/main.cpp ARINC429.h MIL1553.h)
