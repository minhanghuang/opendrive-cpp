#ifndef OPENDRIVE_CPP_ADAPTER_ROAD_H_
#define OPENDRIVE_CPP_ADAPTER_ROAD_H_ 

#include "opendrive-cpp/adapter/adapter.h"
#include "opendrive-cpp/adapter/lane_adapter.h"
#include "opendrive-cpp/core/base.h"

namespace opendrive {
namespace adapter {

class RoadAdapter : public AdapterBase {
 public:
  RoadAdapter();
  opendrive::Status Run(const base::Road* odr_road,
                        core::Section::Ptr section_ptr);

 private:
  virtual void Init() override;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_ROAD_H_ 
