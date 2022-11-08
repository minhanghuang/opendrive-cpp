#include "opendrive-cpp/parser/road_xml_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadXmlParser::Parse(const tinyxml2::XMLElement* road_ele,
                                       g::Road* road) {
  road_ele_ = road_ele;
  road_ = road;
  Init();
  ParseAttributes()
      .ParseLinkEle()
      .ParseTypeEle()
      .ParsePlanViewEle()
      .ParseLanesEle();
  return status();
}

void RoadXmlParser::Init() {
  if (!road_ele_ || !road_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
  }
}

RoadXmlParser& RoadXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  std::string rule;
  common::XmlQueryStringAttribute(road_ele_, "name", road_->attributes.name);
  common::XmlQueryEnumAttribute(
      road_ele_, "rule", road_->attributes.rule,
      std::map<std::string, RoadRule>{std::make_pair("LHT", RoadRule::LHT),
                                      std::make_pair("RHT", RoadRule::RHT)});
  common::XmlQueryDoubleAttribute(road_ele_, "length",
                                  road_->attributes.length);
  common::XmlQueryIntAttribute(road_ele_, "id", road_->attributes.id);
  common::XmlQueryIntAttribute(road_ele_, "junction",
                               road_->attributes.junction);
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLinkEle() {
  if (!IsValid()) return *this;
  auto link_ele = road_ele_->FirstChildElement("link");
  if (link_ele) {
    std::vector<std::string> link_elements{"predecessor", "successor"};
    for (const auto& link_element : link_elements) {
      const tinyxml2::XMLElement* link_type_ele =
          link_ele->FirstChildElement(link_element.c_str());
      if (link_type_ele) {
        if ("predecessor" == link_element) {
          common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                       road_->link.predecessor.id);
          common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                          road_->link.predecessor.s);
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementType", road_->link.predecessor.type,
              std::map<std::string, g::RoadLinkInfo::Type>{
                  std::make_pair("road", g::RoadLinkInfo::Type::ROAD),
                  std::make_pair("junction", g::RoadLinkInfo::Type::JUNCTION)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "contactPoint", road_->link.predecessor.point_type,
              std::map<std::string, g::RoadLinkInfo::PointType>{
                  std::make_pair("start", g::RoadLinkInfo::PointType::START),
                  std::make_pair("end", g::RoadLinkInfo::PointType::END)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementDir", road_->link.predecessor.dir,
              std::map<std::string, g::RoadLinkInfo::Dir>{
                  std::make_pair("+", g::RoadLinkInfo::Dir::PLUS),
                  std::make_pair("-", g::RoadLinkInfo::Dir::MINUS)});
        } else if ("successor" == link_element) {
          common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                       road_->link.successor.id);
          common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                          road_->link.successor.s);
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementType", road_->link.successor.type,
              std::map<std::string, g::RoadLinkInfo::Type>{
                  std::make_pair("road", g::RoadLinkInfo::Type::ROAD),
                  std::make_pair("junction", g::RoadLinkInfo::Type::JUNCTION)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "contactPoint", road_->link.successor.point_type,
              std::map<std::string, g::RoadLinkInfo::PointType>{
                  std::make_pair("start", g::RoadLinkInfo::PointType::START),
                  std::make_pair("end", g::RoadLinkInfo::PointType::END)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementDir", road_->link.predecessor.dir,
              std::map<std::string, g::RoadLinkInfo::Dir>{
                  std::make_pair("+", g::RoadLinkInfo::Dir::PLUS),
                  std::make_pair("-", g::RoadLinkInfo::Dir::MINUS)});
        }
      }
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseTypeEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* curr_type_ele =
      road_ele_->FirstChildElement("type");
  while (curr_type_ele) {
    g::RoadTypeInfo road_type_info;
    common::XmlQueryDoubleAttribute(curr_type_ele, "s", road_type_info.s);
    common::XmlQueryEnumAttribute(
        curr_type_ele, "type", road_type_info.type,
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
    common::XmlQueryStringAttribute(curr_type_ele, "country",
                                    road_type_info.country);
    const tinyxml2::XMLElement* speed_ele =
        curr_type_ele->FirstChildElement("speed");
    if (speed_ele) {
      common::XmlQueryDoubleAttribute(speed_ele, "max",
                                      road_type_info.max_speed);
      common::XmlQueryEnumAttribute(
          speed_ele, "unit", road_type_info.speed_unit,
          std::map<std::string, RoadSpeedUnit>{
              std::make_pair("m/s", RoadSpeedUnit::MS),
              std::make_pair("mph", RoadSpeedUnit::MPH),
              std::make_pair("km/h", RoadSpeedUnit::KMH),
          });
    }
    road_->type_info.emplace_back(road_type_info);
    curr_type_ele = common::XmlNextSiblingElement(curr_type_ele);
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParsePlanViewEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* planview_ele =
      road_ele_->FirstChildElement("planView");
  if (planview_ele) {
    const tinyxml2::XMLElement* curr_geometry_ele =
        planview_ele->FirstChildElement("geometry");
    while (curr_geometry_ele) {
      const tinyxml2::XMLElement* geometry_type_ele =
          curr_geometry_ele->FirstChildElement("line");
      std::shared_ptr<g::GeometryAttributes> geometry_base_ptr;
      if (geometry_type_ele) {
        std::shared_ptr<g::GeometryAttributesLine> geometry_ptr =
            std::make_shared<g::GeometryAttributesLine>();
        geometry_ptr->type = GeometryType::LINE;
        geometry_base_ptr =
            std::dynamic_pointer_cast<g::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("arc");
      if (geometry_type_ele) {
        std::shared_ptr<g::GeometryAttributesArc> geometry_ptr =
            std::make_shared<g::GeometryAttributesArc>();
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvature",
                                        geometry_ptr->curvature);
        geometry_ptr->type = GeometryType::ARC;
        geometry_base_ptr =
            std::dynamic_pointer_cast<g::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("spiral");
      if (geometry_type_ele) {
        std::shared_ptr<g::GeometryAttributesSpiral> geometry_ptr =
            std::make_shared<g::GeometryAttributesSpiral>();
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvStart",
                                        geometry_ptr->curve_start);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvEnd",
                                        geometry_ptr->curve_end);
        geometry_ptr->type = GeometryType::SPIRAL;
        geometry_base_ptr =
            std::dynamic_pointer_cast<g::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("poly3");
      if (geometry_type_ele) {
        std::shared_ptr<g::GeometryAttributesPoly3> geometry_ptr =
            std::make_shared<g::GeometryAttributesPoly3>();
        common::XmlQueryDoubleAttribute(geometry_type_ele, "a",
                                        geometry_ptr->a);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "b",
                                        geometry_ptr->b);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "c",
                                        geometry_ptr->c);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "d",
                                        geometry_ptr->d);
        geometry_ptr->type = GeometryType::POLY3;
        geometry_base_ptr =
            std::dynamic_pointer_cast<g::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("paramPoly3");
      if (geometry_type_ele) {
        std::shared_ptr<g::GeometryAttributesParamPoly3> geometry_ptr =
            std::make_shared<g::GeometryAttributesParamPoly3>();
        common::XmlQueryEnumAttribute(
            geometry_type_ele, "pRange", geometry_ptr->p_range,
            std::map<std::string, g::GeometryAttributesParamPoly3::PRange>{
                std::make_pair(
                    "arcLength",
                    g::GeometryAttributesParamPoly3::PRange::ARCLENGTH),
                std::make_pair(
                    "normalized",
                    g::GeometryAttributesParamPoly3::PRange::NORMALIZED),
            });
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aU",
                                        geometry_ptr->aU);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bU",
                                        geometry_ptr->bU);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cU",
                                        geometry_ptr->cU);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dU",
                                        geometry_ptr->dU);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aV",
                                        geometry_ptr->aV);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bV",
                                        geometry_ptr->bV);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cV",
                                        geometry_ptr->cV);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dV",
                                        geometry_ptr->dV);
        geometry_ptr->type = GeometryType::PARAMPOLY3;
        geometry_base_ptr =
            std::dynamic_pointer_cast<g::GeometryAttributes>(geometry_ptr);
      }
      if (!geometry_base_ptr) {
        set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
                   "Parse <geometry> Element Exception.");
        return *this;
      }
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "s",
                                      geometry_base_ptr->s);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "x",
                                      geometry_base_ptr->x);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "y",
                                      geometry_base_ptr->y);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "z",
                                      geometry_base_ptr->z);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "hdg",
                                      geometry_base_ptr->hdg);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "length",
                                      geometry_base_ptr->length);
      road_->plan_view.geometrys.emplace_back(geometry_base_ptr);
      curr_geometry_ele = common::XmlNextSiblingElement(curr_geometry_ele);
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLanesEle() {
  if (!IsValid()) return *this;
  g::Lanes lanes;
  const tinyxml2::XMLElement* lanes_ele = road_ele_->FirstChildElement("lanes");
  LanesXmlParser lanes_parser;
  lanes_parser.Parse(lanes_ele, &lanes);
  road_->lanes = lanes;
  return *this;
}

}  // namespace parser
}  // namespace opendrive
