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
  std::cout << "Line" << std::endl;
  return status();
}

void SpiralGeometryAdapter::Init() {}
opendrive::Status SpiralGeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesSpiral> spiral_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesSpiral>(geometry_ptr);
  std::cout << "Spira" << std::endl;
  return status();
}

void ArcGeometryAdapter::Init() {}
opendrive::Status ArcGeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesArc> arc_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesArc>(geometry_ptr);
  std::cout << "ARC" << std::endl;
  return status();
}

void Poly3GeometryAdapter::Init() {}
opendrive::Status Poly3GeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesPoly3> poly3_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesPoly3>(geometry_ptr);
  std::cout << "Poly3" << std::endl;
  return status();
}

void ParamPoly3GeometryAdapter::Init() {}
opendrive::Status ParamPoly3GeometryAdapter::Run(
    const g::GeometryAttributes::Ptr geometry_ptr) {
  std::shared_ptr<g::GeometryAttributesParamPoly3> param_poly3_geometry_ptr =
      std::dynamic_pointer_cast<g::GeometryAttributesParamPoly3>(geometry_ptr);
  std::cout << "ParamPoly3" << std::endl;
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
