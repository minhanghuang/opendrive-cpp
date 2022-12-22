#ifndef OPENDRIVE_CPP_ROAD_PARSER_H_
#define OPENDRIVE_CPP_ROAD_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"
#include "opendrive-cpp/parser/section_parser.h"

namespace opendrive {
namespace parser {

class RoadXmlParser : public XmlParser {
 public:
  RoadXmlParser() = default;
  RoadXmlParser(const std::string& version);
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_road,
                          element::Road* ele_road);

 private:
  RoadXmlParser& Attributes();
  RoadXmlParser& LinkElement();
  RoadXmlParser& TypeElement();
  RoadXmlParser& PlanViewElement();
  RoadXmlParser& LanesElement();
  RoadXmlParser& CheckLanesElement();
  RoadXmlParser& GenerateRoad();
  const tinyxml2::XMLElement* xml_road_;
  element::Road* ele_road_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_PARSER_H_
