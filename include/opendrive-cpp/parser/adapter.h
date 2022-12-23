#ifndef OPENDRIVE_CPP_ADAPTER_H_
#define OPENDRIVE_CPP_ADAPTER_H_

#include <cmath>
#include <mutex>
#include <unordered_map>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/road_parser.h"

namespace opendrive {
namespace adapter {

class AdapterMap {
 public:
  AdapterMap();
  opendrive::Status Start(const element::Map* ele_map, core::Map::Ptr map_ptr,
                          float step = 0.5);

 private:
  void set_status(ErrorCode code, const std::string& msg);
  bool IsValid();
  void Clean();
  AdapterMap& Header();
  AdapterMap& Junctions();
  AdapterMap& JunctionAttributes(const element::Junction* ele_junction,
                                 core::Junction::Ptr junction_ptr);
  AdapterMap& JunctionConnection(const element::Junction* ele_junction,
                                 core::Junction::Ptr junction_ptr);
  AdapterMap& Roads();
  AdapterMap& RoadAttributes(const element::Road* ele_road,
                             core::Road::Ptr road_ptr);
  AdapterMap& RoadSections(const element::Road* ele_road,
                           core::Road::Ptr road_ptr);
  void SectionCenterLine(const element::Geometry::Ptrs& geometrys,
                         const element::LaneOffsets& lane_offsets,
                         core::Section::Ptr core_section, double& road_ds);
  element::Geometry::Ptr GetGeometry(const element::Geometry::Ptrs& geometrys,
                                     double road_ds);
  void GenerateLaneSamples(const element::Lane& ele_lane,
                           core::Lane::Ptr core_lane,
                           const core::Line& reference_line);
  AdapterMap& GenerateTopo();
  void RoadLinkPreRoad(const element::Road& cur_ele_road,
                       element::Idx cur_section_idx,
                       const element::Lane& cur_ele_lane,
                       element::Idx pre_lane_idx);
  void RoadLinkSucRoad(const element::Road& cur_ele_road,
                       element::Idx cur_section_idx,
                       const element::Lane& cur_ele_lane,
                       element::Idx suc_lane_idx);
  void RoadLinkJunction();
  // void LinkDefaultJunction(const element::JunctionAttributes* ele_attr,
  //                          const element::JunctionConnections& ele_conns);
  float step_;
  opendrive::Status status_{ErrorCode::OK, "ok"};
  std::unordered_map<core::Id, core::Lane::Ptr> lanes_router_;
  const element::Map* ele_map_;
  core::Map::Ptr map_ptr_;
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_H_
