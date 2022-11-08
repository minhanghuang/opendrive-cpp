#include "opendrive-cpp/adapter/section_adapter.h"

namespace opendrive {
namespace adapter {

SectionAdapter::SectionAdapter() {}

void SectionAdapter::Init() {}

opendrive::Status SectionAdapter::Run(const base::LaneSection* odr_section,
                                      core::Section::Ptr section_ptr) {
  return status();
}

}  // namespace adapter
}  // namespace opendrive
