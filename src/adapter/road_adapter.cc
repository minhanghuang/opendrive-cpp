#include "opendrive-cpp/adapter/road_adapter.h"

#include <sys/_types/_size_t.h>

#include <memory>
#include <string>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

RoadAdapter::RoadAdapter() {}

opendrive::Status RoadAdapter::Run(const element::Road* ele_road,
                                   core::Road::Ptr road_ptr) {
  ele_road_ = ele_road;
  road_ptr_ = road_ptr;
  if (!ele_road_ || !road_ptr_) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Input Is Null.");
    return status();
  }
  GenerateAttributes().GenerateSections().GenerateCenterLine();
  return status();
}

RoadAdapter& RoadAdapter::GenerateAttributes() {
  if (!IsValid()) return *this;
  if (-1 == ele_road_->attributes.id || ele_road_->attributes.length <= 0) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Road Attributes Execption.");
    return *this;
  }
  road_ptr_->id = std::to_string(ele_road_->attributes.id);
  road_ptr_->length = ele_road_->attributes.length;
  return *this;
}

RoadAdapter& RoadAdapter::GenerateSections() {
  if (!IsValid()) return *this;
  element::RoadTypeInfo road_type_info;
  size_t i = 0;

  for (const auto& ele_section : ele_road_->lanes.lane_sections) {
    core::Section::Ptr section = std::make_shared<core::Section>();
    road_ptr_->sections.emplace_back(section);
    section->id = road_ptr_->id + "_" + std::to_string(i++);
    section->start_position = ele_section.s0;
    section->length = ele_section.s1 - ele_section.s0;
    road_type_info = ele_road_->GetRoadTypeInfo(ele_section.s0);
    section->speed_limit = road_type_info.max_speed;

    /// left lanes
    for (const auto& ele_lane : ele_section.left.lanes) {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = section->id + "_" + std::to_string(ele_lane.attributes.id);
      lane->type = ele_lane.attributes.type;
      section->lanes.emplace_back(lane);
    }
    /// center lane
    if (1 != ele_section.center.lanes.size()) {
      set_status(ErrorCode::ADAPTER_SECTION_ERROR,
                 "Center Line Count Not Equal To 1.");
      return *this;
    } else {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = section->id + "_0";
      section->lanes.emplace_back(lane);
    }
    /// right lane
    for (const auto& ele_lane : ele_section.right.lanes) {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = section->id + "_" + std::to_string(ele_lane.attributes.id);
      lane->type = ele_lane.attributes.type;
      section->lanes.emplace_back(lane);
    }
  }
  return *this;
}

RoadAdapter& RoadAdapter::GenerateGeometry() {
  if (!IsValid()) return *this;
  for (const auto& geometry : ele_road_->plan_view.geometrys) {
  }
  return *this;
}

RoadAdapter& RoadAdapter::GenerateCenterLine() {
  if (!IsValid()) return *this;
  for (const auto& section : road_ptr_->sections) {
    for (const auto& lane : section->lanes) {
      if ("0" == common::Split(lane->id, "_").at(2)) {
        std::cout << "中线:" << lane->id << std::endl;
      }
    }
  }
  return *this;
}

RoadAdapter& RoadAdapter::GenerateSamples() {
  if (!IsValid()) return *this;

  return *this;
}

}  // namespace adapter
}  // namespace opendrive
