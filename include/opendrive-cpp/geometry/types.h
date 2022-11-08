#ifndef OPENDRIVE_CPP_TYPES_H_
#define OPENDRIVE_CPP_TYPES_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "enums.h"

namespace opendrive {
namespace g {

typedef int Id;
typedef std::vector<Id> Ids;
typedef std::string IdStr;
typedef std::string Name;

struct Header {
  std::string rev_major;
  std::string rev_minor;
  std::string name;
  std::string version;
  std::string date;
  double north = 0.;
  double south = 0.;
  double west = 0.;
  double east = 0.;
  std::string vendor;
};

/// Geometry Attributes
struct GeometryAttributes {
  virtual ~GeometryAttributes() = default;
  double s = 0.;  // s-offset [meters]
  double x = 0.;  // [meters]
  double y = 0.;  // [meters]
  double z = 0.;
  double hdg = 0.;     // start orientation [radians]
  double length = 0.;  // length of the road section [meters]
  GeometryType type = GeometryType::UNKNOWN;  // geometry type
};

struct GeometryAttributesLine : public GeometryAttributes {};

struct GeometryAttributesArc : public GeometryAttributes {
  double curvature = 0.;
};

struct GeometryAttributesSpiral : public GeometryAttributes {
  double curve_start = 0.;
  double curve_end = 0.;
};

struct GeometryAttributesPoly3 : public GeometryAttributes {
  double a = 0.;
  double b = 0.;
  double c = 0.;
  double d = 0.;
};

struct GeometryAttributesParamPoly3 : public GeometryAttributes {
  enum class PRange : std::uint8_t {
    UNKNOWN = 0,
    ARCLENGTH = 1,
    NORMALIZED = 2
  };
  PRange p_range = PRange::UNKNOWN;
  double aU = 0.;
  double bU = 0.;
  double cU = 0.;
  double dU = 0.;
  double aV = 0.;
  double bV = 0.;
  double cV = 0.;
  double dV = 0.;
};

struct LaneAttributes {
  Id id = -1;
  LaneType type = LaneType::UNKNOWN;
  Boolean level = Boolean::UNKNOWN;
};

struct OffsetPoly3 {
  // f(s) = a + b*s + c*s*s + d*s*s*s
  double s = 0.;  // start position (s - start_offset)[meters]]
  double a = 0.;  // a - polynomial value at start_offset=0
  double b = 0.;  // b
  double c = 0.;  // c
  double d = 0.;  // d
};

struct RoadMark {
  double s = 0.;
  RoadMarkType type = RoadMarkType::UNKNOWN;
  RoadMarkColor color = RoadMarkColor::UNKNOWN;
  RoadMarkWeight weigth = RoadMarkWeight::UNKNOWN;
  double width = 0.;
  double height = 0.;
  std::string material = "standard";
  RoadMarkLaneChange lane_change = RoadMarkLaneChange::UNKNOWN;
};

struct LaneWidth : public OffsetPoly3 {};

struct LaneBorder : public OffsetPoly3 {};

struct LaneLink {
  Id predecessor = 0;
  Id successor = 0;
};

struct Lane {
  LaneAttributes attributes;
  LaneLink link;
  std::vector<LaneWidth> widths;
  std::vector<LaneBorder> borders;
  std::vector<RoadMark> road_marks;
};

struct LanesInfo {
  std::vector<Lane> lanes;
};

struct LaneOffset : public OffsetPoly3 {};

struct LaneSection {
  double s = 0.;
  LanesInfo left, center, right;
};

struct Lanes {
  std::vector<LaneOffset> lane_offsets;
  std::vector<LaneSection> lane_sections;
};

struct RoadAttributes {
  Name name;
  double length;
  Id id = -1;
  Id junction = -1;
  RoadRule rule = RoadRule::UNKNOWN;
};

struct RoadLinkInfo {
  enum class Type { UNKNOWN = 0, ROAD = 1, JUNCTION = 2 };
  enum class PointType { UNKNOWN = 0, START = 1, END = 2 };
  enum class Dir { UNKNOWN = 0, PLUS = 1, MINUS = 2 };
  Id id = -1;
  double s = 0.;
  Type type = Type::UNKNOWN;
  PointType point_type = PointType::UNKNOWN;
  Dir dir = Dir::UNKNOWN;
};

struct RoadLink {
  RoadLinkInfo predecessor;
  RoadLinkInfo successor;
};

struct RoadTypeInfo {
  double s = 0.;
  RoadType type = RoadType::UNKNOWN;
  std::string country;
  double max_speed = 0.;
  RoadSpeedUnit speed_unit = RoadSpeedUnit::UNKNOWN;
};

struct RoadPlanView {
  std::vector<std::shared_ptr<GeometryAttributes>> geometrys;
};

struct Road {
  RoadAttributes attributes;
  RoadLink link;
  std::vector<RoadTypeInfo> type_info;
  RoadPlanView plan_view;
  Lanes lanes;
};

typedef struct Map MapType;
struct Map {
  typedef std::shared_ptr<MapType> Ptr;
  Header header;
  std::vector<Road> roads;
};

}  // namespace g
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_TYPES_H_
