#include "opendrive-cpp/parser/lanes_xml_parser.h"

#include <vector>

#include "opendrive-cpp/common/common.hpp"

namespace opendrive {
namespace parser {

opendrive::Status LanesXmlParser::Parse(const tinyxml2::XMLElement* lanes_ele,
                                        core::Lanes::Ptr lanes_ptr) {
  lanes_ele_ = lanes_ele;
  lanes_ptr_ = lanes_ptr;
  Init();
  this->ParseLaneOffsetEle().ParseLaneSectionEle();

  /// 2. lane section

  return opendrive::Status(opendrive::ErrorCode::OK, "ok");
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
    core::LaneOffset lane_offset;
    double lane_offset_s;
    double lane_offset_a;
    double lane_offset_b;
    double lane_offset_c;
    double lane_offset_d;
    common::XmlQueryDoubleAttribute(lane_offset_ele, "s", lane_offset_s);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "a", lane_offset_a);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "b", lane_offset_b);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "c", lane_offset_c);
    common::XmlQueryDoubleAttribute(lane_offset_ele, "d", lane_offset_d);
    lane_offset.s = lane_offset_s;
    lane_offset.a = lane_offset_a;
    lane_offset.b = lane_offset_b;
    lane_offset.c = lane_offset_c;
    lane_offset.d = lane_offset_d;
    lanes_ptr_->lane_offsets.emplace_back(lane_offset);
    lane_offset_ele = common::XmlNextSiblingElement(lane_offset_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneSectionEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* lane_section_ele =
      lanes_ele_->FirstChildElement("laneSection");
  while (lane_section_ele) {
    core::LaneSection lane_section;
    double lane_section_s;
    common::XmlQueryDoubleAttribute(lane_section_ele, "s", lane_section_s);
    lane_section.s = lane_section_s;
    ParseLaneSectionLanesEle(lane_section_ele, lane_section);
    lane_section_ele = common::XmlNextSiblingElement(lane_section_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneSectionLanesEle(
    const tinyxml2::XMLElement* sections_ele, core::LaneSection& lane_section) {
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
      core::Lane lane;
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
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;

  this->ParseLaneAttribute(lane_ele, lane)
      .ParseLaneLinkEle(lane_ele, lane)
      .ParseLaneWidthEle(lane_ele, lane)
      .ParseLaneRoadMarkEle(lane_ele, lane);

  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneAttribute(
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;
  double lane_id;
  std::string lane_type;
  std::string lane_level;
  common::XmlQueryDoubleAttribute(lane_ele, "id", lane_id);
  common::XmlQueryStringAttribute(lane_ele, "type", lane_type);
  common::XmlQueryStringAttribute(lane_ele, "level", lane_type);
  lane.attributes.id = lane_id;

  if (common::StrEquals("shoulder", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::SHOULDER;
  } else if (common::StrEquals("border", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::BORDER;
  } else if (common::StrEquals("driving", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::DRIVING;
  } else if (common::StrEquals("stop", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::STOP;
  } else if (common::StrEquals("none", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::NONE;
  } else if (common::StrEquals("restricted", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::RESTRICTED;
  } else if (common::StrEquals("parking", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::PARKING;
  } else if (common::StrEquals("median", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::MEDIAN;
  } else if (common::StrEquals("biking", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::BIKING;
  } else if (common::StrEquals("sidewalk", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::SIDEWALK;
  } else if (common::StrEquals("curb", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::CURB;
  } else if (common::StrEquals("exit", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::EXIT;
  } else if (common::StrEquals("entry", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::ENTRY;
  } else if (common::StrEquals("onRamp", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::ONRAMP;
  } else if (common::StrEquals("offRamp", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::OFFRAMP;
  } else if (common::StrEquals("connectingRamp", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::CONNECTINGRAMP;
  } else if (common::StrEquals("bidirectional", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::BIDIRECTIONAL;
  } else if (common::StrEquals("special1", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::SPECIAL1;
  } else if (common::StrEquals("special2", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::SPECIAL2;
  } else if (common::StrEquals("special3", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::SPECIAL3;
  } else if (common::StrEquals("roadWorks", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::ROADWORKS;
  } else if (common::StrEquals("tram", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::TRAM;
  } else if (common::StrEquals("rail", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::RAIL;
  } else if (common::StrEquals("bus", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::BUS;
  } else if (common::StrEquals("taxi", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::TAXI;
  } else if (common::StrEquals("HOV", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::HOV;
  } else if (common::StrEquals("mwyEntry", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::MWYENTRY;
  } else if (common::StrEquals("mwyExit", lane_type)) {
    lane.attributes.type = core::LaneAttributes::Type::MWYEXIT;
  }

  if (common::StrEquals(lane_level, "false")) {
    lane.attributes.level = core::Boolean::FALSE;
  } else if (common::StrEquals(lane_type, "true")) {
    lane.attributes.level = core::Boolean::TRUE;
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneLinkEle(
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* lane_link_ele =
      lane_ele->FirstChildElement("link");
  if (lane_link_ele) {
    const tinyxml2::XMLElement* lane_link_predecessor_node =
        lane_link_ele->FirstChildElement("predecessor");
    if (lane_link_predecessor_node) {
      double predecessor_id;
      common::XmlQueryDoubleAttribute(lane_link_predecessor_node, "id",
                                      predecessor_id);
      lane.link.predecessor = predecessor_id;
    }
    const tinyxml2::XMLElement* lane_link_successor_node =
        lane_link_ele->FirstChildElement("successor");
    if (lane_link_successor_node) {
      double successor_id;
      common::XmlQueryDoubleAttribute(lane_link_successor_node, "id",
                                      successor_id);
      lane.link.successor = successor_id;
    }
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneWidthEle(
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* lane_width_ele =
      lane_ele->FirstChildElement("width");
  while (lane_width_ele) {
    core::LaneWidth lane_width;
    double s;
    double a;
    double b;
    double c;
    double d;
    common::XmlQueryDoubleAttribute(lane_width_ele, "sOffset", s);
    common::XmlQueryDoubleAttribute(lane_width_ele, "a", a);
    common::XmlQueryDoubleAttribute(lane_width_ele, "b", b);
    common::XmlQueryDoubleAttribute(lane_width_ele, "c", c);
    common::XmlQueryDoubleAttribute(lane_width_ele, "d", d);
    lane_width.s = s;
    lane_width.a = a;
    lane_width.b = b;
    lane_width.c = c;
    lane_width.d = d;
    lane.widths.emplace_back(lane_width);
    lane_width_ele = common::XmlNextSiblingElement(lane_width_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneBorderEle(
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_lane_border_ele =
      lane_ele->FirstChildElement("border");
  while (curr_lane_border_ele) {
    core::LaneBorder lane_border;
    double s;
    double a;
    double b;
    double c;
    double d;
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "sOffset", s);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "a", a);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "b", b);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "c", c);
    common::XmlQueryDoubleAttribute(curr_lane_border_ele, "d", d);
    lane_border.s = s;
    lane_border.a = a;
    lane_border.b = b;
    lane_border.c = c;
    lane_border.d = d;
    lane.borders.emplace_back(lane_border);
    curr_lane_border_ele = common::XmlNextSiblingElement(curr_lane_border_ele);
  }
  return *this;
}

LanesXmlParser& LanesXmlParser::ParseLaneRoadMarkEle(
    const tinyxml2::XMLElement* lane_ele, core::Lane& lane) {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_lane_mark_ele =
      lane_ele->FirstChildElement("roadMark");
  while (curr_lane_mark_ele) {
    core::RoadMark road_mark;
    double mark_s;
    std::string mark_type;
    std::string mark_color;
    std::string mark_weight;
    double mark_width;
    double mark_height;
    std::string mark_material;
    std::string mark_lane_change;
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "sOffset", mark_s);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "type", mark_type);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "color", mark_color);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "weight", mark_weight);
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "width", mark_width);
    common::XmlQueryDoubleAttribute(curr_lane_mark_ele, "height", mark_height);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "material",
                                    mark_material);
    common::XmlQueryStringAttribute(curr_lane_mark_ele, "laneChange",
                                    mark_lane_change);
    road_mark.s = mark_s;
    if (common::StrEquals("none", mark_type)) {
      road_mark.type = core::RoadMark::Type::NONE;
    } else if (common::StrEquals("solid", mark_type)) {
      road_mark.type = core::RoadMark::Type::SOLID;
    } else if (common::StrEquals("broken", mark_type)) {
      road_mark.type = core::RoadMark::Type::BROKEN;
    } else if (common::StrEquals("solid solid", mark_type)) {
      road_mark.type = core::RoadMark::Type::SOLIDSOLID;
    } else if (common::StrEquals("solid broken", mark_type)) {
      road_mark.type = core::RoadMark::Type::SOLIDBROKEN;
    } else if (common::StrEquals("broken solid", mark_type)) {
      road_mark.type = core::RoadMark::Type::BROKENSOLID;
    } else if (common::StrEquals("broken broken", mark_type)) {
      road_mark.type = core::RoadMark::Type::BROKENBROKEN;
    } else if (common::StrEquals("botts dots", mark_type)) {
      road_mark.type = core::RoadMark::Type::BOTTSDOTS;
    } else if (common::StrEquals("grass", mark_type)) {
      road_mark.type = core::RoadMark::Type::GRASS;
    } else if (common::StrEquals("curb", mark_type)) {
      road_mark.type = core::RoadMark::Type::CURB;
    } else if (common::StrEquals("custom", mark_type)) {
      road_mark.type = core::RoadMark::Type::CUSTOM;
    } else if (common::StrEquals("edge", mark_type)) {
      road_mark.type = core::RoadMark::Type::EDGE;
    }

    if (common::StrEquals("standard", mark_color)) {
      road_mark.color = core::RoadMark::Color::STANDARD;
    } else if (common::StrEquals("blue", mark_color)) {
      road_mark.color = core::RoadMark::Color::BLUE;
    } else if (common::StrEquals("green", mark_color)) {
      road_mark.color = core::RoadMark::Color::GREEN;
    } else if (common::StrEquals("red", mark_color)) {
      road_mark.color = core::RoadMark::Color::RED;
    } else if (common::StrEquals("white", mark_color)) {
      road_mark.color = core::RoadMark::Color::WHITE;
    } else if (common::StrEquals("yellow", mark_color)) {
      road_mark.color = core::RoadMark::Color::YELLOW;
    } else if (common::StrEquals("orange", mark_color)) {
      road_mark.color = core::RoadMark::Color::ORANGE;
    }

    if (common::StrEquals("bold", mark_weight)) {
      road_mark.weigth = core::RoadMark::Weight::BOLD;
    } else if (common::StrEquals("standard", mark_weight)) {
      road_mark.weigth = core::RoadMark::Weight::STANDARD;
    }

    if (common::StrEquals("increase", mark_lane_change)) {
      road_mark.lane_change = core::RoadMark::LaneChange::INCREASE;
    } else if (common::StrEquals("decrease", mark_lane_change)) {
      road_mark.lane_change = core::RoadMark::LaneChange::DECREASE;
    } else if (common::StrEquals("both", mark_lane_change)) {
      road_mark.lane_change = core::RoadMark::LaneChange::BOTH;
    } else if (common::StrEquals("none", mark_lane_change)) {
      road_mark.lane_change = core::RoadMark::LaneChange::NONE;
    }
    road_mark.width = mark_width;
    road_mark.height = mark_height;
    road_mark.material = mark_material;
    lane.road_marks.emplace_back(road_mark);
    curr_lane_mark_ele = common::XmlNextSiblingElement(curr_lane_mark_ele);
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
