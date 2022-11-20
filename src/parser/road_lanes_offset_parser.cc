#include "opendrive-cpp/parser/road_lanes_offset_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadLanesOffsetXmlParser::Parse(
    const tinyxml2::XMLElement* xml_offset, element::LaneOffset* ele_offset) {
  xml_offset_ = xml_offset;
  ele_offset_ = ele_offset;
  if (!xml_offset_ || !ele_offset_) {
    set_status(ErrorCode::XML_LANES_OFFSET_ELEMENT_ERROR, "Input is null.");
  }
  ParseAttributes();
  return status();
}

RoadLanesOffsetXmlParser& RoadLanesOffsetXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryDoubleAttribute(xml_offset_, "s", ele_offset_->s);
  common::XmlQueryDoubleAttribute(xml_offset_, "a", ele_offset_->a);
  common::XmlQueryDoubleAttribute(xml_offset_, "b", ele_offset_->b);
  common::XmlQueryDoubleAttribute(xml_offset_, "c", ele_offset_->c);
  common::XmlQueryDoubleAttribute(xml_offset_, "d", ele_offset_->d);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
