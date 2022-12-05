#include "opendrive-cpp/parser/road_planview_parser.h"

#include "opendrive-cpp/common/common.hpp"

namespace opendrive {
namespace parser {

opendrive::Status RoadPlanViewXmlParser::Parse(
    const tinyxml2::XMLElement* xml_planview,
    element::RoadPlanView* ele_planview) {
  xml_planview_ = xml_planview;
  ele_planview_ = ele_planview;
  if (!xml_planview_ || !ele_planview_) {
    set_status(ErrorCode::XML_ROAD_PLANVIEW_ELEMENT_ERROR, "Input is null.");
    return status();
  }
  ParseAttributes();
  return status();
}

RoadPlanViewXmlParser& RoadPlanViewXmlParser::ParseAttributes() {
  if (!IsValid()) return *this;
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
      xml_planview_->FirstChildElement("geometry");
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
    ele_planview_->geometrys.emplace_back(geometry_base_ptr);
    curr_ele_geometry = common::XmlNextSiblingElement(curr_ele_geometry);
  }

  return *this;
}

}  // namespace parser
}  // namespace opendrive
