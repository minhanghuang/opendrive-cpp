#include "opendrive-cpp/adapter/map_adapter.h"

#include <memory>

#include "opendrive-cpp/adapter/junction_adapter.h"
#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

MapAdapter::MapAdapter()
    : junction_adapter_(std::make_shared<JunctionAdapter>()),
      road_adapter_(std::make_shared<RoadAdapter>()) {}

opendrive::Status MapAdapter::Run(const element::Map* ele_map,
                                  core::Map::Ptr map_ptr) {
  ele_map_ = ele_map;
  map_ptr_ = map_ptr;
  if (!ele_map_ || !map_ptr_ || !road_adapter_) {
    set_status(ErrorCode::ADAPTER_ROOT_ERROR, "Input Is Null.");
    return status();
  }
  GenerateHeader().GenerateJunctions().GenerateRoads();
  return status();
}

MapAdapter& MapAdapter::GenerateHeader() {
  if (!IsValid()) return *this;
  core::Header::Ptr header = std::make_shared<core::Header>();
  header->rev_major = ele_map_->header.rev_major;
  header->rev_minor = ele_map_->header.rev_minor;
  header->name = ele_map_->header.name;
  header->version = ele_map_->header.version;
  header->date = ele_map_->header.date;
  header->north = ele_map_->header.north;
  header->south = ele_map_->header.south;
  header->west = ele_map_->header.west;
  header->east = ele_map_->header.east;
  header->vendor = ele_map_->header.vendor;
  map_ptr_->header = header;
  return *this;
}

MapAdapter& MapAdapter::GenerateJunctions() {
  if (!IsValid()) return *this;
  Status status{ErrorCode::OK, "ok"};

  for (const auto& ele_junction : ele_map_->junctions) {
    core::Junction::Ptr junction_ptr = std::make_shared<core::Junction>();
    status = junction_adapter_->Run(&ele_junction, junction_ptr);
    if (ErrorCode::OK != status.error_code) {
      CheckStatus(status);
      return *this;
    }
    if (0 == map_ptr_->junctions.count(junction_ptr->id)) {
      map_ptr_->junctions.insert({junction_ptr->id, junction_ptr});
    }
  }
  return *this;
}

MapAdapter& MapAdapter::GenerateRoads() {
  if (!IsValid()) return *this;
  Status status{ErrorCode::OK, "ok"};

  for (const auto& ele_road : ele_map_->roads) {
    core::Road::Ptr road_ptr = std::make_shared<core::Road>();
    status = road_adapter_->Run(&ele_road, road_ptr);
    if (ErrorCode::OK != status.error_code) {
      CheckStatus(status);
      return *this;
    }
    if (0 == map_ptr_->roads.count(road_ptr->id)) {
      map_ptr_->roads.insert({road_ptr->id, road_ptr});
    }
  }
  return *this;
}

}  // namespace adapter
}  // namespace opendrive
