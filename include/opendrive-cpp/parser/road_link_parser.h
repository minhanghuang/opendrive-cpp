#ifndef OPENDRIVE_CPP_ROAD_LINK_PARSER_H_
#define OPENDRIVE_CPP_ROAD_LINK_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadLinkXmlParser : public XmlParser {
 public:
  RoadLinkXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_link,
                          element::RoadLink* ele_link);

 private:
  RoadLinkXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* xml_link_;
  element::RoadLink* ele_link_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_LINK_PARSER_H_
