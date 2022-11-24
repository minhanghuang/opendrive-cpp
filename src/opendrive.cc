#include "opendrive-cpp/opendrive.h"

#include <memory>

namespace opendrive {

Parser::Parser()
    : map_parser_(std::make_unique<parser::MapXmlParser>()),
      header_parser_(std::make_unique<parser::HeaderXmlParser>()),
      road_parser_(std::make_unique<parser::RoadXmlParser>()),
      road_link_parser_(std::make_unique<parser::RoadLinkXmlParser>()),
      road_type_parser_(std::make_unique<parser::RoadTypeXmlParser>()),
      road_planview_parser_(std::make_unique<parser::RoadPlanViewXmlParser>()),
      road_lanes_parser_(std::make_unique<parser::RoadLanesXmlParser>()) {}

opendrive::Status Parser::Map(const std::string& xml_file,
                              element::Map* ele_map) {
  tinyxml2::XMLDocument xml_doc;
  xml_doc.LoadFile(xml_file.c_str());
  if (xml_doc.Error()) {
    return Status{ErrorCode::XML_ROAD_ELEMENT_ERROR,
                  "Parse Xml File Exection."};
  }
  tinyxml2::XMLElement* xml_root = xml_doc.RootElement();
  return map_parser_->Parse(xml_root, ele_map);
}

opendrive::Status Parser::Map(const tinyxml2::XMLElement* xml_root,
                              element::Map* ele_map) {
  return map_parser_->Parse(xml_root, ele_map);
}

opendrive::Status Parser::Header(const tinyxml2::XMLElement* xml_header,
                                 element::Header* header) {
  return header_parser_->Parse(xml_header, header);
}

opendrive::Status Parser::Road(const tinyxml2::XMLElement* xml_road,
                               element::Road* road) {
  return road_parser_->Parse(xml_road, road);
}

opendrive::Status Parser::RoadLink(const tinyxml2::XMLElement* xml_road_link,
                                   element::RoadLink* ele_road_link) {
  return road_link_parser_->Parse(xml_road_link, ele_road_link);
}

opendrive::Status Parser::RoadType(const tinyxml2::XMLElement* xml_road_type,
                                   element::RoadTypeInfo* ele_road_type) {
  return road_type_parser_->Parse(xml_road_type, ele_road_type);
}

opendrive::Status Parser::RoadPlanView(
    const tinyxml2::XMLElement* xml_road_planview,
    element::RoadPlanView* ele_road_planview) {
  return road_planview_parser_->Parse(xml_road_planview, ele_road_planview);
}

opendrive::Status Parser::RoadLanes(const tinyxml2::XMLElement* xml_road_lanes,
                                    element::Lanes* ele_road_lanes) {
  return road_lanes_parser_->Parse(xml_road_lanes, ele_road_lanes);
}

Adapter::Adapter()
    : map_adapter_(std::make_unique<adapter::MapAdapter>()),
      road_adapter_(std::make_unique<adapter::RoadAdapter>()) {}

opendrive::Status Adapter::Map(const element::Map* ele_map,
                               core::Map::Ptr map_ptr, float step) {
  if (step <= 0) {
    return opendrive::Status{ErrorCode::ADAPTER_ROAD_ERROR, "Input Execption."};
  }
  map_adapter_->set_step(step);
  return map_adapter_->Run(ele_map, map_ptr);
}

opendrive::Status Adapter::Road(const element::Road* ele_road,
                                core::Road::Ptr road_ptr, float step) {
  if (step <= 0) {
    return opendrive::Status{ErrorCode::ADAPTER_ROAD_ERROR, "Input Execption."};
  }

  road_adapter_->set_step(step);
  return road_adapter_->Run(ele_road, road_ptr);
}

}  // namespace opendrive
