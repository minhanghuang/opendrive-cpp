#ifndef OPENDRIVE_CPP_COMMON_HPP_
#define OPENDRIVE_CPP_COMMON_HPP_

#include <tinyxml2.h>

#include <climits>
#include <iostream>
#include <set>
#include <string>
#include <type_traits>

namespace opendrive {

static void Assert(bool r, const std::string& msg = "fault") {
  if (!r) {
    std::cout << "assert msg: " << msg << std::endl;
    assert(false);
  }
}

static tinyxml2::XMLError XmlQueryStringAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    std::string& value, bool enable_exit = false) {
  value = xml_node->Attribute(name.c_str());
  if (value.empty()) {
    if (enable_exit) {
      Assert(false, "xml query attribute fault.");
    }
    return tinyxml2::XML_NO_ATTRIBUTE;
  }
  return tinyxml2::XML_SUCCESS;
}

static tinyxml2::XMLError XmlQueryIntAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name, int& value,
    bool enable_exit = false) {
  std::string val = xml_node->Attribute(name.c_str());
  if (val.empty()) {
    if (enable_exit) {
      Assert(false, "xml query attribute fault.");
    }
    value = 0;
    return tinyxml2::XML_NO_ATTRIBUTE;
  }
  value = std::stoi(val);
  return tinyxml2::XML_SUCCESS;
}

static tinyxml2::XMLError XmlQueryDoubleAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    double& value, bool enable_exit = false) {
  std::string val = xml_node->Attribute(name.c_str());
  if (val.empty()) {
    if (enable_exit) {
      Assert(false, "xml query attribute fault.");
    }
    value = 0.;
    return tinyxml2::XML_NO_ATTRIBUTE;
  }
  value = std::stod(val);
  return tinyxml2::XML_SUCCESS;
}

static const tinyxml2::XMLElement* XmlNextSiblingElement(
    const tinyxml2::XMLElement* element) {
  element = element->NextSiblingElement(element->Name());
  return element;
}

static std::string StrToUpper(const std::string& s) {
  std::string ret = s;
  std::transform(s.begin(), s.end(), ret.begin(),
                 [](auto t) { return std::toupper(t); });
  return ret;
}

static std::string StrToLower(const std::string& s) {
  std::string ret = s;
  std::transform(s.begin(), s.end(), ret.begin(),
                 [](auto t) { return std::tolower(t); });
  return ret;
}

static bool StrEquals(const std::string& a, const std::string& b) {
  if (a.size() == b.size() && StrToUpper(a) == StrToUpper(b)) {
    return true;
  }
  return false;
}

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_COMMON_HPP_
