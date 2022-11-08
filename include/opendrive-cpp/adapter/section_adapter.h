#ifndef OPENDRIVE_CPP_ADAPTER_SECTION_H_
#define OPENDRIVE_CPP_ADAPTER_SECTION_H_

#include "opendrive-cpp/adapter/adapter.h"
#include "opendrive-cpp/adapter/lane_adapter.h"

namespace opendrive {
namespace adapter {

class SectionAdapter : public AdapterBase {
 public:
  SectionAdapter();
  opendrive::Status Run(const base::LaneSection* odr_section,
                        core::Section::Ptr section_ptr);

 private:
  virtual void Init() override;
  SectionAdapter& ParseLane();
  SectionAdapter& Parse();
  const base::LaneSection* odr_section_;
  core::Section::Ptr section_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_SECTION_H_
