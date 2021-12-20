cmake_minimum_required(VERSION 3.20)
project(lw1)

set(CMAKE_CXX_STANDARD 14)

add_executable(sns sns/main.cpp ARINC429.h MIL1553.h Timer.h transform.h transform.cpp sns/Sns.cpp sns/Sns.h)
add_executable(ins ins/main.cpp ARINC429.h MIL1553.h Timer.h transform.h ins/Ins.cpp ins/Ins.h transform.cpp)
add_executable(server server/main.cpp ARINC429.h MIL1553.h transform.h transform.cpp)
