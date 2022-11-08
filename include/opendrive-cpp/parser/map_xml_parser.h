#ifndef OPENDRIVE_CPP_MAP_XML_PARSER_H_
#define OPENDRIVE_CPP_MAP_XML_PARSER_H_

#include "lanes_xml_parser.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/types.h"

namespace opendrive {
namespace parser {
class MapXmlParser : public XmlParser {
 public:
  opendrive::Status Parse(const tinyxml2::XMLElement* map_ele,
                          g::Map::Ptr map_ptr);

 private:
  virtual void Init() override;
  MapXmlParser& ParseHeaderEle();
  MapXmlParser& ParseRoadEle();
  const tinyxml2::XMLElement* map_ele_;
  g::Map::Ptr map_ptr_;
};

}  // namespace parser
}  // namespace opendrive

#endif  //  OPENDRIVE_CPP_MAP_XML_PARSER_H_
