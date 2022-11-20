#include "opendrive-cpp/parser/header_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status HeaderXmlParser::Parse(const tinyxml2::XMLElement* ele_header,
                                         element::Header* header) {
  ele_header_ = ele_header;
  header_ = header;
  if (!ele_header_ || !header_) {
    set_status(ErrorCode::XML_HEADER_ELEMENT_ERROR, "Input is null.");
  }
  this->ParseAttributes();
  return status();
}

HeaderXmlParser& HeaderXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryStringAttribute(ele_header_, "revMajor", header_->rev_major);
  common::XmlQueryStringAttribute(ele_header_, "revMinor", header_->rev_minor);
  common::XmlQueryStringAttribute(ele_header_, "name", header_->name);
  common::XmlQueryStringAttribute(ele_header_, "version", header_->version);
  common::XmlQueryStringAttribute(ele_header_, "vendor", header_->vendor);
  common::XmlQueryStringAttribute(ele_header_, "date", header_->date);
  common::XmlQueryDoubleAttribute(ele_header_, "north", header_->north);
  common::XmlQueryDoubleAttribute(ele_header_, "south", header_->south);
  common::XmlQueryDoubleAttribute(ele_header_, "west", header_->west);
  common::XmlQueryDoubleAttribute(ele_header_, "east", header_->east);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
