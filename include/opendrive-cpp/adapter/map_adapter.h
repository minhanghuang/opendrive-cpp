#ifndef OPENDRIVE_CPP_ADAPTER_MAP_H_
#define OPENDRIVE_CPP_ADAPTER_MAP_H_

#include <memory>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

class MapAdapter : public UtilAdapter {
 public:
  MapAdapter();
  opendrive::Status Run(const element::Map* ele_map, core::Map::Ptr map_ptr);

 private:
  MapAdapter& TransformRoads();
  std::shared_ptr<RoadAdapter> road_adapter_;
  const element::Map* ele_map_;
  core::Map::Ptr map_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_MAP_H_
