#ifndef OPENDRIVE_CPP_LANES_PARSER_H_
#define OPENDRIVE_CPP_LANES_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/road_lanes_offset_parser.h"
#include "opendrive-cpp/parser/road_lanes_section_parser.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadLanesXmlParser : public XmlParser {
 public:
  RoadLanesXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* lanes_ele,
                          element::Lanes* lanes);

 private:
  RoadLanesXmlParser& ParseLaneOffsetEle();
  RoadLanesXmlParser& ParseLaneSectionEle();

  RoadLanesOffsetXmlParser offset_parser_;
  RoadLanesSectionXmlParser section_parser_;
  const tinyxml2::XMLElement* xml_lanes_;
  element::Lanes* ele_lanes_;
};
}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_LANES_PARSER_H_
