#ifndef OPENDRIVE_CPP_ADAPTER_ROAD_H_
#define OPENDRIVE_CPP_ADAPTER_ROAD_H_

#include <memory>

#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

class RoadAdapter : public UtilAdapter {
 public:
  RoadAdapter();
  opendrive::Status Run(const element::Road* ele_road,
                        core::Road::Ptr road_ptr);

 private:
  RoadAdapter& GenerateAttributes();
  RoadAdapter& GenerateSections();
  RoadAdapter& GenerateGeometry();
  RoadAdapter& GenerateCenterLine();
  RoadAdapter& GenerateSamples();
  const element::Road* ele_road_;
  core::Road::Ptr road_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_ROAD_H_
