#include "opendrive-cpp/parser/section_parser.h"

#include "opendrive-cpp/common/choices.h"

namespace opendrive {
namespace parser {

RoadLanesSectionXmlParser::RoadLanesSectionXmlParser(const std::string& version)
    : XmlParser(version) {}

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
  common::XmlQueryDoubleAttribute(xml_section_, "s", ele_section_->s1);
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
  /// sort left lanes
  std::sort(ele_section_->left.lanes.begin(), ele_section_->left.lanes.end(),
            [](const element::Lane& l1, const element::Lane& l2) {
              return l1.attributes.id() < l2.attributes.id();
            });
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  ParseLaneAttributes(xml_lane, ele_lane)
      .ParseLaneWidthEle(xml_lane, ele_lane)
      .ParseLaneLinkEle(xml_lane, ele_lane)
      .ParseLaneBorderEle(xml_lane, ele_lane)
      .ParseLaneRoadMarkEle(xml_lane, ele_lane)
      .ParseLaneSpeedEle(xml_lane, ele_lane);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneAttributes(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  std::string lane_type;
  std::string lane_level;
  common::XmlQueryIntAttribute(xml_lane, "id",
                               ele_lane.attributes.mutable_id());
  common::XmlQueryStringAttribute(xml_lane, "type", lane_type);
  common::XmlQueryStringAttribute(xml_lane, "level", lane_level);
  common::XmlQueryEnumAttribute(
      xml_lane, "type", ele_lane.attributes.mutable_type(), LANE_TYPE_CHOICES);
  common::XmlQueryEnumAttribute(
      xml_lane, "level", ele_lane.attributes.mutable_level(), BOOLEAN_CHOICES);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneLinkEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* xml_lane_link =
      xml_lane->FirstChildElement("link");
  if (xml_lane_link) {
    element::Id id = 0;
    const tinyxml2::XMLElement* curr_xml_lane_link_predecessor =
        xml_lane_link->FirstChildElement("predecessor");
    while (curr_xml_lane_link_predecessor) {
      id = 0;
      common::XmlQueryIntAttribute(curr_xml_lane_link_predecessor, "id", id);
      ele_lane.link.predecessors.emplace_back(id);
      curr_xml_lane_link_predecessor =
          common::XmlNextSiblingElement(curr_xml_lane_link_predecessor);
    }
    const tinyxml2::XMLElement* curr_xml_lane_link_successor =
        xml_lane_link->FirstChildElement("successor");
    while (curr_xml_lane_link_successor) {
      id = 0;
      common::XmlQueryIntAttribute(curr_xml_lane_link_successor, "id", id);
      ele_lane.link.successors.emplace_back(id);
      curr_xml_lane_link_successor =
          common::XmlNextSiblingElement(curr_xml_lane_link_successor);
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
    common::XmlQueryDoubleAttribute(curr_xml_width, "sOffset",
                                    lane_width.mutable_s());
    common::XmlQueryDoubleAttribute(curr_xml_width, "a",
                                    lane_width.mutable_a());
    common::XmlQueryDoubleAttribute(curr_xml_width, "b",
                                    lane_width.mutable_b());
    common::XmlQueryDoubleAttribute(curr_xml_width, "c",
                                    lane_width.mutable_c());
    common::XmlQueryDoubleAttribute(curr_xml_width, "d",
                                    lane_width.mutable_d());
    ele_lane.widths.emplace_back(lane_width);
    curr_xml_width = common::XmlNextSiblingElement(curr_xml_width);
  }
  common::VectorSortPoloy3(ele_lane.widths);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneBorderEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_border =
      xml_lane->FirstChildElement("border");
  while (curr_xml_border) {
    element::LaneBorder lane_border;
    common::XmlQueryDoubleAttribute(curr_xml_border, "sOffset",
                                    lane_border.mutable_s());
    common::XmlQueryDoubleAttribute(curr_xml_border, "a",
                                    lane_border.mutable_a());
    common::XmlQueryDoubleAttribute(curr_xml_border, "b",
                                    lane_border.mutable_b());
    common::XmlQueryDoubleAttribute(curr_xml_border, "c",
                                    lane_border.mutable_c());
    common::XmlQueryDoubleAttribute(curr_xml_border, "d",
                                    lane_border.mutable_d());
    ele_lane.borders.emplace_back(lane_border);
    curr_xml_border = common::XmlNextSiblingElement(curr_xml_border);
  }
  common::VectorSortPoloy3(ele_lane.borders);
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
    common::XmlQueryDoubleAttribute(curr_xml_mark, "sOffset",
                                    road_mark.mutable_s());
    common::XmlQueryDoubleAttribute(curr_xml_mark, "width",
                                    road_mark.mutable_width());
    common::XmlQueryDoubleAttribute(curr_xml_mark, "height",
                                    road_mark.mutable_height());
    common::XmlQueryStringAttribute(curr_xml_mark, "material",
                                    road_mark.mutable_material());
    common::XmlQueryEnumAttribute(
        curr_xml_mark, "type", road_mark.mutable_type(), ROADMARK_TYPE_CHOICES);
    common::XmlQueryEnumAttribute(curr_xml_mark, "color",
                                  road_mark.mutable_color(),
                                  ROAD_MARK_COLOR_CHOICES);
    common::XmlQueryEnumAttribute(curr_xml_mark, "weight",
                                  road_mark.mutable_weigth(),
                                  ROAD_MARK_WEIGHT_CHOICES);
    common::XmlQueryEnumAttribute(curr_xml_mark, "laneChange",
                                  road_mark.mutable_lane_change(),
                                  ROAD_MARK_LANE_CHANGE_CHOICES);
    ele_lane.road_marks.emplace_back(road_mark);
    curr_xml_mark = common::XmlNextSiblingElement(curr_xml_mark);
  }
  common::VectorSortPoloy3(ele_lane.road_marks);
  return *this;
}

RoadLanesSectionXmlParser& RoadLanesSectionXmlParser::ParseLaneSpeedEle(
    const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_speed =
      xml_lane->FirstChildElement("speed");
  while (curr_xml_speed) {
    element::LaneSpeed lane_speed;
    common::XmlQueryDoubleAttribute(curr_xml_speed, "sOffset", lane_speed.s);
    common::XmlQueryFloatAttribute(curr_xml_speed, "max", lane_speed.max);
    common::XmlQueryEnumAttribute(curr_xml_speed, "unit", lane_speed.unit,
                                  SPEEDUNIT_CHOICES);
    ele_lane.max_speeds.emplace_back(lane_speed);
    curr_xml_speed = common::XmlNextSiblingElement(curr_xml_speed);
  }
  common::VectorSortPoloy3(ele_lane.max_speeds);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
