#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
  printf(R"({"i": %d, "j": %d})""\n", i, j);
  return i + j;
}

PYBIND11_MODULE(simple, m) {
  m.doc() = "pybind11 example plugin";
  m.def(
      "add", &add, "A function which adds two numbers", py::arg("i") = 3,
      py::arg("j") = 4);
  m.attr("number") = 123;
  py::object hello = py::cast("hello world");
  m.attr("hello") = hello;
}
