#include "opendrive-cpp/adapter/section_adapter.h"

#include <memory>

namespace opendrive {
namespace adapter {

SectionAdapter::SectionAdapter() {}

void SectionAdapter::Init() {
  // if (ele_geometry_ || !ele_section_ || !section_ptr_) {
  // set_status(ErrorCode::ADAPTER_SECTION_ERROR, "Input is null.");
  // }
}

// opendrive::Status SectionAdapter::Run(element::Geometry& ele_geometry,
                                      // const element::LaneSection* ele_section,
                                      // core::Section::Ptr section_ptr) {
  // ele_geometry_ = ele_geometry;
  // ele_section_ = ele_section;
  // section_ptr_ = section_ptr;
  // Init();
  // TransformCenterLane().TransformLeftLanes().TransformRightLanes();
  // return status();
// }

SectionAdapter& SectionAdapter::TransformCenterLane() {
  if (!IsValid()) return *this;
  if (ele_section_->center.lanes.empty()) {
    set_status(ErrorCode::ADAPTER_SECTION_ERROR,
               "section center line execption.");
    return *this;
  }
  auto reference_line = std::make_shared<core::Lane>();
  section_ptr_->reference_line = reference_line;

  return *this;
}

SectionAdapter& SectionAdapter::TransformLeftLanes() {
  if (!IsValid()) return *this;

  if (ele_section_->left.lanes.empty()) {
    return *this;
  }

  return *this;
}

SectionAdapter& SectionAdapter::TransformRightLanes() {
  if (!IsValid()) return *this;

  if (ele_section_->right.lanes.empty()) {
    return *this;
  }

  return *this;
}

}  // namespace adapter
}  // namespace opendrive
