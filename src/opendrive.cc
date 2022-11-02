#include "opendrive-cpp/opendrive.h"

namespace opendrive {

Parser::~Parser() {}
Parser::Parser()
    : header_parser_(std::make_shared<parser::HeaderXmlParser>()) {}

opendrive::Status Parser::Header(const tinyxml2::XMLElement* xml_node,
                                 core::Header::Ptr header_ptr) {
  return header_parser_->Parse(xml_node, header_ptr);
}

opendrive::Status Parser::Road(const tinyxml2::XMLElement* xml_node,
                               core::Road::Ptr road_ptr) {
  return opendrive::Status(opendrive::ErrorCode::OK, "ok");
}

}  // namespace opendrive
