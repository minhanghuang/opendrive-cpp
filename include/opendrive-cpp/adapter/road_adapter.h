#ifndef OPENDRIVE_CPP_ADAPTER_ROAD_H_
#define OPENDRIVE_CPP_ADAPTER_ROAD_H_

#include <memory>

#include "opendrive-cpp/adapter/geometry_adapter.h"
#include "opendrive-cpp/adapter/lane_adapter.h"
#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

class RoadAdapter : public UtilAdapter {
 public:
  RoadAdapter();
  opendrive::Status Run(const element::Road* ele_road,
                        core::Road::Ptr road_ptr);

 private:
  virtual void Init() override;
  RoadAdapter& TransformAttributes();
  RoadAdapter& TransformSection();
  RoadAdapter& TransformReferenceLine(size_t i);
  RoadAdapter& TransformSectionLanes(size_t i);
  std::shared_ptr<GeometryAdapter> geometry_adapter_ptr_;
  const element::Road* ele_road_;
  core::Road::Ptr road_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_ROAD_H_
