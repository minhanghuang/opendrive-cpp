#include "opendrive-cpp/opendrive.h"

#include <memory>

namespace opendrive {

Parser::Parser() : map_parser_(std::make_unique<parser::MapXmlParser>()) {}

std::string Parser::GetOpenDriveVersion() const {
  return map_parser_->opendrive_version();
}

opendrive::Status Parser::ParseMap(const std::string& xml_file,
                                   element::Map::Ptr ele_map) {
  tinyxml2::XMLDocument xml_doc;
  xml_doc.LoadFile(xml_file.c_str());
  if (xml_doc.Error()) {
    return Status{ErrorCode::XML_ROAD_ELEMENT_ERROR,
                  "Parse Xml File Exection."};
  }
  tinyxml2::XMLElement* xml_root = xml_doc.RootElement();
  return map_parser_->Parse(xml_root, ele_map);
}

opendrive::Status Parser::ParseMap(const tinyxml2::XMLElement* xml_root,
                                   element::Map::Ptr ele_map) {
  return map_parser_->Parse(xml_root, ele_map);
}

}  // namespace opendrive
