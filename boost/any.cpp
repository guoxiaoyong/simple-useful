#include <algorithm>
#include <boost/any.hpp>
#include <iostream>
#include <list>
#include <typeinfo>

typedef std::list<boost::any> many;

void append_int(many &values, int value) {
  boost::any to_append = value;
  values.push_back(to_append);
}

void append_string(many &values, const std::string &value) {
  values.push_back(value);
}

void append_char_ptr(many &values, const char *value) {
  values.push_back(value);
}

void append_any(many &values, const boost::any &value) {
  values.push_back(value);
}

void append_nothing(many &values) { values.push_back(boost::any()); }

bool is_empty(const boost::any &operand) { return operand.empty(); }

bool is_int(const boost::any &operand) { return operand.type() == typeid(int); }

bool is_char_ptr(const boost::any &operand) {
  try {
    boost::any_cast<const char *>(operand);
    return true;
  } catch (const boost::bad_any_cast &badcast_e) {
    return false;
  }
}

bool is_string(const boost::any &operand) {
  return boost::any_cast<std::string>(&operand);
}

void count_all(const many &values, std::ostream &out) {
  out << "#empty==" << std::count_if(values.begin(), values.end(), is_empty)
      << std::endl;
  out << "#int == " << std::count_if(values.begin(), values.end(), is_int)
      << std::endl;
  out << "#char* ==" << std::count_if(values.begin(), values.end(), is_char_ptr)
      << std::endl;
  out << "#string ==" << std::count_if(values.begin(), values.end(), is_string)
      << std::endl;
}

int main(int argc, char *argv[]) {

  many values;
  append_int(values, 1);
  append_int(values, 2);
  count_all(values, std::cout);
  return 0;
}
