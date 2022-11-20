#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <memory>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/adapter/section_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/header_parser.h"
#include "opendrive-cpp/parser/road_parser.h"

namespace opendrive {

class Parser {
 public:
  ~Parser();
  Parser();
  opendrive::Status Header(const tinyxml2::XMLElement* xml_node,
                           element::Header* header);
  opendrive::Status Road(const tinyxml2::XMLElement* xml_node,
                         element::Road* road);

 private:
  std::shared_ptr<parser::HeaderXmlParser> header_parser_;
  std::shared_ptr<parser::RoadXmlParser> road_parser_;
};

class Adapter {
 public:
  ~Adapter();
  Adapter();
  opendrive::Status Map(const element::Map* ele_map, core::Map::Ptr map_ptr,
                        float step = 0.5);
  opendrive::Status Road(const element::Road* ele_road,
                         core::Road::Ptr road_ptr, float step = 0.5);

 private:
  std::shared_ptr<adapter::RoadAdapter> road_adapter_;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_H_
