#ifndef OPENDRIVE_CPP_ROAD_XML_PARSER_H_
#define OPENDRIVE_CPP_ROAD_XML_PARSER_H_

#include "lanes_xml_parser.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/types.h"
#include "opendrive-cpp/parser/util_xml_parser.h"

namespace opendrive {
namespace parser {

class RoadXmlParser : public XmlParser {
 public:
  RoadXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* road_ele,
                          g::Road* road_ptr);

 private:
  virtual void Init() override;
  RoadXmlParser& ParseAttributes();
  RoadXmlParser& ParseLinkEle();
  RoadXmlParser& ParseTypeEle();
  RoadXmlParser& ParsePlanViewEle();
  RoadXmlParser& ParseLanesEle();
  const tinyxml2::XMLElement* road_ele_;
  g::Road* road_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_XML_PARSER_H_
