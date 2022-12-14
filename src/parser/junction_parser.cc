#include "opendrive-cpp/parser/junction_parser.h"

#include <map>
#include <string>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {
namespace parser {

opendrive::Status JunctionXmlParser::Parse(
    const tinyxml2::XMLElement* xml_junction, element::Junction* ele_junction) {
  xml_junction_ = xml_junction;
  ele_junction_ = ele_junction;
  if (!xml_junction_ || !ele_junction_) {
    set_status(ErrorCode::XML_JUNCTION_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseAttributes().ParseConnection();
  return status();
}

JunctionXmlParser& JunctionXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryIntAttribute(xml_junction_, "id",
                               ele_junction_->attributes.id);
  common::XmlQueryStringAttribute(xml_junction_, "name",
                                  ele_junction_->attributes.name);
  common::XmlQueryEnumAttribute(
      xml_junction_, "type", ele_junction_->attributes.type,
      std::map<std::string, JunctionType>{
          std::make_pair("default", JunctionType::DEFAULT),
          std::make_pair("virtual", JunctionType::VIRTUAL)});
  return *this;
}

JunctionXmlParser& JunctionXmlParser::ParseConnection() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_xml_connection =
      xml_junction_->FirstChildElement("connection");
  while (curr_xml_connection) {
    element::JunctionConnection connection;
    common::XmlQueryIntAttribute(curr_xml_connection, "id", connection.id);
    common::XmlQueryEnumAttribute(
        curr_xml_connection, "type", connection.type,
        std::map<std::string, JunctionType>{
            std::make_pair("default", JunctionType::DEFAULT),
            std::make_pair("virtual", JunctionType::VIRTUAL)});
    common::XmlQueryIntAttribute(curr_xml_connection, "incomingRoad",
                                 connection.incoming_road);
    common::XmlQueryIntAttribute(curr_xml_connection, "connectingRoad",
                                 connection.connecting_road);
    common::XmlQueryEnumAttribute(
        curr_xml_connection, "contactPoint", connection.contact_point,
        std::map<std::string, JunctionContactPointType>{
            std::make_pair("start", JunctionContactPointType::START),
            std::make_pair("end", JunctionContactPointType::END)});
    const tinyxml2::XMLElement* curr_xml_laneLink =
        curr_xml_connection->FirstChildElement("laneLink");
    while (curr_xml_laneLink) {
      element::JunctionLaneLink lane_link;
      common::XmlQueryIntAttribute(curr_xml_laneLink, "from", lane_link.from);
      common::XmlQueryIntAttribute(curr_xml_laneLink, "to", lane_link.to);
      connection.lane_links.emplace_back(lane_link);
      curr_xml_laneLink = common::XmlNextSiblingElement(curr_xml_laneLink);
    }
    ele_junction_->connections.emplace_back(connection);
    curr_xml_connection = common::XmlNextSiblingElement(curr_xml_connection);
  }
  return *this;
}

}  // namespace parser
}  // namespace opendrive
