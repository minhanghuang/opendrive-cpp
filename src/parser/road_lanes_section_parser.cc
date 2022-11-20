#include "opendrive-cpp/parser/road_lanes_section_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadLanesSectionXmlParser::Parse(
    const tinyxml2::XMLElement* xml_section,
    element::LaneSection* ele_section) {
  xml_section_ = xml_section;
  ele_section_ = ele_section;
  if (!xml_section_ || !ele_section_) {
    set_status(ErrorCode::XML_LANES_SECTION_ELEMENT_ERROR, "Input is null.");
  }

  return status();
}

}  // namespace parser
}  // namespace opendrive
