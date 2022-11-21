#ifndef OPENDRIVE_CPP_ADAPTER_SECTION_H_
#define OPENDRIVE_CPP_ADAPTER_SECTION_H_

#include "opendrive-cpp/adapter/util_adapter.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

class SectionAdapter : public UtilAdapter {
 public:
  SectionAdapter();
  // opendrive::Status Run(
      // element::Geometry& ele_geometry,
      // const element::LaneSection* ele_section,
                        // core::Section::Ptr section_ptr);

 private:
  SectionAdapter& TransformCenterLane();
  SectionAdapter& TransformLeftLanes();
  SectionAdapter& TransformRightLanes();
  const element::LaneSection* ele_section_;
  // element::Geometry::Ptr ele_geometry_;
  core::Section::Ptr section_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_SECTION_H_
