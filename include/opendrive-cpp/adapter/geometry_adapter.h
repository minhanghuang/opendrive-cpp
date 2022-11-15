#ifndef OPENDRIVE_CPP_ADAPTER_GEOMETRY_H_
#define OPENDRIVE_CPP_ADAPTER_GEOMETRY_H_

#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

class LineGeometryAdapter : public UtilAdapter {
 public:
  LineGeometryAdapter() = default;
  opendrive::Status Run(const element::Geometry::Ptr geometry_ptr,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
};

class SpiralGeometryAdapter : public UtilAdapter {
 public:
  SpiralGeometryAdapter() = default;
  opendrive::Status Run(const element::Geometry::Ptr geometry_ptr,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
};

class ArcGeometryAdapter : public UtilAdapter {
 public:
  ArcGeometryAdapter() = default;
  opendrive::Status Run(const element::Geometry::Ptr geometry_ptr,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
};

class Poly3GeometryAdapter : public UtilAdapter {
 public:
  Poly3GeometryAdapter() = default;
  opendrive::Status Run(const element::Geometry::Ptr geometry_ptr,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
};

class ParamPoly3GeometryAdapter : public UtilAdapter {
 public:
  ParamPoly3GeometryAdapter() = default;
  opendrive::Status Run(const element::Geometry::Ptr geometry_ptr,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
};

class GeometryAdapter : public UtilAdapter {
 public:
  GeometryAdapter();
  opendrive::Status Run(const element::Geometry::Ptr geometry,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
  LineGeometryAdapter line_adapter_;
  SpiralGeometryAdapter spiral_adapter_;
  ArcGeometryAdapter arc_adapter_;
  Poly3GeometryAdapter poly3_adapter_;
  ParamPoly3GeometryAdapter param_poly3_adapter_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_GEOMETRY_H_
