#include "opendrive-cpp/adapter/road_adapter.h"

namespace opendrive {
namespace adapter {

RoadAdapter::RoadAdapter()
    : geometry_ptr_(std::make_shared<GeometryAdapter>()) {}

opendrive::Status RoadAdapter::Run(const g::Road* g_road,
                                   core::Road::Ptr road_ptr) {
  g_road_ = g_road;
  road_ptr_ = road_ptr;
  Init();
  TransformGeometry().TransformSection();
  return status();
}

void RoadAdapter::Init() {
  if (!g_road_ || !road_ptr_) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Input is null.");
  }
  geometry_ptr_.reset(new GeometryAdapter());
}

RoadAdapter& RoadAdapter::TransformGeometry() {
  if (!IsValid()) return *this;
  for (const auto& geometry : g_road_->plan_view.geometrys) {
    geometry_ptr_->Run(geometry, road_ptr_);
  }
  return *this;
}

RoadAdapter& RoadAdapter::TransformSection() {
  if (!IsValid()) return *this;

  return *this;
}

}  // namespace adapter
}  // namespace opendrive
