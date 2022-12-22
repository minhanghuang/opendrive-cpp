#ifndef OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_
#define OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_

#include <algorithm>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadLanesSectionXmlParser : public XmlParser {
 public:
  RoadLanesSectionXmlParser() = default;
  RoadLanesSectionXmlParser(const std::string& version);
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_section,
                          element::LaneSection* ele_section);

 private:
  RoadLanesSectionXmlParser& ParseAttributes();
  RoadLanesSectionXmlParser& ParseLanesEle();
  RoadLanesSectionXmlParser& ParseLaneEle(const tinyxml2::XMLElement* xml_lane,
                                          element::Lane& ele_lane);
  RoadLanesSectionXmlParser& ParseLaneAttributes(
      const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane);
  RoadLanesSectionXmlParser& ParseLaneLinkEle(
      const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane);
  RoadLanesSectionXmlParser& ParseLaneWidthEle(
      const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane);
  RoadLanesSectionXmlParser& ParseLaneBorderEle(
      const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane);
  RoadLanesSectionXmlParser& ParseLaneRoadMarkEle(
      const tinyxml2::XMLElement* xml_lane, element::Lane& ele_lane);
  const tinyxml2::XMLElement* xml_section_;
  element::LaneSection* ele_section_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_LANES_SECTION_PARSER_H_
