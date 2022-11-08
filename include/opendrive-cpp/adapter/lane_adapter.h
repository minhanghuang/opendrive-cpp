#ifndef OPENDRIVE_CPP_ADAPTER_LANE_H_
#define OPENDRIVE_CPP_ADAPTER_LANE_H_

#include "opendrive-cpp/adapter/util_adapter.h"

namespace opendrive {
namespace adapter {

class LaneAdapter : public UtilAdapter {
 public:
  LaneAdapter();
  opendrive::Status Run();

 private:
  virtual void Init() override;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_LANE_H_
