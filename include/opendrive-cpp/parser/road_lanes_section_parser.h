#ifndef OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_
#define OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadLanesSectionXmlParser : public XmlParser {
 public:
  RoadLanesSectionXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* ele_section,
                          element::LaneSection* section);

 private:
  RoadLanesSectionXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* xml_section_;
  element::LaneSection* ele_section_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_
