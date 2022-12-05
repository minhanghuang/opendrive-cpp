#include "opendrive-cpp/adapter/road_adapter.h"

#include <iostream>
#include <memory>
#include <string>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"

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
  GenerateAttributes().GenerateSections().DebugCenterLine();
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
  double reference_line_ds = 0.;
  int geometry_index = -1;
  int geometry_index_curr = -1;
  size_t i = 0;

  for (const auto& ele_section : ele_road_->lanes.lane_sections) {
    core::Section::Ptr section = std::make_shared<core::Section>();
    road_ptr_->sections.emplace_back(section);
    geometry_index_curr = common::GetLeftPtrPoloy3(
        ele_road_->plan_view.geometrys, section->start_position);
    if (geometry_index_curr < 0) {
      set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR,
                 "GEOMETRY INDEX Execption.");
      return *this;
    }
    if (geometry_index != geometry_index_curr) {
      // new reference line
      std::cout << "[debug] update reference line" << std::endl;
      geometry_index = geometry_index_curr;
      reference_line_ds = 0.;
    }
    element::Geometry::Ptr geometry_base =
        ele_road_->plan_view.geometrys.at(geometry_index);
    if (nullptr == geometry_base) {
      set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR,
                 "GEOMETRY POINTER IS NULL.");
      return *this;
    }
    section->id = road_ptr_->id + "_" + std::to_string(i++);
    section->start_position = ele_section.s0;
    section->length = ele_section.s1 - ele_section.s0;
    road_type_info = ele_road_->GetRoadTypeInfo(ele_section.s0);
    section->speed_limit = road_type_info.max_speed;

    std::cout << "[debug] section id:" << section->id << std::endl;
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
      section->center_line = lane;
      this->GenerateCenterLine(section, geometry_base, reference_line_ds);
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

void RoadAdapter::GenerateCenterLine(core::Section::Ptr section,
                                     element::Geometry::Ptr geometry_base,
                                     double& reference_line_ds) {
  double section_ds = 0.;
  section->center_line->line.points.clear();

  /// laneoffset
  int laneoffset_index = common::GetLeftValuePoloy3(
      ele_road_->lanes.lane_offsets, section->start_position);
  element::LaneOffset lane_offset;
  if (laneoffset_index >= 0) {
    lane_offset = ele_road_->lanes.lane_offsets.at(laneoffset_index);
  }

  /// geometry
  if (GeometryType::LINE == geometry_base->type) {
    auto geometry =
        std::dynamic_pointer_cast<element::GeometryLine>(geometry_base);
    core::PointXD center_point;
    element::Point reference_point;
    element::Point lane_offset_point;
    double offset = 0.;
    while (section_ds <= section->length) {
      reference_point = geometry->GetPoint(reference_line_ds);
      if (laneoffset_index >= 0) {
        offset = lane_offset.GetOffset(section_ds);
        lane_offset_point = lane_offset.GetPoint(reference_point, offset);
        center_point.x = lane_offset_point.x;
        center_point.y = lane_offset_point.y;
        center_point.hdg = lane_offset_point.hdg;
      } else {
        center_point.x = reference_point.x;
        center_point.y = reference_point.y;
        center_point.hdg = reference_point.hdg;
      }
      section->center_line->line.points.emplace_back(center_point);
      section_ds += step();
      reference_line_ds += step();
    }
  } else if (GeometryType::ARC == geometry_base->type) {
    auto geometry =
        std::dynamic_pointer_cast<element::GeometryArc>(geometry_base);
  } else if (GeometryType::SPIRAL == geometry_base->type) {
    auto geometry =
        std::dynamic_pointer_cast<element::GeometrySpiral>(geometry_base);
  } else if (GeometryType::POLY3 == geometry_base->type) {
    auto geometry =
        std::dynamic_pointer_cast<element::GeometryPoly3>(geometry_base);
  } else if (GeometryType::PARAMPOLY3 == geometry_base->type) {
    auto geometry =
        std::dynamic_pointer_cast<element::GeometryParamPoly3>(geometry_base);
  }  // if geometry type
}

void RoadAdapter::DebugCenterLine() {
  for (const auto& section : road_ptr_->sections) {
    std::cout << "\ncenter id: " << section->center_line->id << std::endl;
    for (const auto& point : section->center_line->line.points) {
      std::cout << "[" << point.x << "," << point.y << "],";
    }
  }
}

}  // namespace adapter
}  // namespace opendrive
