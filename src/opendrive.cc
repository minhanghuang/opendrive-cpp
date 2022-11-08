#include "opendrive-cpp/opendrive.h"

namespace opendrive {

Parser::~Parser() {}
Parser::Parser()
    : header_parser_(std::make_shared<parser::HeaderXmlParser>()) {}

opendrive::Status Parser::Header(const tinyxml2::XMLElement* xml_node,
                                 base::Header* header) {
  return header_parser_->Parse(xml_node, header);
}

opendrive::Status Parser::Road(const tinyxml2::XMLElement* xml_node,
                               base::Road* road) {
  return road_parser_->Parse(xml_node, road);
}

Adapter::~Adapter() {}
Adapter::Adapter()
    : section_adapter_(std::make_shared<adapter::SectionAdapter>()) {}

opendrive::Status Adapter::LaneSection(const base::LaneSection* odr_section,
                                       core::Section::Ptr section_ptr) {
  return section_adapter_->Run(odr_section, section_ptr);
}

}  // namespace opendrive
