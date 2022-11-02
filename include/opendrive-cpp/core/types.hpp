#ifndef OPENDRIVE_CPP_TYPES_HPP_
#define OPENDRIVE_CPP_TYPES_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace opendrive {
namespace core {

typedef int Id;
typedef std::vector<Id> Ids;
typedef std::string IdStr;
typedef std::string Name;

typedef struct Header HeaderTypedef;
struct Header {
  typedef std::shared_ptr<HeaderTypedef> Ptr;
  std::string rev_major;
  std::string rev_minor;
  std::string name;
  std::string version;
  std::string date;
  double north;
  double south;
  double west;
  double east;
  std::string vendor;
};

struct Point {
  double x = 0.;
  double y = 0.;
  double z = 0.;
};

/// Geometry Attributes
struct GeometryAttributes {
  enum class Type : std::uint8_t {
    NONE = 0,
    ARC = 1,
    LINE = 2,
    SPIRAL = 3,
    POLY3 = 4,
    PARAMPOLY3 = 5
  };
  Type type = Type::NONE;        // geometry type
  double length = 0.;            // length of the road section [meters]
  double start_position = 0.;    // s-offset [meters]
  double heading = 0.;           // start orientation [radians]
  double start_position_x = 0.;  // [meters]
  double start_position_y = 0.;  // [meters]
  double start_position_z = 0.;
};

struct GeometryAttributesArc : public GeometryAttributes {
  double curvature = 0.;
};

struct GeometryAttributesLine : public GeometryAttributes {};

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
  std::string p_range = "arcLength";
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
  enum class Type : std::uint8_t {
    NONE = 0,
    DRIVING = 1,
    STOP = 2,
    SHOULDER = 3,
    BIKING = 4,
    SIDEWALK = 5,
    BORDER = 6,
    RESTRICTED = 7,
    PARKING = 8,
    BIDIRECTIONAL = 9,
    MEDIAN = 10,
    SPECIAL1 = 11,
    SPECIAL2 = 12,
    SPECIAL3 = 13,
    ROADWORKS = 14,
    TRAM = 15,
    RAIL = 16,
    ENTRY = 17,
    EXIT = 18,
    OFFRAMP = 19,
    ONRAMP = 20
  };

  Id id = -1;
  Type type = Type::NONE;
  bool level = false;
};

struct OffsetPoly3 {
  // f(s) = a + b*s + c*s*s + d*s*s*s
  double start_offset = 0.;  // start position (s - start_offset)[meters]]
  double a = 0.;             // a - polynomial value at start_offset=0
  double b = 0.;             // b
  double c = 0.;             // c
  double d = 0.;             // d
};

struct RoadMark {
  enum class Type : std::uint8_t {
    NONE = 0,
    SOLID = 1,
    BROKEN = 2,
    SOLIDSOLID = 3,
    SOLIDBROKEN = 4,
    BROKENSOLID = 5,
    BROKENBROKEN = 6,
    BOTTSDOTS = 7,
    GRASS = 8,
    CURB = 9,
    CUSTOM = 10,
    EDGE = 11
  };
  enum class Color : std::uint8_t {
    STANDARD = 0,
    BLUE = 1,
    GREEN = 2,
    RED = 3,
    WHITE = 4,
    YELLOW = 5,
    ORANGE = 6
  };
  enum class Weight : std::uint8_t { STANDARD = 0, BOLD = 1 };
  enum class LaneChange : std::uint8_t {
    INCREASE = 0,
    DECREASE = 1,
    BOTH = 2,
    NONE = 3
  };

  double start_offset = 0.;
  Type type = Type::NONE;
  Color color = Color::STANDARD;
  double width = 0.;
  double height = 0.;
  Weight weigth = Weight::STANDARD;
  std::string material = "standard";
  LaneChange lane_change = LaneChange::NONE;
};

struct LaneWidth : public OffsetPoly3 {};

struct LaneLink {
  Id predecessor = -1;
  Id successor = -1;
};

struct Lane {
  LaneAttributes attributes;
  LaneLink link;
  std::vector<LaneWidth> width;
  RoadMark road_mark;
};

struct LaneInfo {
  std::vector<Lane> lanes;
};

struct LaneOffset : public OffsetPoly3 {};

struct LaneSection {
  double start_position = 0.;
  LaneInfo left, center, right;
};

struct Lanes {
  LaneOffset lane_offset;
  std::vector<LaneSection> lane_sections;
};

struct RoadAttributes {
  enum class Rule {
    RHT = 0,
    LHT = 1,
  };
  Name name;
  double length;
  Id id = -1;
  Id junction = -1;
  Rule rule = Rule::LHT;
};

struct RoadLinkInfo {
  enum class Type { ROAD = 0, JUNCTION = 1 };
  enum class Point { START = 0, END = 1 };
  enum class Dir { PLUS = 0, MINUS = 1 };

  Type type = Type::ROAD;
  Id id = -1;
  Point point = Point::START;
  double s = 0.;
  Dir dir = Dir::PLUS;
};

struct RoadLink {
  RoadLinkInfo predecessor;
  RoadLinkInfo successor;
};

struct RoadTypeInfo {
  enum class Type : std::uint8_t {
    UNKNOWN = 0,
    RURAL = 1,
    MOTORWAY = 2,
    TURAL = 3,
    TOWN = 4,
    LOWSPEED = 5,
    PEDESTRIAN = 6,
    BICYCLE = 7,
    TOWNEXPRESSWAY = 8,
    TOWNCOLLECTOR = 9,
    TOWNARTERIAL = 10,
    TOWNPRIVATE = 11,
    TOWNLOCAL = 12,
    TOWNPLAYSTREET = 13
  };
  enum class SpeedUnit {
    MS = 0,   // m/s
    MPH = 1,  // mph
    KMH = 2   // km/h
  };
  double s = 0.;
  Type type = Type::UNKNOWN;
  std::string country;
  double max_speed = 0.;
  SpeedUnit speed_unit = SpeedUnit::KMH;
};

struct RoadPlanView {
  std::vector<std::unique_ptr<GeometryAttributes>> geometrys;
};

typedef struct Road RoadTypedef;
struct Road {
  typedef std::shared_ptr<RoadTypedef> Ptr;
  RoadAttributes attributes;
  RoadLink link;
  std::vector<RoadTypeInfo> type_info;
  RoadPlanView plan_view;
  Lanes lanes;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_TYPES_HPP_
