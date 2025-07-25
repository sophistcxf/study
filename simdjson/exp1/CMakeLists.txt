cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(exp1)

add_subdirectory(simdjson-master)

add_executable(exp1 main.cpp)

target_include_directories(exp1 PRIVATE simdjson-master/include)
target_link_directories(exp1 PRIVATE simdjson-master)
target_link_libraries(exp1 PRIVATE libsimdjson.a)