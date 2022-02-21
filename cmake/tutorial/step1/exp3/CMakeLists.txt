cmake_minimum_required(VERSION 3.10)

# set the project name，可以包括版本号
project(Tutorial VERSION 1.0)

# 自动生成TutorialConfig.h文件，生成在cmake的build目录下
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 设置可以使用c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Tutorial tutorial.cpp)

# 由于TutorialConfig.h生成在build目录下，也即binary的目录
# 要引用他，需要把路径加到头文件的搜索路径上
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

