#include "opendrive-cpp/parser/road_parser.h"

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {
namespace parser {

RoadXmlParser::RoadXmlParser(const std::string& version) : XmlParser(version) {}

opendrive::Status RoadXmlParser::Parse(const tinyxml2::XMLElement* xml_road,
                                       element::Road* ele_road) {
  xml_road_ = xml_road;
  ele_road_ = ele_road;
  if (!xml_road_ || !ele_road_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  Attributes()
      .LinkElement()
      .TypeElement()
      .PlanViewElement()
      .LanesElement()
      .GenerateRoad();
  return status();
}

RoadXmlParser& RoadXmlParser::Attributes() {
  if (!IsValid()) return *this;
  std::string rule;
  common::XmlQueryStringAttribute(xml_road_, "name",
                                  ele_road_->attributes.name);
  common::XmlQueryEnumAttribute(
      xml_road_, "rule", ele_road_->attributes.rule,
      std::map<std::string, RoadRule>{std::make_pair("LHT", RoadRule::LHT),
                                      std::make_pair("RHT", RoadRule::RHT)});
  common::XmlQueryDoubleAttribute(xml_road_, "length",
                                  ele_road_->attributes.length);
  common::XmlQueryIntAttribute(xml_road_, "id", ele_road_->attributes.id);
  common::XmlQueryIntAttribute(xml_road_, "junction",
                               ele_road_->attributes.junction);
  return *this;
}

RoadXmlParser& RoadXmlParser::LinkElement() {
  if (!IsValid()) return *this;
  auto xml_link = xml_road_->FirstChildElement("link");
  if (!xml_link) {
    return *this;
  }
  std::vector<std::string> xml_link_ments{"predecessor", "successor"};
  for (const auto& xml_link_ment : xml_link_ments) {
    const tinyxml2::XMLElement* link_type_ele =
        xml_link->FirstChildElement(xml_link_ment.c_str());
    if (link_type_ele) {
      if ("predecessor" == xml_link_ment) {
        common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                     ele_road_->link.predecessor.id);
        common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                        ele_road_->link.predecessor.s);
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementType", ele_road_->link.predecessor.type,
            std::map<std::string, RoadLinkType>{
                std::make_pair("road", RoadLinkType::ROAD),
                std::make_pair("junction", RoadLinkType::JUNCTION)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "contactPoint",
            ele_road_->link.predecessor.contact_point,
            std::map<std::string, ContactPointType>{
                std::make_pair("start", ContactPointType::START),
                std::make_pair("end", ContactPointType::END)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementDir", ele_road_->link.predecessor.dir,
            std::map<std::string, Dir>{std::make_pair("+", Dir::PLUS),
                                       std::make_pair("-", Dir::MINUS)});
      } else if ("successor" == xml_link_ment) {
        common::XmlQueryIntAttribute(link_type_ele, "elementId",
                                     ele_road_->link.successor.id);
        common::XmlQueryDoubleAttribute(link_type_ele, "elementS",
                                        ele_road_->link.successor.s);
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementType", ele_road_->link.successor.type,
            std::map<std::string, RoadLinkType>{
                std::make_pair("road", RoadLinkType::ROAD),
                std::make_pair("junction", RoadLinkType::JUNCTION)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "contactPoint",
            ele_road_->link.successor.contact_point,
            std::map<std::string, ContactPointType>{
                std::make_pair("start", ContactPointType::START),
                std::make_pair("end", ContactPointType::END)});
        common::XmlQueryEnumAttribute(
            link_type_ele, "elementDir", ele_road_->link.successor.dir,
            std::map<std::string, Dir>{std::make_pair("+", Dir::PLUS),
                                       std::make_pair("-", Dir::MINUS)});
      }
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::TypeElement() {
  if (!IsValid()) return *this;
  // 0~*
  const tinyxml2::XMLElement* curr_xml_type =
      xml_road_->FirstChildElement("type");
  while (curr_xml_type) {
    element::RoadTypeInfo ele_road_type;
    common::XmlQueryDoubleAttribute(curr_xml_type, "s", ele_road_type.s);
    common::XmlQueryEnumAttribute(
        curr_xml_type, "type", ele_road_type.type,
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
    common::XmlQueryStringAttribute(curr_xml_type, "country",
                                    ele_road_type.country);
    const tinyxml2::XMLElement* speed_ele =
        curr_xml_type->FirstChildElement("speed");
    if (speed_ele) {
      common::XmlQueryFloatAttribute(speed_ele, "max", ele_road_type.max_speed);
      common::XmlQueryEnumAttribute(speed_ele, "unit", ele_road_type.speed_unit,
                                    std::map<std::string, SpeedUnit>{
                                        std::make_pair("m/s", SpeedUnit::MS),
                                        std::make_pair("mph", SpeedUnit::MPH),
                                        std::make_pair("km/h", SpeedUnit::KMH),
                                    });
    }
    ele_road_->type_info.emplace_back(ele_road_type);
    curr_xml_type = common::XmlNextSiblingElement(curr_xml_type);
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::PlanViewElement() {
  if (!IsValid()) return *this;
  /// eq 1
  const tinyxml2::XMLElement* xml_planview =
      xml_road_->FirstChildElement("planView");
  if (!xml_planview) {
    set_status(ErrorCode::XML_ROAD_PLANVIEW_ELEMENT_ERROR,
               "ROAD PLANVIEW ELEMENT IS NULL.");
    return *this;
  }
  double s;
  double x;
  double y;
  double hdg;
  double length;
  GeometryType type;
  double curvature;
  double curve_start;
  double curve_end;
  double a;
  double b;
  double c;
  double d;
  double au;
  double bu;
  double cu;
  double du;
  double av;
  double bv;
  double cv;
  double dv;
  element::GeometryParamPoly3::PRange p_range =
      element::GeometryParamPoly3::PRange::UNKNOWN;
  const tinyxml2::XMLElement* curr_ele_geometry =
      xml_planview->FirstChildElement("geometry");
  while (curr_ele_geometry) {
    element::Geometry::Ptr geometry_base_ptr;
    s = 0.;
    x = 0.;
    y = 0.;
    hdg = 0.;
    length = 0.;
    type = GeometryType::UNKNOWN;
    common::XmlQueryDoubleAttribute(curr_ele_geometry, "s", s);
    common::XmlQueryDoubleAttribute(curr_ele_geometry, "x", x);
    common::XmlQueryDoubleAttribute(curr_ele_geometry, "y", y);
    common::XmlQueryDoubleAttribute(curr_ele_geometry, "hdg", hdg);
    common::XmlQueryDoubleAttribute(curr_ele_geometry, "length", length);

    const tinyxml2::XMLElement* ele_geometry_type =
        curr_ele_geometry->FirstChildElement("line");
    if (ele_geometry_type) {
      std::shared_ptr<element::GeometryLine> geometry_ptr =
          std::make_shared<element::GeometryLine>(s, x, y, hdg, length,
                                                  GeometryType::LINE);
      geometry_base_ptr =
          std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
    }
    ele_geometry_type = curr_ele_geometry->FirstChildElement("arc");
    if (ele_geometry_type) {
      curvature = 0.;
      common::XmlQueryDoubleAttribute(ele_geometry_type, "curvature",
                                      curvature);
      std::shared_ptr<element::GeometryArc> geometry_ptr =
          std::make_shared<element::GeometryArc>(s, x, y, hdg, length,
                                                 GeometryType::ARC, curvature);
      geometry_base_ptr =
          std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
    }
    ele_geometry_type = curr_ele_geometry->FirstChildElement("spiral");
    if (ele_geometry_type) {
      curve_start = 0.;
      curve_end = 0.;
      common::XmlQueryDoubleAttribute(ele_geometry_type, "curvStart",
                                      curve_start);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "curvEnd", curve_end);
      std::shared_ptr<element::GeometrySpiral> geometry_ptr =
          std::make_shared<element::GeometrySpiral>(s, x, y, hdg, length,
                                                    GeometryType::SPIRAL,
                                                    curve_start, curve_end);
      geometry_base_ptr =
          std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
    }
    ele_geometry_type = curr_ele_geometry->FirstChildElement("poly3");
    if (ele_geometry_type) {
      a = 0.;
      b = 0.;
      c = 0.;
      d = 0.;
      common::XmlQueryDoubleAttribute(ele_geometry_type, "a", a);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "b", b);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "c", c);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "d", d);

      std::shared_ptr<element::GeometryPoly3> geometry_ptr =
          std::make_shared<element::GeometryPoly3>(
              s, x, y, hdg, length, GeometryType::POLY3, a, b, c, d);
      geometry_base_ptr =
          std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
    }
    ele_geometry_type = curr_ele_geometry->FirstChildElement("paramPoly3");
    if (ele_geometry_type) {
      au = 0.;
      bu = 0.;
      cu = 0.;
      du = 0.;
      av = 0.;
      bv = 0.;
      cv = 0.;
      dv = 0.;
      p_range = element::GeometryParamPoly3::PRange::UNKNOWN;
      common::XmlQueryDoubleAttribute(ele_geometry_type, "aU", au);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "bU", bu);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "cU", cu);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "dU", du);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "aV", av);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "bV", bv);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "cV", cv);
      common::XmlQueryDoubleAttribute(ele_geometry_type, "dV", dv);
      common::XmlQueryEnumAttribute(
          ele_geometry_type, "pRange", p_range,
          std::map<std::string, element::GeometryParamPoly3::PRange>{
              std::make_pair("arcLength",
                             element::GeometryParamPoly3::PRange::ARCLENGTH),
              std::make_pair("normalized",
                             element::GeometryParamPoly3::PRange::NORMALIZED),
          });
      std::shared_ptr<element::GeometryParamPoly3> geometry_ptr =
          std::make_shared<element::GeometryParamPoly3>(
              s, x, y, hdg, length, GeometryType::PARAMPOLY3, au, bu, cu, du,
              av, bv, cv, dv, p_range);
      geometry_base_ptr =
          std::dynamic_pointer_cast<element::Geometry>(geometry_ptr);
    }
    if (!geometry_base_ptr) {
      set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
                 "Parse <geometry> Element Exception.");
      return *this;
    }
    ele_road_->plan_view.geometrys.emplace_back(geometry_base_ptr);
    curr_ele_geometry = common::XmlNextSiblingElement(curr_ele_geometry);
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::LanesElement() {
  if (!IsValid()) return *this;
  /// eq 1
  const tinyxml2::XMLElement* xml_lanes = xml_road_->FirstChildElement("lanes");
  if (!xml_lanes) {
    set_status(ErrorCode::XML_LANES_ELEMENT_ERROR,
               "ROAD LANES ELEMENT IS NULL.");
    return *this;
  }

  // lanes laneoffset
  // 0~*
  auto curr_xml_offset = xml_lanes->FirstChildElement("laneOffset");
  while (curr_xml_offset) {
    element::LaneOffset offset;
    common::XmlQueryDoubleAttribute(curr_xml_offset, "s", offset.s);
    common::XmlQueryDoubleAttribute(curr_xml_offset, "a", offset.a);
    common::XmlQueryDoubleAttribute(curr_xml_offset, "b", offset.b);
    common::XmlQueryDoubleAttribute(curr_xml_offset, "c", offset.c);
    common::XmlQueryDoubleAttribute(curr_xml_offset, "d", offset.d);
    ele_road_->lanes.lane_offsets.emplace_back(offset);
    curr_xml_offset = common::XmlNextSiblingElement(curr_xml_offset);
  }

  // lanes section
  // 1~*
  const tinyxml2::XMLElement* curr_xml_section =
      xml_lanes->FirstChildElement("laneSection");
  if (!curr_xml_section) {
    set_status(ErrorCode::XML_LANES_SECTION_ELEMENT_ERROR,
               "ROAD LANES SECTION ELEMENT IS NULL.");
    return *this;
  }
  size_t section_idx = 0;
  Status status{ErrorCode::OK, "ok"};
  RoadLanesSectionXmlParser section_parser{this->opendrive_version()};
  while (curr_xml_section) {
    element::LaneSection lane_section;
    lane_section.id = section_idx++;
    status = section_parser.Parse(curr_xml_section, &lane_section);
    CheckStatus(status);
    ele_road_->lanes.lane_sections.emplace_back(lane_section);
    curr_xml_section = common::XmlNextSiblingElement(curr_xml_section);
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::GenerateRoad() {
  if (!IsValid()) return *this;
  /// update section end posotion
  for (auto it = ele_road_->lanes.lane_sections.begin();
       it != ele_road_->lanes.lane_sections.end(); ++it) {
    if (it + 1 == ele_road_->lanes.lane_sections.end()) {  // 最后一个section
      it->s1 = ele_road_->attributes.length;
    } else {
      it->s1 = (it + 1)->s0;
    }
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
