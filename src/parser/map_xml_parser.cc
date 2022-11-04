#include "opendrive-cpp/parser/map_xml_parser.h"

#include <memory>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/parser/header_xml_parser.h"
#include "opendrive-cpp/parser/road_xml_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status MapXmlParser::Parse(const tinyxml2::XMLElement* root_ele,
                                      core::Map::Ptr map_ptr) {
  Init();
  this->ParseHeaderEle().ParseRoadEle();
  return status();
}

void MapXmlParser::Init() {
  if (!map_ele_ || !map_ptr_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
  }
}

MapXmlParser& MapXmlParser::ParseHeaderEle() {
  if (!IsValid()) return *this;
  core::Header::Ptr header_ptr = std::make_shared<core::Header>();
  map_ptr_->header = header_ptr;
  HeaderXmlParser header_parser;
  const tinyxml2::XMLElement* header_ele =
      map_ele_->FirstChildElement("header");
  header_parser.Parse(header_ele, header_ptr);
  return *this;
}

MapXmlParser& MapXmlParser::ParseRoadEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_road_ele =
      map_ele_->FirstChildElement("road");
  while (curr_road_ele) {
    core::Road::Ptr road_ptr = std::make_shared<core::Road>();
    map_ptr_->roads.emplace_back(road_ptr);
    RoadXmlParser road_parser;
    road_parser.Parse(curr_road_ele, road_ptr);
    XmlNextSiblingElement(curr_road_ele);
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
