#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <memory>

#include "opendrive-cpp/adapter/section_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/header_xml_parser.h"
#include "opendrive-cpp/parser/road_xml_parser.h"

namespace opendrive {

class Parser {
 public:
  ~Parser();
  Parser();
  opendrive::Status Header(const tinyxml2::XMLElement* xml_node,
                           g::Header* header);
  opendrive::Status Road(const tinyxml2::XMLElement* xml_node, g::Road* road);

 private:
  std::shared_ptr<parser::HeaderXmlParser> header_parser_;
  std::shared_ptr<parser::RoadXmlParser> road_parser_;
};

class Adapter {
 public:
  ~Adapter();
  Adapter();
  opendrive::Status LaneSection(const g::LaneSection* odr_section,
                                core::Section::Ptr section_ptr);

 private:
  std::shared_ptr<adapter::SectionAdapter> section_adapter_;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_H_
