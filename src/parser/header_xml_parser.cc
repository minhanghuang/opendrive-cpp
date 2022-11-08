#include "opendrive-cpp/parser/header_xml_parser.h"

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/parser/parser.h"

namespace opendrive {
namespace parser {

opendrive::Status HeaderXmlParser::Parse(const tinyxml2::XMLElement* header_ele,
                                         base::Header* header) {
  header_ele_ = header_ele;
  header_ = header;
  Init();
  this->ParseAttributes();
  return status();
}

void HeaderXmlParser::Init() {
  if (!header_ele_ || !header_) {
    set_status(ErrorCode::XML_HEADER_ELEMENT_ERROR, "Input is null.");
  }
}

HeaderXmlParser& HeaderXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryStringAttribute(header_ele_, "revMajor", header_->rev_major);
  common::XmlQueryStringAttribute(header_ele_, "revMinor", header_->rev_minor);
  common::XmlQueryStringAttribute(header_ele_, "name", header_->name);
  common::XmlQueryStringAttribute(header_ele_, "version", header_->version);
  common::XmlQueryStringAttribute(header_ele_, "vendor", header_->vendor);
  common::XmlQueryStringAttribute(header_ele_, "date", header_->date);
  common::XmlQueryDoubleAttribute(header_ele_, "north", header_->north);
  common::XmlQueryDoubleAttribute(header_ele_, "south", header_->south);
  common::XmlQueryDoubleAttribute(header_ele_, "west", header_->west);
  common::XmlQueryDoubleAttribute(header_ele_, "east", header_->east);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
