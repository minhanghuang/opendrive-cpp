#include "opendrive-cpp/parser/road_lanes_parser.h"

#include "opendrive-cpp/common/common.hpp"

namespace opendrive {
namespace parser {

opendrive::Status RoadLanesXmlParser::Parse(
    const tinyxml2::XMLElement* xml_lanes, element::Lanes* lanes) {
  xml_lanes_ = xml_lanes;
  ele_lanes_ = lanes;
  if (!xml_lanes_ || !ele_lanes_) {
    set_status(ErrorCode::XML_LANES_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseLaneOffsetEle().ParseLaneSectionEle();
  return status();
}

RoadLanesXmlParser& RoadLanesXmlParser::ParseLaneOffsetEle() {
  if (!IsValid()) return *this;
  auto xml_offset = xml_lanes_->FirstChildElement("laneOffset");
  Status status{ErrorCode::OK, "ok"};
  while (xml_offset) {
    element::LaneOffset offset;
    status = offset_parser_.Parse(xml_offset, &offset);
    CheckStatus(status);
    ele_lanes_->lane_offsets.emplace_back(offset);
    xml_offset = common::XmlNextSiblingElement(xml_offset);
  }
  common::VectorSortPoloy3(ele_lanes_->lane_offsets);
  return *this;
}

RoadLanesXmlParser& RoadLanesXmlParser::ParseLaneSectionEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_section =
      xml_lanes_->FirstChildElement("laneSection");
  Status status{ErrorCode::OK, "ok"};
  while (curr_xml_section) {
    element::LaneSection lane_section;
    common::XmlQueryDoubleAttribute(curr_xml_section, "s", lane_section.s0);
    common::XmlQueryDoubleAttribute(curr_xml_section, "s", lane_section.s1);
    status = section_parser_.Parse(curr_xml_section, &lane_section);
    CheckStatus(status);
    ele_lanes_->lane_sections.emplace_back(lane_section);
    curr_xml_section = common::XmlNextSiblingElement(curr_xml_section);
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
