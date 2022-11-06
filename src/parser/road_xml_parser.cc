#include "opendrive-cpp/parser/road_xml_parser.h"

#include <vector>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/core/types.hpp"
#include "opendrive-cpp/parser/lanes_xml_parser.h"

namespace opendrive {
namespace parser {

opendrive::Status RoadXmlParser::Parse(const tinyxml2::XMLElement* road_ele,
                                       core::Road::Ptr road_ptr) {
  road_ele_ = road_ele;
  road_ptr_ = road_ptr;
  Init();
  ParseAttributes()
      .ParseLinkEle()
      // .ParseTypeEle()
      .ParsePlanViewEle()
      // .ParseLanesEle();
      ;
  return status();
}

void RoadXmlParser::Init() {
  if (!road_ele_ || !road_ptr_) {
    set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR, "Input is null.");
  }
}

RoadXmlParser& RoadXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
  std::string name;
  std::string rule;
  double length;
  core::Id id;
  core::Id junction;
  common::XmlQueryStringAttribute(road_ele_, "name", name);
  common::XmlQueryStringAttribute(road_ele_, "rule", rule);
  common::XmlQueryDoubleAttribute(road_ele_, "length", length);
  common::XmlQueryIntAttribute(road_ele_, "id", id);
  common::XmlQueryIntAttribute(road_ele_, "junction", junction);
  road_ptr_->attributes.name = name;
  road_ptr_->attributes.length = length;
  road_ptr_->attributes.id = id;
  road_ptr_->attributes.junction = junction;
  if (common::StrEquals("RHT", rule)) {
    road_ptr_->attributes.rule = core::RoadAttributes::Rule::RHT;
  } else if (common::StrEquals("LHT", rule)) {
    road_ptr_->attributes.rule = core::RoadAttributes::Rule::LHT;
  }
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
        std::string link_type;
        core::Id link_id;
        std::string link_point;
        double link_s;
        std::string link_dir;
        common::XmlQueryStringAttribute(link_type_ele, "elementType",
                                        link_type);
        common::XmlQueryIntAttribute(link_type_ele, "elementId", link_id);
        common::XmlQueryStringAttribute(link_type_ele, "contactPoint",
                                        link_point);
        common::XmlQueryDoubleAttribute(link_type_ele, "elementS", link_s);
        common::XmlQueryStringAttribute(link_type_ele, "elementDir", link_dir);
        if ("predecessor" == link_element) {
          road_ptr_->link.predecessor.s = link_s;
          road_ptr_->link.predecessor.id = link_id;
          if (common::StrEquals("road", link_type)) {
            road_ptr_->link.predecessor.type = core::RoadLinkInfo::Type::ROAD;
          } else if (common::StrEquals("junction", link_type)) {
            road_ptr_->link.predecessor.type =
                core::RoadLinkInfo::Type::JUNCTION;
          }
          if (common::StrEquals("start", link_point)) {
            road_ptr_->link.predecessor.point_type =
                core::RoadLinkInfo::PointType::START;
          } else if (common::StrEquals("end", link_point)) {
            road_ptr_->link.predecessor.point_type =
                core::RoadLinkInfo::PointType::END;
          }
          if ("+" == link_dir) {
            road_ptr_->link.predecessor.dir = core::RoadLinkInfo::Dir::PLUS;
          } else if ("-" == link_dir) {
            road_ptr_->link.predecessor.dir = core::RoadLinkInfo::Dir::MINUS;
          }
        } else if ("successor" == link_element) {
          road_ptr_->link.successor.s = link_s;
          road_ptr_->link.successor.id = link_id;
          if (common::StrEquals("road", link_type)) {
            road_ptr_->link.successor.type = core::RoadLinkInfo::Type::ROAD;
          } else if (common::StrEquals("junction", link_type)) {
            road_ptr_->link.successor.type = core::RoadLinkInfo::Type::JUNCTION;
          }
          if (common::StrEquals("start", link_point)) {
            road_ptr_->link.successor.point_type =
                core::RoadLinkInfo::PointType::START;
          } else if (common::StrEquals("end", link_point)) {
            road_ptr_->link.successor.point_type =
                core::RoadLinkInfo::PointType::END;
          }
          if ("+" == link_dir) {
            road_ptr_->link.successor.dir = core::RoadLinkInfo::Dir::PLUS;
          } else if ("-" == link_dir) {
            road_ptr_->link.successor.dir = core::RoadLinkInfo::Dir::MINUS;
          }
        }
      }
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseTypeEle() {
  if (!IsValid()) return *this;
  const tinyxml2::XMLElement* type_ele = road_ele_->FirstChildElement("type");
  while (type_ele) {
    core::RoadTypeInfo road_type_info;
    double type_s;
    std::string road_type;
    std::string road_country;
    double road_max_speed;
    std::string road_speed_unit;
    common::XmlQueryDoubleAttribute(type_ele, "s", type_s);
    common::XmlQueryStringAttribute(type_ele, "type", road_type);
    common::XmlQueryStringAttribute(type_ele, "country", road_country);
    const tinyxml2::XMLElement* speed_ele =
        type_ele->FirstChildElement("speed");
    if (speed_ele) {
      common::XmlQueryDoubleAttribute(speed_ele, "max", road_max_speed);
      common::XmlQueryStringAttribute(speed_ele, "unit", road_speed_unit);
      road_type_info.max_speed = road_max_speed;
      if (common::StrEquals("m/s", road_speed_unit)) {
        road_type_info.speed_unit = core::RoadTypeInfo::SpeedUnit::MS;
      } else if (common::StrEquals("mph", road_speed_unit)) {
        road_type_info.speed_unit = core::RoadTypeInfo::SpeedUnit::MPH;
      } else if (common::StrEquals("km/h", road_speed_unit)) {
        road_type_info.speed_unit = core::RoadTypeInfo::SpeedUnit::KMH;
      }
    }
    road_type_info.s = type_s;
    road_type_info.country = road_country;
    if (common::StrEquals("unknown", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::UNKNOWN;
    } else if (common::StrEquals("rural", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::RURAL;
    } else if (common::StrEquals("motorway", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::MOTORWAY;
    } else if (common::StrEquals("town", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWN;
    } else if (common::StrEquals("lowSpeed", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::LOWSPEED;
    } else if (common::StrEquals("pedestrian", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::PEDESTRIAN;
    } else if (common::StrEquals("bicycle", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::BICYCLE;
    } else if (common::StrEquals("townExpressway", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNEXPRESSWAY;
    } else if (common::StrEquals("townCollector", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNCOLLECTOR;
    } else if (common::StrEquals("townArterial", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNARTERIAL;
    } else if (common::StrEquals("townPrivate", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNPRIVATE;
    } else if (common::StrEquals("townLocal", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNLOCAL;
    } else if (common::StrEquals("townPlayStreet", road_type)) {
      road_type_info.type = core::RoadTypeInfo::Type::TOWNPLAYSTREET;
    }
    road_ptr_->type_info.emplace_back(road_type_info);
    type_ele = common::XmlNextSiblingElement(type_ele);
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
      std::shared_ptr<core::GeometryAttributes> geometry_attributes_ptr;
      if (geometry_type_ele) {
        std::shared_ptr<core::GeometryAttributesLine> geometry_ptr =
            std::make_shared<core::GeometryAttributesLine>();
        geometry_ptr->type = core::GeometryAttributes::Type::LINE;
        geometry_attributes_ptr =
            std::dynamic_pointer_cast<core::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = planview_ele->FirstChildElement("arc");
      if (geometry_type_ele) {
        std::shared_ptr<core::GeometryAttributesArc> geometry_ptr =
            std::make_shared<core::GeometryAttributesArc>();
        double arc_curvature;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvature",
                                        arc_curvature);
        geometry_ptr->curvature = arc_curvature;
        geometry_ptr->type = core::GeometryAttributes::Type::ARC;
        geometry_attributes_ptr =
            std::dynamic_pointer_cast<core::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = planview_ele->FirstChildElement("spiral");
      if (geometry_type_ele) {
        std::shared_ptr<core::GeometryAttributesSpiral> geometry_ptr =
            std::make_shared<core::GeometryAttributesSpiral>();
        double spiral_curvstart;
        double spiral_curvend;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvStart",
                                        spiral_curvstart);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "curvEnd",
                                        spiral_curvend);
        geometry_ptr->curve_start = spiral_curvstart;
        geometry_ptr->curve_end = spiral_curvend;
        geometry_ptr->type = core::GeometryAttributes::Type::SPIRAL;
        geometry_attributes_ptr =
            std::dynamic_pointer_cast<core::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = planview_ele->FirstChildElement("poly3");
      if (geometry_type_ele) {
        std::shared_ptr<core::GeometryAttributesPoly3> geometry_ptr =
            std::make_shared<core::GeometryAttributesPoly3>();
        double poly3_a;
        double poly3_b;
        double poly3_c;
        double poly3_d;
        common::XmlQueryDoubleAttribute(geometry_type_ele, "a", poly3_a);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "b", poly3_b);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "c", poly3_c);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "d", poly3_d);
        geometry_ptr->a = poly3_a;
        geometry_ptr->b = poly3_b;
        geometry_ptr->c = poly3_c;
        geometry_ptr->d = poly3_d;
        geometry_ptr->type = core::GeometryAttributes::Type::POLY3;
        geometry_attributes_ptr =
            std::dynamic_pointer_cast<core::GeometryAttributes>(geometry_ptr);
      }
      geometry_type_ele = planview_ele->FirstChildElement("paramPoly3");
      if (geometry_type_ele) {
        std::shared_ptr<core::GeometryAttributesParamPoly3> geometry_ptr =
            std::make_shared<core::GeometryAttributesParamPoly3>();
        std::string param_poly3_p_range;
        double param_poly3_au;
        double param_poly3_bu;
        double param_poly3_cu;
        double param_poly3_du;
        double param_poly3_av;
        double param_poly3_bv;
        double param_poly3_cv;
        double param_poly3_dv;
        common::XmlQueryStringAttribute(geometry_type_ele, "pRange",
                                        param_poly3_p_range);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aU",
                                        param_poly3_au);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bU",
                                        param_poly3_bu);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cU",
                                        param_poly3_cu);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dU",
                                        param_poly3_du);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "aV",
                                        param_poly3_av);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "bV",
                                        param_poly3_bv);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "cV",
                                        param_poly3_cv);
        common::XmlQueryDoubleAttribute(geometry_type_ele, "dV",
                                        param_poly3_dv);
        if (common::StrEquals("arcLength", param_poly3_p_range)) {
          geometry_ptr->p_range =
              core::GeometryAttributesParamPoly3::PRange::ARCLENGTH;
        } else if (common::StrEquals("normalized", param_poly3_p_range)) {
          geometry_ptr->p_range =
              core::GeometryAttributesParamPoly3::PRange::NORMALIZED;
        }
        geometry_ptr->type = core::GeometryAttributes::Type::PARAMPOLY3;
        geometry_attributes_ptr =
            std::dynamic_pointer_cast<core::GeometryAttributes>(geometry_ptr);
        std::cout << "type: paramPoly3" << std::endl;
      }
      if (!geometry_attributes_ptr) {
        set_status(ErrorCode::XML_ROAD_ELEMENT_ERROR,
                   "Parse <geometry> Element Exception.");
        return *this;
      }
      double geometry_s;
      double geometry_x;
      double geometry_y;
      double geometry_z;
      double geometry_hdg;
      double geometry_length;
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "s", geometry_s);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "x", geometry_x);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "y", geometry_y);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "z", geometry_z);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "hdg", geometry_hdg);
      common::XmlQueryDoubleAttribute(curr_geometry_ele, "length",
                                      geometry_length);
      geometry_attributes_ptr->s = geometry_s;
      geometry_attributes_ptr->x = geometry_x;
      geometry_attributes_ptr->y = geometry_y;
      geometry_attributes_ptr->z = geometry_z;
      geometry_attributes_ptr->hdg = geometry_hdg;
      geometry_attributes_ptr->length = geometry_length;
      road_ptr_->plan_view.geometrys.emplace_back(geometry_attributes_ptr);
      curr_geometry_ele = common::XmlNextSiblingElement(curr_geometry_ele);
    }
  }
  return *this;
}

RoadXmlParser& RoadXmlParser::ParseLanesEle() {
  if (!IsValid()) return *this;
  core::Lanes::Ptr lanes_ptr = std::make_shared<core::Lanes>();
  road_ptr_->lanes = lanes_ptr;
  const tinyxml2::XMLElement* lanes_ele = road_ele_->FirstChildElement("lanes");
  LanesXmlParser lanes;
  lanes.Parse(lanes_ele, lanes_ptr);
  return *this;
}

}  // namespace parser
}  // namespace opendrive
