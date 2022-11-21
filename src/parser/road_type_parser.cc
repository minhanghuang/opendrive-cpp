#include "opendrive-cpp/parser/road_type_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadTypeXmlParser::Parse(
    const tinyxml2::XMLElement* xml_roadtype,
    element::RoadTypeInfo* ele_roadtype) {
  xml_roadtype_ = xml_roadtype;
  ele_roadtype_ = ele_roadtype;
  if (!xml_roadtype_ || !ele_roadtype_) {
    set_status(ErrorCode::XML_ROAD_TYPE_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseAttributes();
  return status();
}

RoadTypeXmlParser& RoadTypeXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  common::XmlQueryDoubleAttribute(xml_roadtype_, "s", ele_roadtype_->s);
  common::XmlQueryEnumAttribute(
      xml_roadtype_, "type", ele_roadtype_->type,
      std::map<std::string, RoadType>{
          std::make_pair("unknown", RoadType::UNKNOWN),
          std::make_pair("rural", RoadType::RURAL),
          std::make_pair("motorway", RoadType::MOTORWAY),
          std::make_pair("town", RoadType::TOWN),
          std::make_pair("pedestrian", RoadType::PEDESTRIAN),
          std::make_pair("bicycle", RoadType::BICYCLE),
          std::make_pair("townExpressway", RoadType::TOWNEXPRESSWAY),
          std::make_pair("townCollector", RoadType::TOWNCOLLECTOR),
          std::make_pair("townArterial", RoadType::TOWNARTERIAL),
          std::make_pair("townPrivate", RoadType::TOWNPRIVATE),
          std::make_pair("townLocal", RoadType::TOWNLOCAL),
          std::make_pair("townPlayStreet", RoadType::TOWNPLAYSTREET)});
  common::XmlQueryStringAttribute(xml_roadtype_, "country",
                                  ele_roadtype_->country);
  const tinyxml2::XMLElement* speed_ele =
      xml_roadtype_->FirstChildElement("speed");
  if (speed_ele) {
    common::XmlQueryDoubleAttribute(speed_ele, "max", ele_roadtype_->max_speed);
    common::XmlQueryEnumAttribute(
        speed_ele, "unit", ele_roadtype_->speed_unit,
        std::map<std::string, RoadSpeedUnit>{
            std::make_pair("m/s", RoadSpeedUnit::MS),
            std::make_pair("mph", RoadSpeedUnit::MPH),
            std::make_pair("km/h", RoadSpeedUnit::KMH),
        });
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
