#include "opendrive-cpp/adapter/road_adapter.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
  GenerateAttributes().GenerateSections();
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
  road_ptr_->predecessor = std::to_string(ele_road_->link.predecessor.id);
  road_ptr_->successor = std::to_string(ele_road_->link.successor.id);
  road_ptr_->predecessor_type = ele_road_->link.predecessor.type;
  road_ptr_->successor_type = ele_road_->link.successor.type;
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
    int road_type_index =
        common::GetLeftValuePoloy3(ele_road_->type_info, ele_section.s0);
    if (road_type_index >= 0) {
      road_type_info = ele_road_->type_info.at(road_type_index);
      section->speed_limit = road_type_info.max_speed;
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
    }
    this->GenerateCenterLine(section, geometry_base, reference_line_ds);

    /// left lanes
    for (size_t i = 0; i < ele_section.left.lanes.size(); i++) {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = section->id + "_" +
                 std::to_string(ele_section.left.lanes.at(i).attributes.id);
      lane->type = ele_section.left.lanes.at(i).attributes.type;
      this->GenerateLaneLink(lane, ele_section.left.lanes.at(i).link);
      if (0 == i) {
        this->GenerateLaneSamples(ele_section.left.lanes.at(i), lane,
                                  section->center_line->line);
      } else {
        this->GenerateLaneSamples(ele_section.left.lanes.at(i), lane,
                                  section->lanes.back()->right_boundary.line);
      }
      section->lanes.emplace_back(lane);
    }
    /// right lanes
    for (size_t i = 0; i < ele_section.right.lanes.size(); i++) {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = section->id + "_" +
                 std::to_string(ele_section.right.lanes.at(i).attributes.id);
      lane->type = ele_section.right.lanes.at(i).attributes.type;
      this->GenerateLaneLink(lane, ele_section.right.lanes.at(i).link);
      if (0 == i) {
        this->GenerateLaneSamples(ele_section.right.lanes.at(i), lane,
                                  section->center_line->line);
      } else {
        this->GenerateLaneSamples(ele_section.right.lanes.at(i), lane,
                                  section->lanes.back()->right_boundary.line);
      }
      section->lanes.emplace_back(lane);
    }
  }
  return *this;
}

void RoadAdapter::GenerateCenterLine(core::Section::Ptr core_section,
                                     element::Geometry::Ptr geometry_base,
                                     double& reference_line_ds) {
  double section_ds = 0.;
  core_section->center_line->line.points.clear();

  /// laneoffset
  int laneoffset_index = common::GetLeftValuePoloy3(
      ele_road_->lanes.lane_offsets, core_section->start_position);
  element::LaneOffset lane_offset;
  if (laneoffset_index >= 0) {
    lane_offset = ele_road_->lanes.lane_offsets.at(laneoffset_index);
  }

  /// geometry
  element::Geometry::Ptr geometry = nullptr;
  if (GeometryType::LINE == geometry_base->type) {
    geometry = std::dynamic_pointer_cast<element::GeometryLine>(geometry_base);
  } else if (GeometryType::ARC == geometry_base->type) {
    geometry = std::dynamic_pointer_cast<element::GeometryArc>(geometry_base);
  } else if (GeometryType::SPIRAL == geometry_base->type) {
    geometry =
        std::dynamic_pointer_cast<element::GeometrySpiral>(geometry_base);
  } else if (GeometryType::POLY3 == geometry_base->type) {
    geometry = std::dynamic_pointer_cast<element::GeometryPoly3>(geometry_base);
  } else if (GeometryType::PARAMPOLY3 == geometry_base->type) {
    geometry =
        std::dynamic_pointer_cast<element::GeometryParamPoly3>(geometry_base);
  }

  if (nullptr == geometry) {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "Geometry Pointer Is Null.");
    return;
  }
  core::PointXD center_point;
  element::Point reference_point;
  element::Point center_offset_point;
  double offset = 0.;
  while (section_ds <= core_section->length) {
    reference_point = geometry->GetPoint(reference_line_ds);
    if (laneoffset_index >= 0) {
      offset = lane_offset.GetOffset(section_ds);
      center_offset_point = common::GetOffsetPoint(reference_point, offset);
      center_point.x = center_offset_point.x;
      center_point.y = center_offset_point.y;
      center_point.hdg = center_offset_point.hdg;
      center_point.tangent = center_offset_point.tangent;
      center_point.s = section_ds;
    } else {
      center_point.x = reference_point.x;
      center_point.y = reference_point.y;
      center_point.hdg = reference_point.hdg;
      center_point.tangent = reference_point.tangent;
      center_point.s = section_ds;
    }
    core_section->center_line->line.points.emplace_back(center_point);
    section_ds += step();
    reference_line_ds += step();
  }
}

void RoadAdapter::GenerateLaneSamples(const element::Lane& ele_lane,
                                      core::Lane::Ptr core_lane,
                                      const core::Line& reference_line) {
  double lane_width = 0.;
  core::PointXD right_point;
  core::PointXD center_point;
  const int lane_direction =
      LaneDirection::LEFT == common::GetLaneDirection(core_lane->id) ? 1 : -1;
  size_t index = 0;
  for (const auto& reference_point : reference_line.points) {
    lane_width = ele_lane.GetLaneWidth(reference_point.s) * lane_direction;
    center_point = common::GetOffsetPoint(reference_point, lane_width / 2);
    right_point = common::GetOffsetPoint(reference_point, lane_width);
    core_lane->left_boundary.line.points.emplace_back(reference_point);
    core_lane->line.points.emplace_back(center_point);
    core_lane->right_boundary.line.points.emplace_back(right_point);
  }
}

void RoadAdapter::GenerateLaneLink(core::Lane::Ptr lane,
                                   const element::LaneLink& lane_link) {
  for (const auto& id : lane_link.predecessors) {
    lane->predecessors.emplace_back(std::to_string(id));
  }
  for (const auto& id : lane_link.successors) {
    lane->successors.emplace_back(std::to_string(id));
  }
}

}  // namespace adapter
}  // namespace opendrive
