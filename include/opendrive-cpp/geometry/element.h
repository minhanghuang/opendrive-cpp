#ifndef OPENDRIVE_CPP_TYPES_H_
#define OPENDRIVE_CPP_TYPES_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "enums.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/spiral/odrSpiral.h"

namespace opendrive {
namespace element {

typedef int Id;
typedef Id Idx;
typedef std::vector<Id> Ids;
typedef std::string IdStr;
typedef std::string Name;

struct Point {
  double x = 0.;
  double y = 0.;
  double hdg = 0.;
};

struct Header {
  std::string rev_major;
  std::string rev_minor;
  std::string version;
  std::string name;
  std::string date;
  double north = 0.;
  double south = 0.;
  double west = 0.;
  double east = 0.;
  std::string vendor;
};

typedef struct Geometry GeometryTypedef;
class Geometry {
 public:
  typedef std::shared_ptr<GeometryTypedef> Ptr;
  typedef std::vector<Ptr> Ptrs;
  Geometry(double _s, double _x, double _y, double _hdg, double _length,
           GeometryType _type)
      : s(_s),
        x(_x),
        y(_y),
        hdg(_hdg),
        length(_length),
        type(_type),
        sin_hdg(std::sin(_hdg)),
        cos_hdg(std::cos(_hdg)) {}
  virtual ~Geometry() = default;
  virtual Point GetPoint(double ref_line_ds) const = 0;
  const double s;
  const double x;
  const double y;
  const double hdg;
  const double length;
  const GeometryType type;
  const double sin_hdg;
  const double cos_hdg;
};

class GeometryLine final : public Geometry {
 public:
  GeometryLine(double _s, double _x, double _y, double _hdg, double _length,
               GeometryType _type)
      : Geometry(_s, _x, _y, _hdg, _length, _type) {}
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s;
    const double xd = x + (cos_hdg * ref_line_ds);
    const double yd = y + (sin_hdg * ref_line_ds);
    return Point{.x = xd, .y = yd, .hdg = hdg};
  }
};

class GeometryArc final : public Geometry {
 public:
  GeometryArc(double _s, double _x, double _y, double _hdg, double _length,
              GeometryType _type, double _curvature)
      : Geometry(_s, _x, _y, _hdg, _length, _type),
        curvature(_curvature),
        radius(1.0 / _curvature) {}
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s;
    const double angle_at_s = ref_line_ds * curvature - M_PI / 2;
    const double xd = radius * (std::cos(hdg + angle_at_s) - sin_hdg) + x;
    const double yd = radius * (std::sin(hdg + angle_at_s) + cos_hdg) + y;
    const double tangent = hdg + ref_line_ds * curvature;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  const double curvature;
  const double radius;
};

class GeometrySpiral final : public Geometry {
 public:
  GeometrySpiral(double _s, double _x, double _y, double _hdg, double _length,
                 GeometryType _type, double _curve_start, double _curve_end)
      : Geometry(_s, _x, _y, _hdg, _length, _type),
        curve_start(_curve_start),
        curve_end(_curve_end),
        curve_dot((_curve_end - _curve_start) / (_length)) {}

  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s;
    const double s1 = curve_start / curve_dot + ref_line_ds;
    double x1;
    double y1;
    double t1;
    odrSpiral(s1, curve_dot, &x1, &y1, &t1);

    const double s0 = curve_start / curve_dot;
    double x0;
    double y0;
    double t0;
    odrSpiral(s1, curve_dot, &x0, &y0, &t0);

    x1 -= x0;
    y1 -= y0;
    t1 -= t0;
    const double angle = hdg - t0;
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    const double xd = x + x1 * cos_a - y1 * sin_a;
    const double yd = y + y1 * cos_a + x1 * sin_a;
    const double tangent = hdg + t1;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  const double curve_start;
  const double curve_end;
  const double curve_dot;
};

class GeometryPoly3 final : public Geometry {
 public:
  GeometryPoly3(double _s, double _x, double _y, double _hdg, double _length,
                GeometryType _type, double _a, double _b, double _c, double _d)
      : Geometry(_s, _x, _y, _hdg, _length, _type),
        a(_a),
        b(_b),
        c(_c),
        d(_d) {}

  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s;
    const double u = ref_line_ds;
    const double v = a + b * u + c * std::pow(u, 2) + d * std::pow(u, 3);
    const double x1 = u * cos_hdg - v * sin_hdg;
    const double y1 = u * sin_hdg + v * cos_hdg;
    const double tangent_v = b + 2.0 * c * u + 3.0 * d * std::pow(u, 2);
    const double theta = std::atan2(tangent_v, 1.0);
    const double xd = x + x1;
    const double yd = y + y1;
    const double tangent = hdg + theta;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  const double a;
  const double b;
  const double c;
  const double d;
};

class GeometryParamPoly3 final : public Geometry {
 public:
  enum class PRange : std::uint8_t {
    UNKNOWN = 0,
    ARCLENGTH = 1,
    NORMALIZED = 2
  };
  GeometryParamPoly3(double _s, double _x, double _y, double _hdg,
                     double _length, GeometryType _type, double _au, double _bu,
                     double _cu, double _du, double _av, double _bv, double _cv,
                     double _dv, PRange _p_range)
      : Geometry(_s, _x, _y, _hdg, _length, _type),
        au(_au),
        bu(_bu),
        cu(_cu),
        du(_du),
        av(_av),
        bv(_bv),
        cv(_cv),
        dv(_dv),
        p_range(_p_range) {}
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s;
    double p = ref_line_ds;
    if (PRange::NORMALIZED == p_range) {
      p = std::min(1.0, ref_line_ds / length);
    }
    const double u = au + bu * p + cu * std::pow(p, 2) + du * std::pow(p, 3);
    const double v = av + bv * p + cv * std::pow(p, 2) + dv * std::pow(p, 3);
    const double x1 = u * cos_hdg - v * sin_hdg;
    const double y1 = u * sin_hdg + v * cos_hdg;
    const double tangent_u = bu + 2 * cu * p + 3 * du * std::pow(p, 2);
    const double tangent_v = bv + 2 * cv * p + 3 * dv * std::pow(p, 2);
    const double theta = std::atan2(tangent_v, tangent_u);
    const double xd = x + x1;
    const double yd = y + y1;
    const double tangent = hdg + theta;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  const PRange p_range;
  const double au;
  const double bu;
  const double cu;
  const double du;
  const double av;
  const double bv;
  const double cv;
  const double dv;
};

struct LaneAttributes {
  Id id = std::numeric_limits<Id>::max();
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
  bool operator<(const OffsetPoly3& obj) const { return this->s > obj.s; }
  virtual double GetOffset(double section_ds) const final {
    return a + b * section_ds + c * std::pow(section_ds, 2) +
           d * std::pow(section_ds, 3);
  }
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
typedef std::vector<RoadMark> RoadMarks;

struct LaneWidth : public OffsetPoly3 {};
typedef std::vector<LaneWidth> LaneWidths;

struct LaneBorder : public OffsetPoly3 {};
typedef std::vector<LaneBorder> LaneBorders;

struct LaneLink {
  Ids predecessors;
  Ids successors;
};
typedef std::vector<LaneLink> LaneLinks;

struct LaneSpeed {
  double s = 0.;
  float max = 0.;
  SpeedUnit unit = SpeedUnit::UNKNOWN;
};
typedef std::vector<LaneSpeed> LaneSpeeds;

struct Lane {
  LaneAttributes attributes;
  LaneLink link;
  LaneWidths widths;
  LaneBorders borders;
  RoadMarks road_marks;
  LaneSpeeds max_speeds;
  double GetLaneWidth(double ds) const {
    // width >> border
    if (ds < 0) {
      return GetLaneWidth(0.);
    }
    if (widths.empty()) {
      if (borders.empty()) {
        return 0.;
      }
      /// border
      int border_index = common::GetLeftValuePoloy3(borders, ds);
      if (border_index < 0) {
        return 0.;
      }
      return borders.at(border_index).GetOffset(ds);
    } else {
      /// width
      int width_index = common::GetLeftValuePoloy3(widths, ds);
      if (width_index < 0) {
        return 0.;
      }
      return widths.at(width_index).GetOffset(ds);
    }
    return 0.;
  }
};

struct LanesInfo {
  std::vector<Lane> lanes;
};

struct LaneOffset : public OffsetPoly3 {};
typedef std::vector<LaneOffset> LaneOffsets;

struct LaneSection {
  Id id = -1;      // [>=0] (extended)
  double s0 = 0.;  // start position
  double s1 = 0.;  // end position(extended)
  LanesInfo left, center, right;
};
typedef std::vector<LaneSection> LaneSections;

struct Lanes {
  LaneOffsets lane_offsets;
  LaneSections lane_sections;
};

struct RoadAttributes {
  Name name;
  Id id = -1;        // [>=0]
  Id junction = -1;  // -1: Road; other: junction road
  double length = 0.;
  RoadRule rule = RoadRule::RHT;
};

struct RoadLinkInfo {
  Id id = -1;     // [>=0]
  double s = -1;  // [>=0]
  RoadLinkType type = RoadLinkType::UNKNOWN;
  ContactPointType contact_point = ContactPointType::UNKNOWN;
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
  float max_speed = 0.;
  SpeedUnit speed_unit = SpeedUnit::UNKNOWN;
};

struct RoadPlanView {
  Geometry::Ptrs geometrys;
};

struct Road {
  RoadAttributes attributes;
  RoadLink link;
  std::vector<RoadTypeInfo> type_info;
  RoadPlanView plan_view;
  Lanes lanes;
};

struct JunctionAttributes {
  Id id = -1;  // [required]
  std::string name;
  JunctionType type = JunctionType::DEFAULT;
  Id main_road = -1;               // virtual junctions v1.7
  double s_start = -1;             // virtual junctions v1.7
  double s_end = -1;               // virtual junctions v1.7
  Dir orientation = Dir::UNKNOWN;  // virtual junctions v1.7
};

struct JunctionLaneLink {
  Id from = -1;  // [required] incomingRoad road id
  Id to = -1;    // [required] connectingRoad road id
};
typedef std::vector<JunctionLaneLink> JunctionLaneLinks;

struct JunctionConnection {
  Id id = -1;  // [required]
  JunctionConnectionType type = JunctionConnectionType::UNKNOWN;
  Id incoming_road = -1;
  Id connecting_road = -1;  // @type ="direct" shall not have the attribute v1.7
  Id linked_road = -1;  // Only to be used for junctions of @type="direct" v1.7
  ContactPointType contact_point = ContactPointType::UNKNOWN;
  JunctionLaneLinks lane_links;
};
typedef std::vector<JunctionConnection> JunctionConnections;

struct Junction {
  JunctionAttributes attributes;
  JunctionConnections connections;
};

typedef struct Map MapType;
struct Map {
  typedef std::shared_ptr<MapType> Ptr;
  Header header;
  std::vector<Road> roads;
  std::vector<Junction> junctions;
};

}  // namespace element
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_TYPES_H_
