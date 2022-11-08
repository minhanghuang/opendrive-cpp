#include "opendrive-cpp/adapter/road_adapter.h"

namespace opendrive {
namespace adapter {

RoadAdapter::RoadAdapter() {}

void RoadAdapter::Init() {
  if (!odr_road_ || !road_ptr_) {
    set_status(ErrorCode::ADAPTER_SECTION_ERROR, "Input is null.");
  }
}

opendrive::Status RoadAdapter::Run(const g::Road* odr_road,
                                   core::Road::Ptr road_ptr) {
  odr_road_ = odr_road;
  road_ptr_ = road_ptr;
  Init();
  return status();
}

}  // namespace adapter
}  // namespace opendrive
