#include <pybind11/embed.h>

namespace py = pybind11;

#define PRINT_MODULE(name) \
  printf(#name" ref = %d\n", (int)(name.ptr()->ob_refcnt));

py::module create_module() {
  return py::module("good");
}

int main() {
  py::scoped_interpreter guard{};
  py::module hello("hello");
  py::module world("world");
  PRINT_MODULE(hello);
  PRINT_MODULE(world);

  py::module b(hello);
  PRINT_MODULE(world);

  py::module c(create_module());
  PRINT_MODULE(c);

  return 0;
}
