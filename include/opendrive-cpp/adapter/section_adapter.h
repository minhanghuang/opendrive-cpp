#ifndef OPENDRIVE_CPP_ADAPTER_SECTION_H_
#define OPENDRIVE_CPP_ADAPTER_SECTION_H_

#include "opendrive-cpp/adapter/lane_adapter.h"
#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

class SectionAdapter : public UtilAdapter {
 public:
  SectionAdapter();
  opendrive::Status Run(const g::LaneSection* odr_section,
                        core::Section::Ptr section_ptr);

 private:
  virtual void Init() override;
  SectionAdapter& TransformLane();
  const g::LaneSection* odr_section_;
  core::Section::Ptr section_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_SECTION_H_
