#ifndef OPENDRIVE_CPP_LANES_PARSER_H_
#define OPENDRIVE_CPP_LANES_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class LanesXmlParser : public XmlParser {
 public:
  LanesXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* lanes_ele,
                          element::Lanes* lanes);

 private:
  LanesXmlParser& ParseLaneOffsetEle();
  LanesXmlParser& ParseLaneSectionEle();
  LanesXmlParser& ParseLaneSectionLanesEle(
      const tinyxml2::XMLElement* sections_ele,
      element::LaneSection& lane_section);

  LanesXmlParser& ParseLaneEle(const tinyxml2::XMLElement* lane_ele,
                               element::Lane& lane);
  LanesXmlParser& ParseLaneAttribute(const tinyxml2::XMLElement* lane_ele,
                                     element::Lane& lane);
  LanesXmlParser& ParseLaneLinkEle(const tinyxml2::XMLElement* lane_ele,
                                   element::Lane& lane);
  LanesXmlParser& ParseLaneWidthEle(const tinyxml2::XMLElement* lane_ele,
                                    element::Lane& lane);
  LanesXmlParser& ParseLaneBorderEle(const tinyxml2::XMLElement* lane_ele,
                                     element::Lane& lane);
  LanesXmlParser& ParseLaneRoadMarkEle(const tinyxml2::XMLElement* lane_ele,
                                       element::Lane& lane);

  const tinyxml2::XMLElement* lanes_ele_;
  element::Lanes* lanes_;
};
}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_LANES_PARSER_H_
