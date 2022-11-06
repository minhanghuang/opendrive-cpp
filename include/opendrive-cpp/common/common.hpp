#ifndef OPENDRIVE_CPP_COMMON_HPP_
#define OPENDRIVE_CPP_COMMON_HPP_

#include <tinyxml2.h>

#include <climits>
#include <iostream>
#include <set>
#include <string>
#include <type_traits>

namespace opendrive {
namespace common {

static void Assert(bool r, const std::string& msg = "fault") {
  if (!r) {
    std::cout << "assert msg: " << msg << std::endl;
    assert(false);
  }
}

static tinyxml2::XMLError XmlQueryBoolAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name, bool& value,
    bool enable_exit = false) {
  tinyxml2::XMLError ret = xml_node->QueryBoolAttribute(name.c_str(), &value);
  if (tinyxml2::XML_SUCCESS != ret && enable_exit) {
    Assert(false, "xml query int attribute fault.");
  }
  return ret;
}

static tinyxml2::XMLError XmlQueryStringAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    std::string& value, bool enable_exit = false) {
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

static tinyxml2::XMLError XmlQueryIntAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name, int& value,
    bool enable_exit = false) {
  tinyxml2::XMLError ret = xml_node->QueryIntAttribute(name.c_str(), &value);
  if (tinyxml2::XML_SUCCESS != ret && enable_exit) {
    Assert(false, "xml query int attribute fault.");
  }
  return ret;
}

static tinyxml2::XMLError XmlQueryFloatAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name, float& value,
    bool enable_exit = false) {
  tinyxml2::XMLError ret = xml_node->QueryFloatAttribute(name.c_str(), &value);
  if (tinyxml2::XML_SUCCESS != ret && enable_exit) {
    Assert(false, "xml query float attribute fault.");
  }
  return ret;
}

static tinyxml2::XMLError XmlQueryDoubleAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name,
    double& value, bool enable_exit = false) {
  tinyxml2::XMLError ret = xml_node->QueryDoubleAttribute(name.c_str(), &value);
  if (tinyxml2::XML_SUCCESS != ret && enable_exit) {
    Assert(false, "xml query double attribute fault.");
  }
  return ret;
}

static const tinyxml2::XMLElement* XmlNextSiblingElement(
    const tinyxml2::XMLElement* element) {
  return element->NextSiblingElement(element->Name());
}

static std::string StrToUpper(const std::string& s) {
  std::string ret = s;
  std::transform(s.begin(), s.end(), ret.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return ret;
}

static std::string StrToLower(const std::string& s) {
  std::string ret = s;
  std::transform(s.begin(), s.end(), ret.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return ret;
}

static bool StrEquals(const std::string& a, const std::string& b) {
  if (a.size() == b.size() && StrToUpper(a) == StrToUpper(b)) {
    return true;
  }
  return false;
}

}  // namespace common
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_COMMON_HPP_
