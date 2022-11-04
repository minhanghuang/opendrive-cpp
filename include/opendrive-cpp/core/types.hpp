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

enum class Boolean { NONE = -1, FALSE = 0, TRUE = 1 };

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
  double s = 0.;  // s-offset [meters]
  double x = 0.;  // [meters]
  double y = 0.;  // [meters]
  double z = 0.;
  double hdg = 0.;         // start orientation [radians]
  double length = 0.;      // length of the road section [meters]
  Type type = Type::NONE;  // geometry type
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
  enum class PRange : std::uint8_t { NONE = 0, ARCLENGTH = 1, NORMALIZED = 2 };
  PRange p_range = PRange::NONE;
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
    SHOULDER = 0,
    BORDER = 1,
    DRIVING = 2,
    STOP = 3,
    NONE = 4,
    RESTRICTED = 5,
    PARKING = 6,
    MEDIAN = 7,
    BIKING = 8,
    SIDEWALK = 9,
    CURB = 10,
    EXIT = 11,
    ENTRY = 12,
    ONRAMP = 13,
    OFFRAMP = 14,
    CONNECTINGRAMP = 15,
    BIDIRECTIONAL = 16,
    SPECIAL1 = 17,
    SPECIAL2 = 18,
    SPECIAL3 = 19,
    ROADWORKS = 20,
    TRAM = 21,
    RAIL = 22,
    BUS = 23,
    TAXI = 24,
    HOV = 25,
    MWYENTRY = 26,
    MWYEXIT = 27
  };

  Id id = -1;
  Type type = Type::NONE;
  Boolean level = Boolean::NONE;
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
    NONE = 0,
    STANDARD = 1,
    BLUE = 2,
    GREEN = 3,
    RED = 4,
    WHITE = 5,
    YELLOW = 6,
    ORANGE = 7
  };
  enum class Weight : std::uint8_t { NONE = 0, STANDARD = 1, BOLD = 2 };
  enum class LaneChange : std::uint8_t {
    NONE = 0,
    INCREASE = 1,
    DECREASE = 2,
    BOTH = 3,
  };

  double s = 0.;
  Type type = Type::NONE;
  Color color = Color::NONE;
  Weight weigth = Weight::NONE;
  double width = 0.;
  double height = 0.;
  std::string material = "standard";
  LaneChange lane_change = LaneChange::NONE;
};

struct LaneWidth : public OffsetPoly3 {};

struct LaneBorder : public OffsetPoly3 {};

struct LaneLink {
  Id predecessor = -1;
  Id successor = -1;
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

typedef struct Lanes LanesTypedef;
struct Lanes {
  typedef std::shared_ptr<LanesTypedef> Ptr;
  std::vector<LaneOffset> lane_offsets;
  std::vector<LaneSection> lane_sections;
};

struct RoadAttributes {
  enum class Rule { NONE = 0, RHT = 1, LHT = 2 };
  Name name;
  double length;
  Id id = -1;
  Id junction = -1;
  Rule rule = Rule::NONE;
};

struct RoadLinkInfo {
  enum class Type { NONE = 0, ROAD = 1, JUNCTION = 2 };
  enum class Point { NONE = 0, START = 1, END = 2 };
  enum class Dir { NONE = 0, PLUS = 1, MINUS = 2 };

  Type type = Type::NONE;
  Id id = -1;
  Point point = Point::NONE;
  double s = 0.;
  Dir dir = Dir::NONE;
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
    NONE = 0,
    MS = 1,   // m/s
    MPH = 2,  // mph
    KMH = 3   // km/h
  };
  double s = 0.;
  Type type = Type::UNKNOWN;
  std::string country;
  double max_speed = 0.;
  SpeedUnit speed_unit = SpeedUnit::NONE;
};

struct RoadPlanView {
  std::vector<std::shared_ptr<GeometryAttributes>> geometrys;
};

typedef struct Road RoadTypedef;
struct Road {
  typedef std::shared_ptr<RoadTypedef> Ptr;
  RoadAttributes attributes;
  RoadLink link;
  std::vector<RoadTypeInfo> type_info;
  RoadPlanView plan_view;
  Lanes::Ptr lanes;
};

typedef struct Map MapTypedef;
struct Map {
  typedef std::shared_ptr<MapTypedef> Ptr;
  Header::Ptr header;
  std::vector<Road::Ptr> roads;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_TYPES_HPP_
