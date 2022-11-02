#include "opendrive-cpp/parser/header_xml_parser.h"

#include "opendrive-cpp/common/common.hpp"

namespace opendrive {
namespace parser {

opendrive::Status HeaderXmlParser::Parse(
    const tinyxml2::XMLElement* header_node, core::Header::Ptr header_ptr) {
  if (!header_node) {
    return opendrive::Status(opendrive::ErrorCode::XML_HEADER_ELEMENT_ERROR,
                             "Xml Header Is nullptr.");
  }
  std::string rev_major;
  std::string rev_minor;
  std::string name;
  std::string version;
  std::string date;
  std::string vendor;
  double north = 0.0;
  double south = 0.0;
  double west = 0.0;
  double east = 0.0;
  int checker = tinyxml2::XMLError::XML_SUCCESS;

  checker += XmlQueryStringAttribute(header_node, "revMajor", rev_major);
  checker += XmlQueryStringAttribute(header_node, "revMinor", rev_minor);
  checker += XmlQueryStringAttribute(header_node, "name", name);
  checker += XmlQueryStringAttribute(header_node, "version", version);
  checker += XmlQueryStringAttribute(header_node, "vendor", vendor);
  checker += XmlQueryStringAttribute(header_node, "date", date);
  checker += XmlQueryDoubleAttribute(header_node, "north", north);
  checker += XmlQueryDoubleAttribute(header_node, "south", south);
  checker += XmlQueryDoubleAttribute(header_node, "west", west);
  checker += XmlQueryDoubleAttribute(header_node, "east", east);

  if (tinyxml2::XMLError::XML_SUCCESS != checker) {
    return opendrive::Status(opendrive::ErrorCode::XML_HEADER_ELEMENT_ERROR,
                             "Parser Header Exception");
  }

  header_ptr->rev_major = rev_major;
  header_ptr->rev_minor = rev_minor;
  header_ptr->name = name;
  header_ptr->version = version;
  header_ptr->vendor = vendor;
  header_ptr->date = date;
  header_ptr->north = north;
  header_ptr->south = south;
  header_ptr->west = west;
  header_ptr->east = east;
  return opendrive::Status(opendrive::ErrorCode::OK, "ok");
}

}  // namespace parser
}  // namespace opendrive
