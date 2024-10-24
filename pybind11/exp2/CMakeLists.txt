cmake_minimum_required(VERSION 3.14)

project(example)

set(PythonLibsNew_FIND_VERSION "3.10.14")

add_subdirectory(pybind11)

pybind11_add_module(example exp.cpp)
