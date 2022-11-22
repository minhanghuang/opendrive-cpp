#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <memory>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/header_parser.h"
#include "opendrive-cpp/parser/map_parser.h"
#include "opendrive-cpp/parser/road_lanes_parser.h"
#include "opendrive-cpp/parser/road_lanes_section_parser.h"
#include "opendrive-cpp/parser/road_link_parser.h"
#include "opendrive-cpp/parser/road_parser.h"
#include "opendrive-cpp/parser/road_planview_parser.h"
#include "opendrive-cpp/parser/road_type_parser.h"

namespace opendrive {

class Parser {
 public:
  ~Parser();
  Parser();
  opendrive::Status Map(const tinyxml2::XMLElement* xml_root,
                        element::Map* ele_map);
  opendrive::Status Header(const tinyxml2::XMLElement* xml_header,
                           element::Header* ele_header);
  opendrive::Status Road(const tinyxml2::XMLElement* xml_road,
                         element::Road* ele_road);
  opendrive::Status RoadLink(const tinyxml2::XMLElement* xml_road_link,
                             element::RoadLink* ele_road_link);
  opendrive::Status RoadType(const tinyxml2::XMLElement* xml_road_type,
                             element::RoadTypeInfo* ele_road_type);
  opendrive::Status RoadPlanView(const tinyxml2::XMLElement* xml_road_planview,
                                 element::RoadPlanView* ele_road_planview);
  opendrive::Status RoadLanes(const tinyxml2::XMLElement* xml_road_lanes,
                              element::Lanes* ele_road_lanes);

 private:
  std::unique_ptr<parser::MapXmlParser> map_parser_;
  std::unique_ptr<parser::HeaderXmlParser> header_parser_;
  std::unique_ptr<parser::RoadXmlParser> road_parser_;
  std::unique_ptr<parser::RoadLinkXmlParser> road_link_parser_;
  std::unique_ptr<parser::RoadTypeXmlParser> road_type_parser_;
  std::unique_ptr<parser::RoadPlanViewXmlParser> road_planview_parser_;
  std::unique_ptr<parser::RoadLanesXmlParser> road_lanes_parser_;
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
