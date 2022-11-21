#ifndef OPENDRIVE_CPP_ROAD_LANES_OFFSET_PARSER_H_
#define OPENDRIVE_CPP_ROAD_LANES_OFFSET_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadLanesOffsetXmlParser : public XmlParser {
 public:
  RoadLanesOffsetXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_offset,
                          element::LaneOffset* ele_offset);

 private:
  RoadLanesOffsetXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* xml_offset_;
  element::LaneOffset* ele_offset_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_LANES_OFFSET_PARSER_H_
