#include "opendrive-cpp/parser/road_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadXmlParser::Parse(const tinyxml2::XMLElement* xml_road,
                                       element::Road* ele_road) {
  xml_road_ = xml_road;
  ele_road_ = ele_road;
  if (!xml_road_ || !ele_road_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
  }
  ParseAttributes()
      .ParseLinkEle()
      .ParseTypeEle()
      .ParsePlanViewEle()
      .ParseLanesEle()
      .CheckLanesEle();
  return status();
}

RoadXmlParser& RoadXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  std::string rule;
  common::XmlQueryStringAttribute(xml_road_, "name",
                                  ele_road_->attributes.name);
  common::XmlQueryEnumAttribute(
      xml_road_, "rule", ele_road_->attributes.rule,
      std::map<std::string, RoadRule>{std::make_pair("LHT", RoadRule::LHT),
                                      std::make_pair("RHT", RoadRule::RHT)});
  common::XmlQueryDoubleAttribute(xml_road_, "length",
                                  ele_road_->attributes.length);
  common::XmlQueryIntAttribute(xml_road_, "id", ele_road_->attributes.id);
  common::XmlQueryIntAttribute(xml_road_, "junction",
                               ele_road_->attributes.junction);
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLinkEle() {
  if (!IsValid()) return *this;
  auto xml_link = xml_road_->FirstChildElement("link");
  auto status = road_link_parser_.Parse(xml_link, &ele_road_->link);
  update_status(status);
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseTypeEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_type =
      xml_road_->FirstChildElement("type");
  Status status{ErrorCode::OK, "ok"};
  while (curr_xml_type) {
    element::RoadTypeInfo ele_road_type;
    status = road_type_parser_.Parse(curr_xml_type, &ele_road_type);
    update_status(status);
    ele_road_->type_info.emplace_back(ele_road_type);
    curr_xml_type = common::XmlNextSiblingElement(curr_xml_type);
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParsePlanViewEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* xml_planview =
      xml_road_->FirstChildElement("planView");
  auto status =
      road_planview_parser_.Parse(xml_planview, &ele_road_->plan_view);
  update_status(status);
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLanesEle() {
  if (!IsValid()) return *this;
  element::Lanes lanes;
  const tinyxml2::XMLElement* lanes_ele = xml_road_->FirstChildElement("lanes");
  LanesXmlParser lanes_parser;
  auto status = lanes_parser.Parse(lanes_ele, &lanes);
  update_status(status);
  ele_road_->lanes = lanes;
  return *this;
}

RoadXmlParser& RoadXmlParser::CheckLanesEle() {
  if (!IsValid()) return *this;
  if (ele_road_->plan_view.geometrys.size() !=
      ele_road_->lanes.lane_sections.size()) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
               "Check <geometry> Element Count Exception.");
    return *this;
  }
  for (const auto& section : ele_road_->lanes.lane_sections) {
    if (section.center.lanes.empty()) {
      set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
                 "Check <laneSection> Element Center Line Exception.");
      return *this;
    }
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
