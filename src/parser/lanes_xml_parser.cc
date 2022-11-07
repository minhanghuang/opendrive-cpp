#include "opendrive-cpp/parser/lanes_xml_parser.h"

#include <utility>
#include <vector>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/core/types.hpp"

namespace opendrive {
namespace parser {

opendrive::Status LanesXmlParser::Parse(const tinyxml2::XMLElement* lanes_ele,
                                        base::Lanes::Ptr lanes_ptr) {
  lanes_ele_ = lanes_ele;
  lanes_ptr_ = lanes_ptr;
  Init();
  this->ParseLaneOffsetEle().ParseLaneSectionEle();
  return status();
}

void LanesXmlParser::Init() {
  if (!lanes_ele_ || !lanes_ptr_) {
    set_status(ErrorCode::XML_LANES_ELEMENT_ERROR, "Input is null.");
  }
}

LanesXmlParser& LanesXmlParser::ParseLaneOffsetEle() {
  if (!IsValid()) return *this;
  auto lane_offset_ele = lanes_ele_->FirstChildElement("laneOffset");
  while (lane_offset_ele) {
    base::LaneOffset lane_offset;
    common::XmlQueryDoubleAttribute(lane_offset_ele, "s", lane_offset.s);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "a", lane_offset.a);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "b", lane_offset.b);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "c", lane_offset.c);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "d", lane_offset.d);
    lanes_ptr_->lane_offsets.emplace_back(lane_offset);
    lane_offset_ele = common::XmlNextSiblingElement(lane_offset_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneSectionEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_lane_section_ele =
      lanes_ele_->FirstChildElement("laneSection");
  while (curr_lane_section_ele) {
    base::LaneSection lane_section;
    common::XmlQueryDoubleAttribute(curr_lane_section_ele, "s", lane_section.s);
    ParseLaneSectionLanesEle(curr_lane_section_ele, lane_section);
    lanes_ptr_->lane_sections.emplace_back(lane_section);
    curr_lane_section_ele =
        common::XmlNextSiblingElement(curr_lane_section_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneSectionLanesEle(
    const tinyxml2::XMLElement* sections_ele, base::LaneSection& lane_section) {
  if (!IsValid()) return *this;

  std::vector<std::string> section_type{"left", "center", "right"};
  for (const auto& type : section_type) {
    const tinyxml2::XMLElement* curr_section_ele =
        sections_ele->FirstChildElement(type.c_str());
    if (!curr_section_ele) {
      continue;
    }
    const tinyxml2::XMLElement* lane_ele =
        curr_section_ele->FirstChildElement("lane");
    while (lane_ele) {
      base::Lane lane;
      this->ParseLaneEle(lane_ele, lane);
      if ("left" == type) {
        lane_section.left.lanes.emplace_back(lane);
      } else if ("center" == type) {
        lane_section.center.lanes.emplace_back(lane);
      } else if ("right" == type) {
        lane_section.right.lanes.emplace_back(lane);
      }
      lane_ele = common::XmlNextSiblingElement(lane_ele);
    }
  }

  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneEle(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;

  this->ParseLaneAttribute(lane_ele, lane)
      .ParseLaneLinkEle(lane_ele, lane)
      .ParseLaneWidthEle(lane_ele, lane)
      .ParseLaneBorderEle(lane_ele, lane)
      .ParseLaneRoadMarkEle(lane_ele, lane);

  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneAttribute(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;
  std::string lane_type;
  std::string lane_level;
  common::XmlQueryIntAttribute(lane_ele, "id", lane.attributes.id);
  common::XmlQueryStringAttribute(lane_ele, "type", lane_type);
  common::XmlQueryStringAttribute(lane_ele, "level", lane_level);
  common::XmlQueryEnumAttribute(
      lane_ele, "type", lane.attributes.type,
      std::map<std::string, base::LaneAttributes::Type>{
          std::make_pair("shoulder", base::LaneAttributes::Type::SHOULDER),
          std::make_pair("border", base::LaneAttributes::Type::BORDER),
          std::make_pair("driving", base::LaneAttributes::Type::DRIVING),
          std::make_pair("stop", base::LaneAttributes::Type::STOP),
          std::make_pair("none", base::LaneAttributes::Type::NONE),
          std::make_pair("restricted", base::LaneAttributes::Type::RESTRICTED),
          std::make_pair("parking", base::LaneAttributes::Type::PARKING),
          std::make_pair("median", base::LaneAttributes::Type::MEDIAN),
          std::make_pair("biking", base::LaneAttributes::Type::BIKING),
          std::make_pair("sidewalk", base::LaneAttributes::Type::SIDEWALK),
          std::make_pair("curb", base::LaneAttributes::Type::CURB),
          std::make_pair("exit", base::LaneAttributes::Type::EXIT),
          std::make_pair("entry", base::LaneAttributes::Type::ENTRY),
          std::make_pair("onRamp", base::LaneAttributes::Type::ONRAMP),
          std::make_pair("offRamp", base::LaneAttributes::Type::OFFRAMP),
          std::make_pair("connectingRamp",
                         base::LaneAttributes::Type::CONNECTINGRAMP),
          std::make_pair("bidirectional",
                         base::LaneAttributes::Type::BIDIRECTIONAL),
          std::make_pair("special1", base::LaneAttributes::Type::SPECIAL1),
          std::make_pair("special2", base::LaneAttributes::Type::SPECIAL2),
          std::make_pair("special3", base::LaneAttributes::Type::SPECIAL3),
          std::make_pair("roadWorks", base::LaneAttributes::Type::ROADWORKS),
          std::make_pair("tram", base::LaneAttributes::Type::TRAM),
          std::make_pair("bus", base::LaneAttributes::Type::BUS),
          std::make_pair("taxi", base::LaneAttributes::Type::TAXI),
          std::make_pair("HOV", base::LaneAttributes::Type::HOV),
          std::make_pair("mwyEntry", base::LaneAttributes::Type::MWYENTRY),
          std::make_pair("mwyExit", base::LaneAttributes::Type::MWYEXIT)});
  common::XmlQueryEnumAttribute(
      lane_ele, "level", lane.attributes.level,
      std::map<std::string, base::Boolean>{
          std::make_pair("false", base::Boolean::FALSE),
          std::make_pair("true", base::Boolean::TRUE),
      });
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneLinkEle(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* lane_link_ele =
      lane_ele->FirstChildElement("link");
  if (lane_link_ele) {
    const tinyxml2::XMLElement* lane_link_predecessor_node =
        lane_link_ele->FirstChildElement("predecessor");
    if (lane_link_predecessor_node) {
      common::XmlQueryIntAttribute(lane_link_predecessor_node, "id",
                                   lane.link.predecessor);
    }
    const tinyxml2::XMLElement* lane_link_successor_node =
        lane_link_ele->FirstChildElement("successor");
    if (lane_link_successor_node) {
      common::XmlQueryIntAttribute(lane_link_successor_node, "id",
                                   lane.link.successor);
    }
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneWidthEle(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* lane_width_ele =
      lane_ele->FirstChildElement("width");
  while (lane_width_ele) {
    base::LaneWidth lane_width;
    common::XmlQueryDoubleAttribute(lane_width_ele, "sOffset", lane_width.s);
    common::XmlQueryDoubleAttribute(lane_width_ele, "a", lane_width.a);
    common::XmlQueryDoubleAttribute(lane_width_ele, "b", lane_width.b);
    common::XmlQueryDoubleAttribute(lane_width_ele, "c", lane_width.c);
    common::XmlQueryDoubleAttribute(lane_width_ele, "d", lane_width.d);
    lane.widths.emplace_back(lane_width);
    lane_width_ele = common::XmlNextSiblingElement(lane_width_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneBorderEle(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_lane_border_ele =
      lane_ele->FirstChildElement("border");
  while (curr_lane_border_ele) {
    base::LaneBorder lane_border;
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "sOffset",
                                    lane_border.s);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "a", lane_border.a);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "b", lane_border.b);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "c", lane_border.c);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "d", lane_border.d);
    lane.borders.emplace_back(lane_border);
    curr_lane_border_ele = common::XmlNextSiblingElement(curr_lane_border_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneRoadMarkEle(
    const tinyxml2::XMLElement* lane_ele, base::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_lane_mark_ele =
      lane_ele->FirstChildElement("roadMark");
  while (curr_lane_mark_ele) {
    base::RoadMark road_mark;
    std::string mark_type;
    std::string mark_color;
    std::string mark_weight;
    std::string mark_lane_change;
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "sOffset", road_mark.s);
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "width",
                                    road_mark.width);
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "height",
                                    road_mark.height);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "material",
                                    road_mark.material);
    common::XmlQueryEnumAttribute(
        curr_lane_mark_ele, "type", road_mark.type,
        std::map<std::string, base::RoadMark::Type>{
            std::make_pair("none", base::RoadMark::Type::NONE),
            std::make_pair("solid", base::RoadMark::Type::SOLID),
            std::make_pair("broken", base::RoadMark::Type::BROKEN),
            std::make_pair("solid solid", base::RoadMark::Type::SOLIDSOLID),
            std::make_pair("solid broken", base::RoadMark::Type::SOLIDBROKEN),
            std::make_pair("broken solid", base::RoadMark::Type::BROKENSOLID),
            std::make_pair("broken broken", base::RoadMark::Type::BROKENBROKEN),
            std::make_pair("botts dots", base::RoadMark::Type::BOTTSDOTS),
            std::make_pair("grass", base::RoadMark::Type::GRASS),
            std::make_pair("curb", base::RoadMark::Type::CURB),
            std::make_pair("custom", base::RoadMark::Type::CUSTOM),
            std::make_pair("edge", base::RoadMark::Type::EDGE)});
    common::XmlQueryEnumAttribute(
        curr_lane_mark_ele, "color", road_mark.color,
        std::map<std::string, base::RoadMark::Color>{
            std::make_pair("standard", base::RoadMark::Color::STANDARD),
            std::make_pair("blue", base::RoadMark::Color::BLUE),
            std::make_pair("green", base::RoadMark::Color::GREEN),
            std::make_pair("red", base::RoadMark::Color::RED),
            std::make_pair("white", base::RoadMark::Color::WHITE),
            std::make_pair("yellow", base::RoadMark::Color::YELLOW),
            std::make_pair("orange", base::RoadMark::Color::ORANGE)});
    common::XmlQueryEnumAttribute(
        curr_lane_mark_ele, "weight", road_mark.weigth,
        std::map<std::string, base::RoadMark::Weight>{
            std::make_pair("bold", base::RoadMark::Weight::BOLD),
            std::make_pair("standard", base::RoadMark::Weight::STANDARD)});
    common::XmlQueryEnumAttribute(
        curr_lane_mark_ele, "laneChange", road_mark.lane_change,
        std::map<std::string, base::RoadMark::LaneChange>{
            std::make_pair("increase", base::RoadMark::LaneChange::INCREASE),
            std::make_pair("decrease", base::RoadMark::LaneChange::DECREASE),
            std::make_pair("both", base::RoadMark::LaneChange::BOTH),
            std::make_pair("none", base::RoadMark::LaneChange::NONE)});
    lane.road_marks.emplace_back(road_mark);
    curr_lane_mark_ele = common::XmlNextSiblingElement(curr_lane_mark_ele);
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
