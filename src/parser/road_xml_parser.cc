#include "opendrive-cpp/parser/road_xml_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadXmlParser::Parse(const tinyxml2::XMLElement* road_ele,
                                       element::Road* road) {
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
              std::map<std::string, element::RoadLinkInfo::Type>{
                  std::make_pair("road", element::RoadLinkInfo::Type::ROAD),
                  std::make_pair("junction",
                                 element::RoadLinkInfo::Type::JUNCTION)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "contactPoint", road_->link.predecessor.point_type,
              std::map<std::string, element::RoadLinkInfo::PointType>{
                  std::make_pair("start",
                                 element::RoadLinkInfo::PointType::START),
                  std::make_pair("end",
                                 element::RoadLinkInfo::PointType::END)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementDir", road_->link.predecessor.dir,
              std::map<std::string, element::RoadLinkInfo::Dir>{
                  std::make_pair("+", element::RoadLinkInfo::Dir::PLUS),
                  std::make_pair("-", element::RoadLinkInfo::Dir::MINUS)});
        } else if ("successor" == link_element) {
          common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                       road_->link.successor.id);
          common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                          road_->link.successor.s);
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementType", road_->link.successor.type,
              std::map<std::string, element::RoadLinkInfo::Type>{
                  std::make_pair("road", element::RoadLinkInfo::Type::ROAD),
                  std::make_pair("junction",
                                 element::RoadLinkInfo::Type::JUNCTION)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "contactPoint", road_->link.successor.point_type,
              std::map<std::string, element::RoadLinkInfo::PointType>{
                  std::make_pair("start",
                                 element::RoadLinkInfo::PointType::START),
                  std::make_pair("end",
                                 element::RoadLinkInfo::PointType::END)});
          common::XmlQueryEnumAttribute(
              link_type_ele, "elementDir", road_->link.predecessor.dir,
              std::map<std::string, element::RoadLinkInfo::Dir>{
                  std::make_pair("+", element::RoadLinkInfo::Dir::PLUS),
                  std::make_pair("-", element::RoadLinkInfo::Dir::MINUS)});
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
    element::RoadTypeInfo road_type_info;
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
    double s = 0.;
    double x = 0.;
    double y = 0.;
    double hdg = 0.;
    double length = 0.;
    GeometryType type = GeometryType::UNKNOWN;
    double curvature = 0.;
    double curve_start = 0.;
    double curve_end = 0.;
    double a = 0.;
    double b = 0.;
    double c = 0.;
    double d = 0.;
    double au = 0.;
    double bu = 0.;
    double cu = 0.;
    double du = 0.;
    double av = 0.;
    double bv = 0.;
    double cv = 0.;
    double dv = 0.;
    element::GeometryParamPoly3::PRange p_range =
        element::GeometryParamPoly3::PRange::UNKNOWN;
    while (curr_geometry_ele) {
      s = 0.;
      x = 0.;
      y = 0.;
      hdg = 0.;
      length = 0.;
      type = GeometryType::UNKNOWN;
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "s", s);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "x", x);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "y", y);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "hdg", hdg);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "length", length);

      const tinyxml2::XMLElement* geometry_type_ele =
          curr_geometry_ele->FirstChildElement("line");
      std::shared_ptr<element::Geometry> geometry_base_ptr;
      if (geometry_type_ele) {
        std::shared_ptr<element::GeometryLine> geometry_ptr =
            std::make_shared<element::GeometryLine>(s, x, y, hdg, length,
                                                    GeometryType::LINE);
        geometry_base_ptr =
            std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("arc");
      if (geometry_type_ele) {
        curvature = 0.;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvature",
                                        curvature);

        std::shared_ptr<element::GeometryArc> geometry_ptr =
            std::make_shared<element::GeometryArc>(
                s, x, y, hdg, length, GeometryType::ARC, curvature);
        geometry_base_ptr =
            std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("spiral");
      if (geometry_type_ele) {
        curve_start = 0.;
        curve_end = 0.;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvStart",
                                        curve_start);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvEnd",
                                        curve_end);
        std::shared_ptr<element::GeometrySpiral> geometry_ptr =
            std::make_shared<element::GeometrySpiral>(s, x, y, hdg, length,
                                                      GeometryType::SPIRAL,
                                                      curve_start, curve_end);
        geometry_base_ptr =
            std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("poly3");
      if (geometry_type_ele) {
        a = 0.;
        b = 0.;
        c = 0.;
        d = 0.;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "a", a);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "b", b);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "c", c);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "d", d);

        std::shared_ptr<element::GeometryPoly3> geometry_ptr =
            std::make_shared<element::GeometryPoly3>(
                s, x, y, hdg, length, GeometryType::POLY3, a, b, c, d);
        geometry_base_ptr =
            std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
      }
      geometry_type_ele = curr_geometry_ele->FirstChildElement("paramPoly3");
      if (geometry_type_ele) {
        au = 0.;
        bu = 0.;
        cu = 0.;
        du = 0.;
        av = 0.;
        bv = 0.;
        cv = 0.;
        dv = 0.;
        p_range = element::GeometryParamPoly3::PRange::UNKNOWN;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aU", au);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bU", bu);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cU", cu);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dU", du);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aV", av);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bV", bv);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cV", cv);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dV", dv);

        std::shared_ptr<element::GeometryParamPoly3> geometry_ptr =
            std::make_shared<element::GeometryParamPoly3>(
                s, x, y, hdg, length, GeometryType::PARAMPOLY3, au, bu, cu, du,
                av, bv, cv, dv, p_range);
        common::XmlQueryEnumAttribute(
            geometry_type_ele, "pRange", p_range,
            std::map<std::string, element::GeometryParamPoly3::PRange>{
                std::make_pair("arcLength",
                               element::GeometryParamPoly3::PRange::ARCLENGTH),
                std::make_pair("normalized",
                               element::GeometryParamPoly3::PRange::NORMALIZED),
            });
        geometry_base_ptr =
            std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
      }
      if (!geometry_base_ptr) {
        set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
                   "Parse <geometry> Element Exception.");
        return *this;
      }
      road_->plan_view.geometrys.emplace_back(geometry_base_ptr);
      curr_geometry_ele = common::XmlNextSiblingElement(curr_geometry_ele);
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLanesEle() {
  if (!IsValid()) return *this;
  element::Lanes lanes;
  const tinyxml2::XMLElement* lanes_ele = road_ele_->FirstChildElement("lanes");
  LanesXmlParser lanes_parser;
  lanes_parser.Parse(lanes_ele, &lanes);
  road_->lanes = lanes;
  return *this;
}

}  // namespace parser
}  // namespace opendrive
