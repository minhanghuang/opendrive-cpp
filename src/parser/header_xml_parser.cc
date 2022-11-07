#include "opendrive-cpp/parser/header_xml_parser.h"

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/parser/parser.h"

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
  common::XmlQueryStringAttribute(header_ele_, "revMajor",
                                  header_ptr_->rev_major);
  common::XmlQueryStringAttribute(header_ele_, "revMinor",
                                  header_ptr_->rev_minor);
  common::XmlQueryStringAttribute(header_ele_, "name", header_ptr_->name);
  common::XmlQueryStringAttribute(header_ele_, "version", header_ptr_->version);
  common::XmlQueryStringAttribute(header_ele_, "vendor", header_ptr_->vendor);
  common::XmlQueryStringAttribute(header_ele_, "date", header_ptr_->date);
  common::XmlQueryDoubleAttribute(header_ele_, "north", header_ptr_->north);
  common::XmlQueryDoubleAttribute(header_ele_, "south", header_ptr_->south);
  common::XmlQueryDoubleAttribute(header_ele_, "west", header_ptr_->west);
  common::XmlQueryDoubleAttribute(header_ele_, "east", header_ptr_->east);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
