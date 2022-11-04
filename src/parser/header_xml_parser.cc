#include "opendrive-cpp/parser/header_xml_parser.h"

#include "opendrive-cpp/common/common.hpp"

namespace opendrive {
namespace parser {

opendrive::Status HeaderXmlParser::Parse(const tinyxml2::XMLElement* header_ele,
                                         core::Header::Ptr header_ptr) {
  header_ele_ = header_ele;
  header_ptr_ = header_ptr;
  Init();
  this->ParseAttributes();
  return status();
}

void HeaderXmlParser::Init() {
  if (!header_ele_ || !header_ptr_) {
    set_status(ErrorCode::XML_HEADER_ELEMENT_ERROR, "Input is null.");
  }
}

HeaderXmlParser& HeaderXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
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
  XmlQueryStringAttribute(header_ele_, "revMajor", rev_major);
  XmlQueryStringAttribute(header_ele_, "revMinor", rev_minor);
  XmlQueryStringAttribute(header_ele_, "name", name);
  XmlQueryStringAttribute(header_ele_, "version", version);
  XmlQueryStringAttribute(header_ele_, "vendor", vendor);
  XmlQueryStringAttribute(header_ele_, "date", date);
  XmlQueryDoubleAttribute(header_ele_, "north", north);
  XmlQueryDoubleAttribute(header_ele_, "south", south);
  XmlQueryDoubleAttribute(header_ele_, "west", west);
  XmlQueryDoubleAttribute(header_ele_, "east", east);
  header_ptr_->rev_major = rev_major;
  header_ptr_->rev_minor = rev_minor;
  header_ptr_->name = name;
  header_ptr_->version = version;
  header_ptr_->vendor = vendor;
  header_ptr_->date = date;
  header_ptr_->north = north;
  header_ptr_->south = south;
  header_ptr_->west = west;
  header_ptr_->east = east;
  return *this;
}

}  // namespace parser
}  // namespace opendrive
