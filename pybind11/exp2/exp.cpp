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

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

class Pet2 {
public:
    Pet2(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

class Pet3 {
public:
    Pet3(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

// 重载。成员函数 set 被重载。
struct Pet4 {
    Pet4(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    const std::string &getName() const { return name; }

    std::string name;
    int age;
};

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

    py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName);


    // Pet2添加了方法 __repr__，这样在 print(p)时可以输出readable信息
    py::class_<Pet2>(m, "Pet2")
    .def(py::init<const std::string &>())
    .def("setName", &Pet2::setName)
    .def("getName", &Pet2::getName)
    .def("__repr__",
        [](const Pet2 &a) {
            return "<example.Pet named '" + a.name + "'>";
        }
    );

    py::class_<Pet3>(m, "Pet3", py::dynamic_attr())
    .def(py::init<const std::string &>())
    .def("setName", &Pet3::setName)
    .def("getName", &Pet3::getName)
    .def("__repr__",
        [](const Pet3 &a) {
            return "<example.Pet named '" + a.name + "'>";
        }
    );

    py::class_<Pet4>(m, "Pet4")
    .def(py::init<const std::string &, int>())
    .def("set", static_cast<void (Pet4::*)(int)>(&Pet4::set), "Set the pet's age")
    .def("set", static_cast<void (Pet4::*)(const std::string &)>(&Pet4::set), "Set the pet's name")
    .def("getName", &Pet4::getName);
}
