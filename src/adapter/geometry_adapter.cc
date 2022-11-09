#include "opendrive-cpp/adapter/geometry_adapter.h"

#include <memory>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/geometry/types.h"

namespace opendrive {
namespace adapter {

void LineGeometryAdapter::Init() {}
opendrive::Status LineGeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesLine> line_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesLine>(geometry_ptr);
  std::cout << "Line: "
            << "s:" << line_geometry_ptr->s << ", "
            << "x:" << line_geometry_ptr->x << ", "
            << "y:" << line_geometry_ptr->y << ", "
            << "z:" << line_geometry_ptr->z << ", "
            << "hdg:" << line_geometry_ptr->hdg << ", "
            << "length:" << line_geometry_ptr->length << "\n"
            << std::endl;
  return status();
}

void SpiralGeometryAdapter::Init() {}
opendrive::Status SpiralGeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesSpiral> spiral_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesSpiral>(geometry_ptr);
  std::cout << "Spira" << std::endl;
  std::cout << "Line: "
            << "s:" << spiral_geometry_ptr->s << ", "
            << "x:" << spiral_geometry_ptr->x << ", "
            << "y:" << spiral_geometry_ptr->y << ", "
            << "z:" << spiral_geometry_ptr->z << ", "
            << "hdg:" << spiral_geometry_ptr->hdg << ", "
            << "length:" << spiral_geometry_ptr->length << ", "
            << "curve_start:" << spiral_geometry_ptr->curve_start << ", "
            << "curve_end:" << spiral_geometry_ptr->curve_end << "\n"
            << std::endl;
  return status();
}

void ArcGeometryAdapter::Init() {}
opendrive::Status ArcGeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesArc> arc_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesArc>(geometry_ptr);
  std::cout << "ARC" << std::endl;
  std::cout << "Line: "
            << "s:" << arc_geometry_ptr->s << ", "
            << "x:" << arc_geometry_ptr->x << ", "
            << "y:" << arc_geometry_ptr->y << ", "
            << "z:" << arc_geometry_ptr->z << ", "
            << "hdg:" << arc_geometry_ptr->hdg << ", "
            << "length:" << arc_geometry_ptr->length << ", "
            << "curvature:" << arc_geometry_ptr->curvature << "\n"
            << std::endl;
  return status();
}

void Poly3GeometryAdapter::Init() {}
opendrive::Status Poly3GeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesPoly3> poly3_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesPoly3>(geometry_ptr);
  std::cout << "Poly3" << std::endl;
  std::cout << "Line: "
            << "s:" << poly3_geometry_ptr->s << ", "
            << "x:" << poly3_geometry_ptr->x << ", "
            << "y:" << poly3_geometry_ptr->y << ", "
            << "z:" << poly3_geometry_ptr->z << ", "
            << "hdg:" << poly3_geometry_ptr->hdg << ", "
            << "length:" << poly3_geometry_ptr->length << ", "
            << "a:" << poly3_geometry_ptr->a << ", "
            << "b:" << poly3_geometry_ptr->b << ", "
            << "c:" << poly3_geometry_ptr->c << ", "
            << "d:" << poly3_geometry_ptr->d << "\n"
            << std::endl;
  return status();
}

void ParamPoly3GeometryAdapter::Init() {}
opendrive::Status ParamPoly3GeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesParamPoly3> param_poly3_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesParamPoly3>(geometry_ptr);
  std::cout << "ParamPoly3" << std::endl;
  std::cout << "Line: "
            << "s:" << param_poly3_geometry_ptr->s << ", "
            << "x:" << param_poly3_geometry_ptr->x << ", "
            << "y:" << param_poly3_geometry_ptr->y << ", "
            << "z:" << param_poly3_geometry_ptr->z << ", "
            << "hdg:" << param_poly3_geometry_ptr->hdg << ", "
            << "length:" << param_poly3_geometry_ptr->length << ", "
            << "p_range:" << static_cast<int>(param_poly3_geometry_ptr->p_range)
            << ", "
            << "aU:" << param_poly3_geometry_ptr->aU << ", "
            << "bU:" << param_poly3_geometry_ptr->bU << ", "
            << "cU:" << param_poly3_geometry_ptr->cU << ", "
            << "dU:" << param_poly3_geometry_ptr->dU << ", "
            << "aV:" << param_poly3_geometry_ptr->aV << ", "
            << "bV:" << param_poly3_geometry_ptr->bV << ", "
            << "cV:" << param_poly3_geometry_ptr->cV << ", "
            << "dV:" << param_poly3_geometry_ptr->dV << "\n"
            << std::endl;
  return status();
}

GeometryAdapter::GeometryAdapter() {}
void GeometryAdapter::Init() {}
opendrive::Status GeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry, core::Road::Ptr road_ptr) {
  if (!geometry || !road_ptr) {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "input is null.");
    return status();
  }

  if (GeometryType::LINE == geometry->type) {
    line_geometry_adapter_.Run(geometry);
  } else if (GeometryType::SPIRAL == geometry->type) {
    spiral_geometry_adapter_.Run(geometry);
  } else if (GeometryType::ARC == geometry->type) {
    arc_geometry_adapter_.Run(geometry);
  } else if (GeometryType::POLY3 == geometry->type) {
    poly3_geometry_adapter_.Run(geometry);
  } else if (GeometryType::PARAMPOLY3 == geometry->type) {
    param_poly3_geometry_adapter_.Run(geometry);
  } else {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "adapter geometry fault.");
  }

  return status();
}

}  // namespace adapter
}  // namespace opendrive
