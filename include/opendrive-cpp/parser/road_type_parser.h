#ifndef OPENDRIVE_CPP_ROAD_TYPE_PARSER_H_
#define OPENDRIVE_CPP_ROAD_TYPE_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class RoadTypeXmlParser : public XmlParser {
 public:
  RoadTypeXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_roadtype,
                          element::RoadTypeInfo* ele_roadtype);

 private:
  RoadTypeXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* xml_roadtype_;
  element::RoadTypeInfo* ele_roadtype_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ROAD_TYPE_PARSER_H_
