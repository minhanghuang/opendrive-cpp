#include "opendrive-cpp/adapter/map_adapter.h"

#include <memory>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

MapAdapter::MapAdapter() : road_adapter_(std::make_shared<RoadAdapter>()) {}

opendrive::Status MapAdapter::Run(const element::Map* ele_map,
                                  core::Map::Ptr map_ptr) {
  ele_map_ = ele_map;
  map_ptr_ = map_ptr;
  if (!ele_map_ || !map_ptr_ || !road_adapter_) {
    set_status(ErrorCode::ADAPTER_ROOT_ERROR, "Input Is Null.");
    return status();
  }
  TransformRoads();
  return status();
}

MapAdapter& MapAdapter::TransformRoads() {
  if (!IsValid()) return *this;
  Status status{ErrorCode::OK, "ok"};

  for (const auto& ele_road : ele_map_->roads) {
    core::Road::Ptr road_ptr = std::make_shared<core::Road>();
    status = road_adapter_->Run(&ele_road, road_ptr);
    if (ErrorCode::OK != status.error_code) {
      CheckStatus(status);
      return *this;
    }
    if (map_ptr_->roads.count(road_ptr->id) != 0) {
      map_ptr_->roads.insert({road_ptr->id, road_ptr});
    }
  }
  return *this;
}

}  // namespace adapter
}  // namespace opendrive
