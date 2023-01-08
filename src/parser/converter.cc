#include "opendrive-cpp/parser/converter.h"

namespace opendrive {
namespace converter {

ConverterMap::ConverterMap() {}

opendrive::Status ConverterMap::Start(element::Map::Ptr ele_map,
                                      core::Map::Ptr map_ptr, float step) {
  ele_map_ = ele_map;
  map_ptr_ = map_ptr;
  step_ = step;
  if (!ele_map_ || !map_ptr_ || step_ <= 0) {
    set_status(ErrorCode::ADAPTER_ROOT_ERROR, "MAP DATA IS NULL.");
    return status_;
  }
  set_status(ErrorCode::OK, "ok");
  Header().Junctions().Roads();
  return status_;
}

void ConverterMap::set_status(ErrorCode code, const std::string& msg) {
  status_.error_code = code;
  status_.msg = msg;
}

bool ConverterMap::IsValid() const {
  return ErrorCode::OK == status_.error_code;
}

ConverterMap& ConverterMap::Header() {
  if (!IsValid()) return *this;
  core::Header::Ptr header = std::make_shared<core::Header>();
  header->rev_major = ele_map_->header.rev_major;
  header->rev_minor = ele_map_->header.rev_minor;
  header->name = ele_map_->header.name;
  header->version = ele_map_->header.version;
  header->date = ele_map_->header.date;
  header->north = ele_map_->header.north;
  header->south = ele_map_->header.south;
  header->west = ele_map_->header.west;
  header->east = ele_map_->header.east;
  header->vendor = ele_map_->header.vendor;
  map_ptr_->header = header;
  return *this;
}

ConverterMap& ConverterMap::Junctions() {
  if (!IsValid()) return *this;
  for (const auto& ele_junction : ele_map_->junctions) {
    if (ele_junction.attributes.id < 0) {
      continue;
    }
    core::Junction::Ptr junction_ptr = std::make_shared<core::Junction>();
    JunctionAttributes(ele_junction, junction_ptr)
        .JunctionConnection(ele_junction, junction_ptr);
    map_ptr_->junctions[junction_ptr->id] = junction_ptr;
  }
  return *this;
}

ConverterMap& ConverterMap::JunctionAttributes(
    const element::Junction& ele_junction, core::Junction::Ptr junction_ptr) {
  if (!IsValid()) return *this;
  junction_ptr->id = std::to_string(ele_junction.attributes.id);
  junction_ptr->name = ele_junction.attributes.name;
  junction_ptr->main_road =
      -1 == ele_junction.attributes.main_road
          ? ""
          : std::to_string(ele_junction.attributes.main_road);
  junction_ptr->s_start = ele_junction.attributes.s_start;
  junction_ptr->s_end = ele_junction.attributes.s_end;
  junction_ptr->orientation = ele_junction.attributes.orientation;
  junction_ptr->type = ele_junction.attributes.type;
  return *this;
}

ConverterMap& ConverterMap::JunctionConnection(
    const element::Junction& ele_junction, core::Junction::Ptr junction_ptr) {
  if (!IsValid()) return *this;
  for (const auto& ele_conn : ele_junction.connections) {
    core::Junction::Connection conn;
    conn.id = std::to_string(ele_conn.id);
    conn.type = ele_conn.type;
    conn.incoming_road = std::to_string(ele_conn.incoming_road);
    conn.connecting_road = std::to_string(ele_conn.connecting_road);
    conn.link_id = conn.incoming_road + "_" + conn.connecting_road;
    conn.contact_point = ele_conn.contact_point;
    for (const auto& ele_lane_link : ele_conn.lane_links) {
      conn.lane_links.emplace_back(
          std::make_pair(std::to_string(ele_lane_link.from),
                         std::to_string(ele_lane_link.to)));
    }
    junction_ptr->connections[conn.link_id] = conn;
  }
  return *this;
}

ConverterMap& ConverterMap::Roads() {
  if (!IsValid()) return *this;
  for (const auto& ele_road : ele_map_->roads) {
    if (ele_road.attributes.id < 0) {
      continue;
    }
    core::Road::Ptr road_ptr = std::make_shared<core::Road>();
    RoadAttributes(ele_road, road_ptr).RoadSections(ele_road, road_ptr);
    map_ptr_->roads[road_ptr->id] = road_ptr;
  }
  return *this;
}

ConverterMap& ConverterMap::RoadAttributes(const element::Road& ele_road,
                                           core::Road::Ptr road_ptr) {
  if (!IsValid()) return *this;
  road_ptr->id = std::to_string(ele_road.attributes.id);
  road_ptr->name = ele_road.attributes.name;
  road_ptr->junction_id = -1 == ele_road.attributes.junction
                              ? ""
                              : std::to_string(ele_road.attributes.junction);
  road_ptr->length = ele_road.attributes.length;
  road_ptr->predecessor = -1 == ele_road.link.predecessor.id
                              ? ""
                              : std::to_string(ele_road.link.predecessor.id);
  road_ptr->successor = -1 == ele_road.link.successor.id
                            ? ""
                            : std::to_string(ele_road.link.successor.id);
  road_ptr->rule = ele_road.attributes.rule;
  road_ptr->predecessor_type = ele_road.link.predecessor.type;
  road_ptr->successor_type = ele_road.link.successor.type;
  road_ptr->predecessor_contact_point = ele_road.link.predecessor.contact_point;
  road_ptr->successor_contact_point = ele_road.link.successor.contact_point;
  return *this;
}

ConverterMap& ConverterMap::RoadSections(const element::Road& ele_road,
                                         core::Road::Ptr road_ptr) {
  if (!IsValid()) return *this;
  element::RoadTypeInfo road_type_info;
  double road_ds = 0.;
  for (const auto& ele_section : ele_road.lanes.lane_sections) {
    core::Section::Ptr section = std::make_shared<core::Section>();
    road_ptr->sections.emplace_back(section);
    section->id = std::to_string(ele_section.id);
    section->s0 = ele_section.s0;
    section->s1 = ele_section.s1;
    section->length = ele_section.s1 - ele_section.s0;
    int road_type_index =
        common::GetGEValuePoloy3(ele_road.type_info, ele_section.s0);
    if (road_type_index >= 0) {
      road_type_info = ele_road.type_info.at(road_type_index);
    }

    /// center lane
    if (1 != ele_section.center.lanes.size()) {
      set_status(ErrorCode::ADAPTER_SECTION_ERROR,
                 "Center Line Count Not Equal To 1.");
      return *this;
    } else {
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = "0";
      lane->length = section->length;
      lane->type = ele_section.center.lanes.front().attributes.type;
      section->center_lane = lane;
      this->SectionCenterLine(ele_road.plan_view.geometrys,
                              ele_road.lanes.lane_offsets, section, road_ds);
    }

    /// left lanes
    for (size_t i = 0; i < ele_section.left.lanes.size(); i++) {
      if (ele_section.id < 0) {
        continue;
      }
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = std::to_string(ele_section.left.lanes.at(i).attributes.id);
      lane->length = section->length;
      lane->type = ele_section.left.lanes.at(i).attributes.type;
      if (0 == i) {
        this->GenerateLaneSamples(ele_section.left.lanes.at(i), lane,
                                  section->center_lane->central_curve);
      } else {
        this->GenerateLaneSamples(
            ele_section.left.lanes.at(i), lane,
            section->left_lanes.back()->right_boundary.line);
        /// lane left right lane
        lane->left_neighbor = section->left_lanes.back()->id;
        section->left_lanes.back()->right_neighbor = lane->id;
      }
      section->left_lanes.emplace_back(lane);
    }

    /// right lanes
    for (size_t i = 0; i < ele_section.right.lanes.size(); i++) {
      if (ele_section.id < 0) {
        continue;
      }
      core::Lane::Ptr lane = std::make_shared<core::Lane>();
      lane->id = std::to_string(ele_section.right.lanes.at(i).attributes.id);
      lane->length = section->length;
      lane->type = ele_section.right.lanes.at(i).attributes.type;
      if (0 == i) {
        this->GenerateLaneSamples(ele_section.right.lanes.at(i), lane,
                                  section->center_lane->central_curve);
      } else {
        this->GenerateLaneSamples(
            ele_section.right.lanes.at(i), lane,
            section->right_lanes.back()->right_boundary.line);
        /// lane left right lane
        lane->left_neighbor = section->right_lanes.back()->id;
        section->right_lanes.back()->right_neighbor = lane->id;
      }
      section->right_lanes.emplace_back(lane);
    }
  }
  return *this;
}

void ConverterMap::SectionCenterLine(const element::Geometry::Ptrs& geometrys,
                                     const element::LaneOffsets& lane_offsets,
                                     core::Section::Ptr core_section,
                                     double& road_ds) {
  double section_ds = 0.;
  core_section->center_lane->central_curve.clear();
  element::Geometry::Ptr geometry = nullptr;
  element::Point refe_point;
  element::Point center_offset_point;
  core::Lane::Point center_point;
  double offset = 0.;
  bool last = false;
  // std::cout << "[debug] ---  " << core_section->id << std::endl;
  while (!last) {
    if (section_ds >= core_section->length) {
      section_ds = core_section->length;
      road_ds = std::fabs(core_section->s1 - 0.001);
      last = true;
    }
    geometry = this->GetGeometry(geometrys, road_ds);
    if (!geometry) {
      break;
    }
    refe_point = geometry->GetPoint(road_ds);
    offset = this->GetLaneOffsetValue(lane_offsets, road_ds);
    // std::cout << std::setprecision(15) << "[debug] offset:" << road_ds << ":"
    // << offset << std::endl;
    if (offset != 0) {
      center_offset_point = common::GetOffsetPoint(refe_point, offset);
      center_point.x = center_offset_point.x;
      center_point.y = center_offset_point.y;
      center_point.hdg = center_offset_point.hdg;
      center_point.s = section_ds;
    } else {
      center_point.x = refe_point.x;
      center_point.y = refe_point.y;
      center_point.hdg = refe_point.hdg;
      center_point.s = section_ds;
    }
    core_section->center_lane->central_curve.emplace_back(center_point);
    core_section->center_lane->left_boundary.line.emplace_back(center_point);
    core_section->center_lane->right_boundary.line.emplace_back(center_point);
    if (last) {
      road_ds = core_section->s1;
      break;
    } else {
      section_ds += step_;
      road_ds += step_;
    }
  }
}

element::Geometry::Ptr ConverterMap::GetGeometry(
    const element::Geometry::Ptrs& geometrys, double road_ds) {
  element::Geometry::Ptr geometry = nullptr;
  auto geometry_index = common::GetGTPtrPoloy3(geometrys, road_ds);
  if (geometry_index < 0) {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "GEOMETRY INDEX Execption.");
    return geometry;
  }
  element::Geometry::Ptr geometry_base = geometrys.at(geometry_index);
  if (nullptr == geometry_base) {
    set_status(ErrorCode::ADAPTER_GEOMETRY_ERROR, "GEOMETRY POINTER IS NULL.");
    return geometry;
  }

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
  return geometry;
}

double ConverterMap::GetLaneOffsetValue(const element::LaneOffsets& offsets,
                                        double road_ds) {
  int offset_idx = common::GetGEValuePoloy3(offsets, road_ds);
  if (offset_idx >= 0) {
    auto offset = offsets.at(offset_idx);
    return offset.GetOffsetValue(road_ds);
  }
  return 0;
}

void ConverterMap::GenerateLaneSamples(const element::Lane& ele_lane,
                                       core::Lane::Ptr core_lane,
                                       const core::Lane::Points& refe_line) {
  double lane_width = 0.;
  core::Lane::Point right_point;
  core::Lane::Point center_point;
  const int lane_direction = core_lane->id > "0" ? 1 : -1;
  for (const auto& refe_point : refe_line) {
    lane_width = ele_lane.GetLaneWidth(refe_point.s) * lane_direction;
    center_point = common::GetOffsetPoint(refe_point, lane_width / 2.0);
    right_point = common::GetOffsetPoint(refe_point, lane_width);
    core_lane->left_boundary.line.emplace_back(refe_point);
    core_lane->central_curve.emplace_back(center_point);
    core_lane->right_boundary.line.emplace_back(right_point);
  }
}

}  // namespace converter
}  // namespace opendrive
