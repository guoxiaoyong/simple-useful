#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;

static const string str_node_name[] = {"id", "name", "accout"};

struct xmlStageDetail {
  string id;
  string name;
  string accout;
};

int load(const char *filename, vector<xmlStageDetail> *m_xml_detail) {
  xmlStageDetail xml_stage_detail;
  using boost::property_tree::ptree;
  ptree pt;
  ptree root;
  try {
    read_xml(filename, pt);
    root = pt.get_child("root");
  } catch (std::exception &e) {
    return -1;
  }

  for (ptree::iterator itr = root.begin(); itr != root.end(); itr++) {
    ptree strage = itr->second;
    for (int i = 0; i < 3; i++) {
      ptree pt;
      string str_value;
      try {
        pt = strage.get_child(str_node_name[i]);
        str_value = pt.data();
      } catch (std::exception &e) {
        return -1;
      }
      switch (i) {
      case 0:
        xml_stage_detail.id = str_value;
        break;
      case 1:
        xml_stage_detail.name = str_value;

        break;
      case 2:
        xml_stage_detail.accout = str_value;

        break;
      }
    }
    m_xml_detail->push_back(xml_stage_detail);
  }
  return 0;
}

int main() {

  vector<xmlStageDetail> m_xml_detail;

  try {

    load("sample.xml", &m_xml_detail);
    std::cout << "Success\n";

  } catch (std::exception &e) {

    std::cout << "Error: " << e.what() << "\n";
    return -1;
  }

  return 0;
}
