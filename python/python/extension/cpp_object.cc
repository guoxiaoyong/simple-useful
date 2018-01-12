#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet {
  Pet(const std::string& name) : name_(name) {}
  void SetName(const std::string& name) { name_ = name; }
  const std::string& GetName() const { return name_; }

  std::string name_;
};

PYBIND11_MODULE(cpp_object, m) {
  py::class_<Pet>(m, "Pet")
      .def(py::init<const std::string&>())
      .def("set_name", &Pet::SetName)
      .def("get_name", &Pet::GetName)
      .def("__repr__",
           [] (const Pet& pet) {
             return "<cpp_object.Pet named '" + pet.GetName() + "'>";
           }
      )
      .def_readwrite("name", &Pet::name_)
      .def_property("Name", &Pet::GetName, &Pet::SetName);
}
