#ifndef OPENDRIVE_CPP_MAP_PARSER_H_
#define OPENDRIVE_CPP_MAP_PARSER_H_

#include <memory>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/road_parser.h"
#include "opendrive-cpp/parser/section_parser.h"

namespace opendrive {
namespace parser {
class MapXmlParser : public XmlParser {
 public:
  opendrive::Status Parse(const tinyxml2::XMLElement* map_ele,
                          element::Map::Ptr ele_map);

 private:
  MapXmlParser& HeaderElement();
  MapXmlParser& JunctionElement();
  MapXmlParser& RoadElement();
  const tinyxml2::XMLElement* xml_map_;
  element::Map::Ptr ele_map_;
};

}  // namespace parser
}  // namespace opendrive

#endif  //  OPENDRIVE_CPP_MAP_PARSER_H_
