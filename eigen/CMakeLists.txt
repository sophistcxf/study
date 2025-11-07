cmake_minimum_required(VERSION 3.5)
project(eigen_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64")

add_executable(eigen_example example.cpp)

target_include_directories(eigen_example PRIVATE
  ${GLOG_INCLUDE_DIRS}
  "eigen-5.0.0"
)
