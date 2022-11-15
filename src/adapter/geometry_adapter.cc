#include "opendrive-cpp/adapter/geometry_adapter.h"

#include <cassert>
#include <memory>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {
namespace adapter {

void LineGeometryAdapter::Init() {}

opendrive::Status LineGeometryAdapter::Run(
    const element::Geometry::Ptr geometry_ptr, core::Road::Ptr road_ptr) {
  std::shared_ptr<element::GeometryLine> line =
      std::dynamic_pointer_cast<element::GeometryLine>(geometry_ptr);
  std::cout << "Line: "
            << "s:" << line->s() << ", "
            << "x:" << line->x() << ", "
            << "y:" << line->y() << ", "
            << "hdg:" << line->hdg() << ", "
            << "length:" << line->length() << "\n"
            << std::endl;
  assert(line->length() > 0);
  double i = 0.;
  while (true) {

    i += 0.5;
    if (i > line->length()) {
      break;
    }
  }
  return status();
}

void SpiralGeometryAdapter::Init() {}
opendrive::Status SpiralGeometryAdapter::Run(
    const element::Geometry::Ptr geometry_ptr, core::Road::Ptr road_ptr) {
  std::shared_ptr<element::GeometrySpiral> spiral =
      std::dynamic_pointer_cast<element::GeometrySpiral>(geometry_ptr);
  std::cout << "Spira" << std::endl;
  std::cout << "Line: "
            << "s:" << spiral->s() << ", "
            << "x:" << spiral->x() << ", "
            << "y:" << spiral->y() << ", "
            << "hdg:" << spiral->hdg() << ", "
            << "length:" << spiral->length() << ", "
            << "curve_start:" << spiral->curve_start() << ", "
            << "curve_end:" << spiral->curve_end() << "\n"
            << std::endl;
  return status();
}

void ArcGeometryAdapter::Init() {}
opendrive::Status ArcGeometryAdapter::Run(
    const element::Geometry::Ptr geometry_ptr, core::Road::Ptr road_ptr) {
  std::shared_ptr<element::GeometryArc> arc =
      std::dynamic_pointer_cast<element::GeometryArc>(geometry_ptr);
  std::cout << "ARC" << std::endl;
  std::cout << "Line: "
            << "s:" << arc->s() << ", "
            << "x:" << arc->x() << ", "
            << "y:" << arc->y() << ", "
            << "hdg:" << arc->hdg() << ", "
            << "length:" << arc->length() << ", "
            << "curvature:" << arc->curvature() << "\n"
            << std::endl;
  return status();
}

void Poly3GeometryAdapter::Init() {}
opendrive::Status Poly3GeometryAdapter::Run(
    const element::Geometry::Ptr geometry_ptr, core::Road::Ptr road_ptr) {
  std::shared_ptr<element::GeometryPoly3> poly3 =
      std::dynamic_pointer_cast<element::GeometryPoly3>(geometry_ptr);
  std::cout << "Poly3" << std::endl;
  std::cout << "Line: "
            << "s:" << poly3->s() << ", "
            << "x:" << poly3->x() << ", "
            << "y:" << poly3->y() << ", "
            << "hdg:" << poly3->hdg() << ", "
            << "length:" << poly3->length() << ", "
            << "a:" << poly3->a() << ", "
            << "b:" << poly3->b() << ", "
            << "c:" << poly3->c() << ", "
            << "d:" << poly3->d() << "\n"
            << std::endl;
  return status();
}

void ParamPoly3GeometryAdapter::Init() {}
opendrive::Status ParamPoly3GeometryAdapter::Run(
    const element::Geometry::Ptr geometry_ptr, core::Road::Ptr road_ptr) {
  std::shared_ptr<element::GeometryParamPoly3> param_poly3 =
      std::dynamic_pointer_cast<element::GeometryParamPoly3>(geometry_ptr);
  std::cout << "ParamPoly3" << std::endl;
  std::cout << "Line: "
            << "s:" << param_poly3->s() << ", "
            << "x:" << param_poly3->x() << ", "
            << "y:" << param_poly3->y() << ", "
            << "hdg:" << param_poly3->hdg() << ", "
            << "length:" << param_poly3->length() << ", "
            << "p_range:" << static_cast<int>(param_poly3->p_range()) << ", "
            << "aU:" << param_poly3->au() << ", "
            << "bU:" << param_poly3->bu() << ", "
            << "cU:" << param_poly3->cu() << ", "
            << "dU:" << param_poly3->du() << ", "
            << "aV:" << param_poly3->av() << ", "
            << "bV:" << param_poly3->bv() << ", "
            << "cV:" << param_poly3->cv() << ", "
            << "dV:" << param_poly3->dv() << "\n"
            << std::endl;
  return status();
}

GeometryAdapter::GeometryAdapter() {}
void GeometryAdapter::Init() {}
opendrive::Status GeometryAdapter::Run(const element::Geometry::Ptr geometry,
                                       core::Road::Ptr road_ptr) {
  if (!geometry || !road_ptr) {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "input is null.");
    return status();
  }

  if (GeometryType::LINE == geometry->type()) {
    line_adapter_.Run(geometry, road_ptr);
  } else if (GeometryType::SPIRAL == geometry->type()) {
    spiral_adapter_.Run(geometry, road_ptr);
  } else if (GeometryType::ARC == geometry->type()) {
    arc_adapter_.Run(geometry, road_ptr);
  } else if (GeometryType::POLY3 == geometry->type()) {
    poly3_adapter_.Run(geometry, road_ptr);
  } else if (GeometryType::PARAMPOLY3 == geometry->type()) {
    param_poly3_adapter_.Run(geometry, road_ptr);
  } else {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "adapter geometry fault.");
  }

  return status();
}

}  // namespace adapter
}  // namespace opendrive
