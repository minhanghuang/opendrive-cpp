#include "opendrive-cpp/parser/map_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status MapXmlParser::Parse(const tinyxml2::XMLElement* xml_map,
                                      element::Map::Ptr map_ptr) {
  xml_map_ = xml_map;
  map_ptr_ = map_ptr;
  if (!xml_map_ || !map_ptr_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
  }
  this->ParseHeaderEle().ParseRoadEle();
  return status();
}

MapXmlParser& MapXmlParser::ParseHeaderEle() {
  if (!IsValid()) return *this;
  element::Header ele_header;
  map_ptr_->header = ele_header;
  HeaderXmlParser header_parser;
  const tinyxml2::XMLElement* xml_header =
      xml_map_->FirstChildElement("header");
  header_parser.Parse(xml_header, &ele_header);
  return *this;
}

MapXmlParser& MapXmlParser::ParseRoadEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_ele_road =
      xml_map_->FirstChildElement("road");
  RoadXmlParser road_parser;
  while (curr_ele_road) {
    element::Road ele_road;
    road_parser.Parse(curr_ele_road, &ele_road);
    map_ptr_->roads.emplace_back(ele_road);
    common::XmlNextSiblingElement(curr_ele_road);
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
