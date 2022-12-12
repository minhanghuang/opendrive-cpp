#include "opendrive-cpp/parser/map_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status MapXmlParser::Parse(const tinyxml2::XMLElement* xml_map,
                                      element::Map* ele_map) {
  xml_map_ = xml_map;
  ele_map_ = ele_map;
  if (!xml_map_ || !ele_map_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseHeaderEle().ParseRoadEle();
  return status();
}

MapXmlParser& MapXmlParser::ParseHeaderEle() {
  if (!IsValid()) return *this;
  element::Header ele_header;
  HeaderXmlParser header_parser;
  const tinyxml2::XMLElement* xml_header =
      xml_map_->FirstChildElement("header");
  auto status = header_parser.Parse(xml_header, &ele_header);
  CheckStatus(status);
  ele_map_->header = ele_header;
  return *this;
}

MapXmlParser& MapXmlParser::ParseRoadEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_ele_road =
      xml_map_->FirstChildElement("road");
  RoadXmlParser road_parser;
  Status status{ErrorCode::OK, "ok"};
  while (curr_ele_road) {
    element::Road ele_road;
    status = road_parser.Parse(curr_ele_road, &ele_road);
    CheckStatus(status);
    ele_map_->roads.emplace_back(ele_road);
    curr_ele_road = common::XmlNextSiblingElement(curr_ele_road);
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
