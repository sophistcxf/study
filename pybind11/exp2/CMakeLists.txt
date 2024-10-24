cmake_minimum_required(VERSION 3.14)

project(example)

# pybind11里，默认是使用python3.8，设置使用3.10
set(PythonLibsNew_FIND_VERSION "3.10")

add_subdirectory(pybind11)

# 使用pybind11/tools/pybind11NewTools.cmake
pybind11_add_module(example exp.cpp)
