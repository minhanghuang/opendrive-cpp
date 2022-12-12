#ifndef OPENDRIVE_CPP_ADAPTER_JUNCTION_H_
#define OPENDRIVE_CPP_ADAPTER_JUNCTION_H_

#include <memory>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

class JunctionAdapter : public UtilAdapter {
 public:
  JunctionAdapter();
  opendrive::Status Run(const element::Junction* ele_junction,
                        core::Junction::Ptr junction_ptr);

 private:
  const element::Junction* ele_junction_;
  core::Junction::Ptr junction_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_JUNCTION_H_
