#ifndef OPENDRIVE_CPP_ROAD_PLANVIEW_PARSER_H_
#define OPENDRIVE_CPP_ROAD_PLANVIEW_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadPlanViewXmlParser : public XmlParser {
 public:
  RoadPlanViewXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_planview,
                          element::RoadPlanView* ele_planview);

 private:
  RoadPlanViewXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* xml_planview_;
  element::RoadPlanView* ele_planview_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_PLANVIEW_PARSER_H_
