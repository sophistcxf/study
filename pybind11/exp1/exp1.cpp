#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

int add2(int i, int j) {
    return i + j;
}

int add3(int i = 1, int j = 2) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");

    m.def("add2", &add2, "A function which adds two numbers",
      py::arg("i"), py::arg("j"));

    // pybind11无法从add3中直接提取默认参数，需要显示的指定
    m.def("add3", &add3, "A function which adds two numbers",
      py::arg("i") = 1, py::arg("j") = 2);

    // 添加属性the_answer，值为42
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    // 添加属性what，值为"World"
    m.attr("what") = world;
}
