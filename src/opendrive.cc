#include "opendrive-cpp/opendrive.h"

#include <cassert>

namespace opendrive {

Parser::~Parser() {}
Parser::Parser()
    : header_parser_(std::make_shared<parser::HeaderXmlParser>()) {}

opendrive::Status Parser::Header(const tinyxml2::XMLElement* xml_node,
                                 element::Header* header) {
  return header_parser_->Parse(xml_node, header);
}

opendrive::Status Parser::Road(const tinyxml2::XMLElement* xml_node,
                               element::Road* road) {
  return road_parser_->Parse(xml_node, road);
}

Adapter::~Adapter() {}
Adapter::Adapter() : road_adapter_(std::make_shared<adapter::RoadAdapter>()) {}

opendrive::Status Adapter::Road(const element::Road* ele_road,
                                core::Road::Ptr road_ptr, float step) {
  if (step <= 0) {
    return opendrive::Status{ErrorCode::ADAPTER_ROAD_ERROR, "input execption."};
  }

  road_adapter_->set_step(step);
  return road_adapter_->Run(ele_road, road_ptr);
}

}  // namespace opendrive
