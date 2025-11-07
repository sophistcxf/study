cmake_minimum_required(VERSION 3.5)
project(eigen_example)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64")


# Find required packages
set(gflags_DIR /opt/homebrew/Cellar/gflags/2.2.2/lib/cmake/gflags)
find_package(gflags REQUIRED)
set(glog_DIR /opt/homebrew/Cellar/glog/0.6.0/lib/cmake/glog)
find_package(glog REQUIRED)
set(benchmark_DIR /opt/homebrew/Cellar/google-benchmark/1.9.4/lib/cmake/benchmark)
find_package(benchmark REQUIRED)

# Add eigen from local source
add_subdirectory(eigen-5.0.0)
set(BUILD_BENCHMARKS OFF CACHE BOOL "Disable benchmark building")

# Add executable
add_executable(eigen_example example.cpp)

# Link libraries
target_link_libraries(eigen_example
  eigen
  gflags
  glog::glog
  benchmark::benchmark
)

# Include directories
target_include_directories(eigen_example PRIVATE
  ${GLOG_INCLUDE_DIRS}
  "eigen-5.0.0"
)
