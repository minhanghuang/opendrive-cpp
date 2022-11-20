#include "opendrive-cpp/adapter/road_adapter.h"

#include <memory>
#include <string>

#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace adapter {

RoadAdapter::RoadAdapter() {}

opendrive::Status RoadAdapter::Run(const element::Road* ele_road,
                                   core::Road::Ptr road_ptr) {
  ele_road_ = ele_road;
  road_ptr_ = road_ptr;
  Init();
  TransformAttributes().TransformSection();
  return status();
}

void RoadAdapter::Init() {
  if (!ele_road_ || !road_ptr_) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Input Is Null.");
    return;
  }
}

RoadAdapter& RoadAdapter::TransformAttributes() {
  if (!IsValid()) return *this;
  if (-1 == ele_road_->attributes.id || ele_road_->attributes.length <= 0) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Road Attributes Execption.");
  }
  road_ptr_->id = std::to_string(ele_road_->attributes.id);
  road_ptr_->length = ele_road_->attributes.length;
  return *this;
}

RoadAdapter& RoadAdapter::TransformSection() {
  if (!IsValid()) return *this;
  if (ele_road_->lanes.lane_sections.size() !=
      ele_road_->plan_view.geometrys.size()) {
    set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Geometrys Size Execption.");
  }

  // size_t i = 0;
  // for (const auto& geometry : ele_road_->plan_view.geometrys) {
  //   core::Section::Ptr section = std::make_shared<core::Section>();
  //   road_ptr_->sections.emplace_back(section);  // add section to core
  //   section->id = road_ptr_->id + "_" + std::to_string(i);

    /// reference line
    // core::Lane::Ptr reference_line = std::make_shared<core::Lane>();
    // reference_line->id = section->id + "_0";
    // reference_line->length = geometry->length();
    // float num = 0.;
    // element::Vec3D ele_point;
    // if (GeometryType::LINE == geometry->type()) {
    //   auto ele_geometry =
    //       std::dynamic_pointer_cast<element::GeometryLine>(geometry);
    //   while (true) {
    //     /// reference line point
    //     core::PointXD point;
    //     ele_point = ele_geometry->GetXYH(num);
    //     point.s = geometry->s0;
    //     point.hdg = geometry->hdg0;
    //     point.x = ele_point.front();
    //     point.y = ele_point.back();
    //     reference_line->line.points.emplace_back(point);
    //     num += step();
    //     if (num > reference_line->length) {
    //       break;
    //     }
    //   }
    // } else if (GeometryType::ARC == geometry->type()) {
    //   auto ele_geometry =
    //       std::dynamic_pointer_cast<element::GeometryArc>(geometry);
    //   while (true) {
    //     /// reference line point
    //     core::PointXD point;
    //     ele_point = ele_geometry->GetXYH(num);
    //     point.s = geometry->sgeometry->s0();
    //     point.hdg = geometry->hdg0;
    //     point.x = ele_point.front();
    //     point.y = ele_point.back();
    //     reference_line->line.points.emplace_back(point);
    //     num += step();
    //     if (num > reference_line->length) {
    //       break;
    //     }
    //   }
    // } else if (GeometryType::SPIRAL == geometry->type()) {
    //   auto ele_geometry =
    //       std::dynamic_pointer_cast<element::GeometrySpiral>(geometry);
    //   while (true) {
    //     /// reference line point
    //     core::PointXD point;
    //     ele_point = ele_geometry->GetXYH(num);
    //     point.s = geometry->s0;
    //     point.hdg = geometry->hdg0;
    //     point.x = ele_point.front();
    //     point.y = ele_point.back();
    //     reference_line->line.points.emplace_back(point);
    //     num += step();
    //     if (num > reference_line->length) {
    //       break;
    //     }
    //   }
    // } else if (GeometryType::POLY3 == geometry->type()) {
    //   auto ele_geometry =
    //       std::dynamic_pointer_cast<element::GeometryPoly3>(geometry);
    //   while (true) {
    //     /// reference line point
    //     core::PointXD point;
    //     ele_point = ele_geometry->GetXYH(num);
    //     point.s = geometry->s0;
    //     point.hdg = geometry->hdg0;
    //     point.x = ele_point.front();
    //     point.y = ele_point.back();
    //     reference_line->line.points.emplace_back(point);
    //     num += step();
    //     if (num > reference_line->length) {
    //       break;
    //     }
    //   }
    // } else if (GeometryType::PARAMPOLY3 == geometry->type()) {
    //   auto ele_geometry =
    //       std::dynamic_pointer_cast<element::GeometryParamPoly3>(geometry);
    //   while (true) {
    //     /// reference line point
    //     core::PointXD point;
    //     ele_point = ele_geometry->GetXYH(num);
    //     point.s = geometry->s0;
    //     point.hdg = geometry->hdg0;
    //     point.x = ele_point.front();
    //     point.y = ele_point.back();
    //     reference_line->line.points.emplace_back(point);
    //     num += step();
    //     if (num > reference_line->length) {
    //       break;
    //     }
    //   }
    // } else {
    //   return *this;
    //   set_status(ErrorCode::ADAPTER_ROAD_ERROR, "Geometrys Type Execption.");
    // }
  // }
  return *this;
}

}  // namespace adapter
}  // namespace opendrive
