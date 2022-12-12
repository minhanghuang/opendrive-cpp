#include "opendrive-cpp/adapter/junction_adapter.h"

#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

JunctionAdapter::JunctionAdapter() {}

opendrive::Status JunctionAdapter::Run(const element::Junction* ele_junction,
                                       core::Junction::Ptr junction_ptr) {
  ele_junction_ = ele_junction;
  junction_ptr_ = junction_ptr;
  if (!ele_junction_ || !junction_ptr_) {
    set_status(ErrorCode::ADAPTER_ROOT_ERROR, "Input Is Null.");
    return status();
  }
  return status();
}

}  // namespace adapter
}  // namespace opendrive
