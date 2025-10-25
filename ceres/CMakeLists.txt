cmake_minimum_required(VERSION 3.5)
project(ceres_example)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64")


# Find required packages
set(Eigen3_DIR /opt/homebrew/Cellar/eigen/3.4.1/share/eigen3/cmake)
find_package(Eigen3 3.4.1 REQUIRED)
set(gflags_DIR /opt/homebrew/Cellar/gflags/2.2.2/lib/cmake/gflags)
find_package(gflags REQUIRED)
set(glog_DIR /opt/homebrew/Cellar/glog/0.6.0/lib/cmake/glog)
find_package(glog REQUIRED)
set(benchmark_DIR /opt/homebrew/Cellar/google-benchmark/1.9.4/lib/cmake/benchmark)
find_package(benchmark REQUIRED)

# Add Ceres from local source
add_subdirectory(ceres-solver-2.2.0)
set(BUILD_BENCHMARKS OFF CACHE BOOL "Disable benchmark building")

# Add executable
add_executable(ceres_example example.cpp)

# Link libraries
target_link_libraries(ceres_example
  ceres
  gflags
  glog::glog
  benchmark::benchmark
)

# Include directories
target_include_directories(ceres_example PRIVATE
  ${EIGEN3_INCLUDE_DIR}
  ${GLOG_INCLUDE_DIRS}
  ceres-solver-2.2.0/include
)
