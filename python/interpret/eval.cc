namespace bp = boost::python;
std::string const python_code = "2 * 21";

try {
  auto py_result = bp::eval(python_code);
  auto cpp_result = bp::extract<int>(py_result);
} catch (bp::error_already_set) {
   // improved error handling.
}
