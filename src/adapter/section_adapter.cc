#include "opendrive-cpp/adapter/section_adapter.h"

namespace opendrive {
namespace adapter {

SectionAdapter::SectionAdapter() {}

void SectionAdapter::Init() {
  if (!odr_section_ || !section_ptr_) {
    set_status(ErrorCode::ADAPTER_SECTION_ERROR, "Input is null.");
  }
}

opendrive::Status SectionAdapter::Run(const g::LaneSection* odr_section,
                                      core::Section::Ptr section_ptr) {
  odr_section_ = odr_section;
  section_ptr_ = section_ptr;
  Init();
  return status();
}

}  // namespace adapter
}  // namespace opendrive
