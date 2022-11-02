#ifndef OPENDRIVE_CPP_COMMON_HPP_
#define OPENDRIVE_CPP_COMMON_HPP_

#include <tinyxml2.h>

#include <iostream>
#include <string>

namespace opendrive {

static void Assert(bool r, const std::string& msg = "fault") {
  if (!r) {
    std::cout << "assert msg: " << msg << std::endl;
    assert(false);
  }
}

static tinyxml2::XMLError XmlQueryStringAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    std::string& value, bool enable_exit = true) {
  const char* val = xml_node->Attribute(name.c_str());
  if (val == nullptr) {
    if (enable_exit) {
      Assert(false, "xml query attribute fault.");
    }
    return tinyxml2::XML_NO_ATTRIBUTE;
  }
  value = val;
  return tinyxml2::XML_SUCCESS;
}

static tinyxml2::XMLError XmlQueryDoubleAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    double& value, bool enable_exit = true) {
  const char* val = xml_node->Attribute(name.c_str());
  if (val == nullptr) {
    if (enable_exit) {
      Assert(false, "xml query attribute fault.");
    }
    return tinyxml2::XML_NO_ATTRIBUTE;
  }
  value = std::stod(val);
  return tinyxml2::XML_SUCCESS;
}

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_COMMON_HPP_
