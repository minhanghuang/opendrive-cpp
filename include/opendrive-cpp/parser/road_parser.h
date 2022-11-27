#ifndef OPENDRIVE_CPP_ROAD_PARSER_H_
#define OPENDRIVE_CPP_ROAD_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/road_link_parser.h"
#include "opendrive-cpp/parser/road_planview_parser.h"
#include "opendrive-cpp/parser/road_type_parser.h"
#include "opendrive-cpp/parser/util_parser.h"
#include "road_lanes_parser.h"

namespace opendrive {
namespace parser {

class RoadXmlParser : public XmlParser {
 public:
  RoadXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_road,
                          element::Road* ele_road);

 private:
  RoadXmlParser& ParseAttributes();
  RoadXmlParser& ParseLinkEle();
  RoadXmlParser& ParseTypeEle();
  RoadXmlParser& ParsePlanViewEle();
  RoadXmlParser& ParseLanesEle();
  RoadXmlParser& CheckLanesEle();
  RoadXmlParser& GenerateRoad();
  RoadLinkXmlParser road_link_parser_;
  RoadTypeXmlParser road_type_parser_;
  RoadPlanViewXmlParser road_planview_parser_;
  const tinyxml2::XMLElement* xml_road_;
  element::Road* ele_road_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_PARSER_H_
