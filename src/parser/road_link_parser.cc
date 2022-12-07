#include "opendrive-cpp/parser/road_link_parser.h"

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadLinkXmlParser::Parse(const tinyxml2::XMLElement* xml_link,
                                           element::RoadLink* ele_link) {
  xml_link_ = xml_link;
  ele_link_ = ele_link;
  if (!xml_link_ || !ele_link_) {
    set_status(ErrorCode::XML_ROAD_LINK_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseAttributes();
  return status();
}

RoadLinkXmlParser& RoadLinkXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  std::vector<std::string> xml_link_ments{"predecessor", "successor"};
  for (const auto& xml_link_ment : xml_link_ments) {
    const tinyxml2::XMLElement* link_type_ele =
        xml_link_->FirstChildElement(xml_link_ment.c_str());
    if (link_type_ele) {
      if ("predecessor" == xml_link_ment) {
        common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                     ele_link_->predecessor.id);
        common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                        ele_link_->predecessor.s);
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementType", ele_link_->predecessor.type,
            std::map<std::string, RoadLinkType>{
                std::make_pair("road", RoadLinkType::ROAD),
                std::make_pair("junction", RoadLinkType::JUNCTION)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "contactPoint", ele_link_->predecessor.point_type,
            std::map<std::string, element::RoadLinkInfo::PointType>{
                std::make_pair("start",
                               element::RoadLinkInfo::PointType::START),
                std::make_pair("end", element::RoadLinkInfo::PointType::END)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementDir", ele_link_->predecessor.dir,
            std::map<std::string, element::RoadLinkInfo::Dir>{
                std::make_pair("+", element::RoadLinkInfo::Dir::PLUS),
                std::make_pair("-", element::RoadLinkInfo::Dir::MINUS)});
      } else if ("successor" == xml_link_ment) {
        common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                     ele_link_->successor.id);
        common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                        ele_link_->successor.s);
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementType", ele_link_->successor.type,
            std::map<std::string, RoadLinkType>{
                std::make_pair("road", RoadLinkType::ROAD),
                std::make_pair("junction", RoadLinkType::JUNCTION)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "contactPoint", ele_link_->successor.point_type,
            std::map<std::string, element::RoadLinkInfo::PointType>{
                std::make_pair("start",
                               element::RoadLinkInfo::PointType::START),
                std::make_pair("end", element::RoadLinkInfo::PointType::END)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementDir", ele_link_->successor.dir,
            std::map<std::string, element::RoadLinkInfo::Dir>{
                std::make_pair("+", element::RoadLinkInfo::Dir::PLUS),
                std::make_pair("-", element::RoadLinkInfo::Dir::MINUS)});
      }
    }
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
