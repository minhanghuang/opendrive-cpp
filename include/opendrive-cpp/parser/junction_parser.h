#ifndef OPENDRIVE_CPP_JUNCTION_PARSER_H_
#define OPENDRIVE_CPP_JUNCTION_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/road_link_parser.h"
#include "opendrive-cpp/parser/road_planview_parser.h"
#include "opendrive-cpp/parser/road_type_parser.h"
#include "opendrive-cpp/parser/util_parser.h"
#include "road_lanes_parser.h"

namespace opendrive {
namespace parser {

class JunctionXmlParser : public XmlParser {
 public:
  JunctionXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* xml_junction,
                          element::Junction* ele_junction);

 private:
  JunctionXmlParser& ParseAttributes();
  JunctionXmlParser& ParseConnection();
  const tinyxml2::XMLElement* xml_junction_;
  element::Junction* ele_junction_;
};

}  // namespace parser
}  // namespace opendrive

#endif  //  OPENDRIVE_CPP_JUNCTION_PARSER_H_
