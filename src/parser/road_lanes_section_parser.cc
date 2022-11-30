#include "opendrive-cpp/parser/road_lanes_section_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadLanesSectionXmlParser::Parse(
    const tinyxml2::XMLElement* xml_section,
    element::LaneSection* ele_section) {
  xml_section_ = xml_section;
  ele_section_ = ele_section;
  if (!xml_section_ || !ele_section_) {
    set_status(ErrorCode::XML_LANES_SECTION_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseAttributes().ParseLanesEle();
  return status();
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryDoubleAttribute(xml_section_, "s", ele_section_->s0);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLanesEle() {
  if (!IsValid()) return *this;
  std::vector<std::string> lanes_type{"left", "center", "right"};
  const tinyxml2::XMLElement* curr_xml_lanes;
  const tinyxml2::XMLElement* curr_xml_lane;
  for (const auto& type : lanes_type) {
    curr_xml_lanes = xml_section_->FirstChildElement(type.c_str());
    if (!curr_xml_lanes) {
      continue;
    }
    curr_xml_lane = curr_xml_lanes->FirstChildElement("lane");
    while (curr_xml_lane) {
      element::Lane lane;
      this->ParseLaneEle(curr_xml_lane, lane);
      if ("left" == type) {
        ele_section_->left.lanes.emplace_back(lane);
      } else if ("center" == type) {
        ele_section_->center.lanes.emplace_back(lane);
      } else if ("right" == type) {
        ele_section_->right.lanes.emplace_back(lane);
      }
      curr_xml_lane = common::XmlNextSiblingElement(curr_xml_lane);
    }
  }
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  ParseLaneAttributes(xml_lane, ele_lane)
      .ParseLaneWidthEle(xml_lane, ele_lane)
      .ParseLaneLinkEle(xml_lane, ele_lane)
      .ParseLaneBorderEle(xml_lane, ele_lane)
      .ParseLaneRoadMarkEle(xml_lane, ele_lane);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneAttributes(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  std::string lane_type;
  std::string lane_level;
  common::XmlQueryIntAttribute(xml_lane, "id", ele_lane.attributes.id);
  common::XmlQueryStringAttribute(xml_lane, "type", lane_type);
  common::XmlQueryStringAttribute(xml_lane, "level", lane_level);
  common::XmlQueryEnumAttribute(
      xml_lane, "type", ele_lane.attributes.type,
      std::map<std::string, LaneType>{
          std::make_pair("shoulder", LaneType::SHOULDER),
          std::make_pair("border", LaneType::BORDER),
          std::make_pair("driving", LaneType::DRIVING),
          std::make_pair("stop", LaneType::STOP),
          std::make_pair("none", LaneType::NONE),
          std::make_pair("restricted", LaneType::RESTRICTED),
          std::make_pair("parking", LaneType::PARKING),
          std::make_pair("median", LaneType::MEDIAN),
          std::make_pair("biking", LaneType::BIKING),
          std::make_pair("sidewalk", LaneType::SIDEWALK),
          std::make_pair("curb", LaneType::CURB),
          std::make_pair("exit", LaneType::EXIT),
          std::make_pair("entry", LaneType::ENTRY),
          std::make_pair("onRamp", LaneType::ONRAMP),
          std::make_pair("offRamp", LaneType::OFFRAMP),
          std::make_pair("connectingRamp", LaneType::CONNECTINGRAMP),
          std::make_pair("bidirectional", LaneType::BIDIRECTIONAL),
          std::make_pair("special1", LaneType::SPECIAL1),
          std::make_pair("special2", LaneType::SPECIAL2),
          std::make_pair("special3", LaneType::SPECIAL3),
          std::make_pair("roadWorks", LaneType::ROADWORKS),
          std::make_pair("tram", LaneType::TRAM),
          std::make_pair("bus", LaneType::BUS),
          std::make_pair("taxi", LaneType::TAXI),
          std::make_pair("HOV", LaneType::HOV),
          std::make_pair("mwyEntry", LaneType::MWYENTRY),
          std::make_pair("mwyExit", LaneType::MWYEXIT)});
  common::XmlQueryEnumAttribute(xml_lane, "level", ele_lane.attributes.level,
                                std::map<std::string, Boolean>{
                                    std::make_pair("false", Boolean::FALSE),
                                    std::make_pair("true", Boolean::TRUE),
                                });
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneLinkEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* xml_lane_link =
      xml_lane->FirstChildElement("link");
  if (xml_lane_link) {
    const tinyxml2::XMLElement* xml_lane_link_predecessor =
        xml_lane_link->FirstChildElement("predecessor");
    if (xml_lane_link_predecessor) {
      common::XmlQueryIntAttribute(xml_lane_link_predecessor, "id",
                                   ele_lane.link.predecessor);
    }
    const tinyxml2::XMLElement* xml_lane_link_successor =
        xml_lane_link->FirstChildElement("successor");
    if (xml_lane_link_successor) {
      common::XmlQueryIntAttribute(xml_lane_link_successor, "id",
                                   ele_lane.link.successor);
    }
  }

  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneWidthEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_width =
      xml_lane->FirstChildElement("width");
  while (curr_xml_width) {
    element::LaneWidth lane_width;
    common::XmlQueryDoubleAttribute(curr_xml_width, "sOffset", lane_width.s);
    common::XmlQueryDoubleAttribute(curr_xml_width, "a", lane_width.a);
    common::XmlQueryDoubleAttribute(curr_xml_width, "b", lane_width.b);
    common::XmlQueryDoubleAttribute(curr_xml_width, "c", lane_width.c);
    common::XmlQueryDoubleAttribute(curr_xml_width, "d", lane_width.d);
    ele_lane.widths.insert(lane_width);
    curr_xml_width = common::XmlNextSiblingElement(curr_xml_width);
  }
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneBorderEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_border =
      xml_lane->FirstChildElement("border");
  while (curr_xml_border) {
    element::LaneBorder lane_border;
    common::XmlQueryDoubleAttribute(curr_xml_border, "sOffset", lane_border.s);
    common::XmlQueryDoubleAttribute(curr_xml_border, "a", lane_border.a);
    common::XmlQueryDoubleAttribute(curr_xml_border, "b", lane_border.b);
    common::XmlQueryDoubleAttribute(curr_xml_border, "c", lane_border.c);
    common::XmlQueryDoubleAttribute(curr_xml_border, "d", lane_border.d);
    ele_lane.borders.insert(lane_border);
    curr_xml_border = common::XmlNextSiblingElement(curr_xml_border);
  }
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneRoadMarkEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_mark =
      xml_lane->FirstChildElement("roadMark");
  while (curr_xml_mark) {
    element::RoadMark road_mark;
    std::string mark_type;
    std::string mark_color;
    std::string mark_weight;
    std::string mark_lane_change;
    common::XmlQueryDoubleAttribute(curr_xml_mark, "sOffset", road_mark.s);
    common::XmlQueryDoubleAttribute(curr_xml_mark, "width", road_mark.width);
    common::XmlQueryDoubleAttribute(curr_xml_mark, "height", road_mark.height);
    common::XmlQueryStringAttribute(curr_xml_mark, "material",
                                    road_mark.material);
    common::XmlQueryEnumAttribute(
        curr_xml_mark, "type", road_mark.type,
        std::map<std::string, RoadMarkType>{
            std::make_pair("none", RoadMarkType::NONE),
            std::make_pair("solid", RoadMarkType::SOLID),
            std::make_pair("broken", RoadMarkType::BROKEN),
            std::make_pair("solid solid", RoadMarkType::SOLIDSOLID),
            std::make_pair("solid broken", RoadMarkType::SOLIDBROKEN),
            std::make_pair("broken solid", RoadMarkType::BROKENSOLID),
            std::make_pair("broken broken", RoadMarkType::BROKENBROKEN),
            std::make_pair("botts dots", RoadMarkType::BOTTSDOTS),
            std::make_pair("grass", RoadMarkType::GRASS),
            std::make_pair("curb", RoadMarkType::CURB),
            std::make_pair("custom", RoadMarkType::CUSTOM),
            std::make_pair("edge", RoadMarkType::EDGE)});
    common::XmlQueryEnumAttribute(
        curr_xml_mark, "color", road_mark.color,
        std::map<std::string, RoadMarkColor>{
            std::make_pair("standard", RoadMarkColor::STANDARD),
            std::make_pair("blue", RoadMarkColor::BLUE),
            std::make_pair("green", RoadMarkColor::GREEN),
            std::make_pair("red", RoadMarkColor::RED),
            std::make_pair("white", RoadMarkColor::WHITE),
            std::make_pair("yellow", RoadMarkColor::YELLOW),
            std::make_pair("orange", RoadMarkColor::ORANGE)});
    common::XmlQueryEnumAttribute(
        curr_xml_mark, "weight", road_mark.weigth,
        std::map<std::string, RoadMarkWeight>{
            std::make_pair("bold", RoadMarkWeight::BOLD),
            std::make_pair("standard", RoadMarkWeight::STANDARD)});
    common::XmlQueryEnumAttribute(
        curr_xml_mark, "laneChange", road_mark.lane_change,
        std::map<std::string, RoadMarkLaneChange>{
            std::make_pair("increase", RoadMarkLaneChange::INCREASE),
            std::make_pair("decrease", RoadMarkLaneChange::DECREASE),
            std::make_pair("both", RoadMarkLaneChange::BOTH),
            std::make_pair("none", RoadMarkLaneChange::NONE)});
    ele_lane.road_marks.emplace_back(road_mark);
    curr_xml_mark = common::XmlNextSiblingElement(curr_xml_mark);
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
