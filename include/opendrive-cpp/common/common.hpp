#ifndef OPENDRIVE_CPP_COMMON_HPP_
#define OPENDRIVE_CPP_COMMON_HPP_

#include <tinyxml2.h>

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

namespace opendrive {
namespace common {

/**
 * @brief 获取第一个小于目标值的元素
 *
 * @tparam T1 element::Poloy3
 * @tparam T2 number
 * @param items ascending sequence
 * @param target target value
 * @return sequence index or -1
 */
template <typename T1, typename T2>
static int GetLeftValuePoloy3(const std::vector<T1>& items, T2 target) {
  if (items.empty() || target < items.at(0).s) {
    return -1;
  }
  for (size_t i = items.size() - 1; i >= 0; i--) {
    if (items.at(i).s <= target) {
      return i;
    }
  }
  return -1;
}

template <typename T1, typename T2>
static int GetLeftPtrPoloy3(const std::vector<T1>& items, T2 target) {
  if (items.empty() || target < items.at(0)->s) {
    return -1;
  }
  for (size_t i = items.size() - 1; i >= 0; i--) {
    if (items.at(i)->s <= target) {
      return i;
    }
  }
  return -1;
}

static bool FileExists(const std::string& path) {
  if (FILE* f = fopen(path.c_str(), "r")) {
    fclose(f);
    return true;
  }
  return false;
}

static std::vector<std::string> Split(const std::string& str,
                                      const std::string& delimiter) {
  std::vector<std::string> res;
  size_t pos = 0;
  size_t index = 0;
  std::string token;
  while ((pos = str.find(delimiter, index)) != std::string::npos) {
    token = str.substr(index, pos - index);
    res.emplace_back(token);
    index = pos + delimiter.length();
  }
  token = str.substr(index);
  res.emplace_back(token);
  return res;
}

static void Assert(bool r, const std::string& msg = "fault") {
  if (!r) {
    std::cout << "assert msg: " << msg << std::endl;
    assert(false);
  }
}

template <typename T>
static T Clamp(T a, T min = T(0), T max = T(1)) {
  return std::min(std::max(a, min), max);
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

template <typename T>
static tinyxml2::XMLError XmlQueryEnumAttribute(
    const tinyxml2::XMLElement* xml_node, const std::string& name, T& value,
    const std::map<std::string, T>& items, bool enable_exit = false) {
  std::string var;
  tinyxml2::XMLError ret =
      XmlQueryStringAttribute(xml_node, name, var, enable_exit);
  if (tinyxml2::XMLError::XML_SUCCESS != ret) {
    if (enable_exit) {
      Assert(false, "xml query enum attribute fault.");
    }
    return ret;
  }
  ret = tinyxml2::XMLError::XML_ERROR_PARSING_TEXT;
  for (const auto& item : items) {
    if (StrEquals(item.first, var)) {
      value = item.second;
      ret = tinyxml2::XMLError::XML_SUCCESS;
      break;
    }
  }
  return ret;
}

static const tinyxml2::XMLElement* XmlNextSiblingElement(
    const tinyxml2::XMLElement* element) {
  return element->NextSiblingElement(element->Name());
}

}  // namespace common
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_COMMON_HPP_
