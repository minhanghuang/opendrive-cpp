#ifndef OPENDRIVE_CPP_TYPES_H_
#define OPENDRIVE_CPP_TYPES_H_

#include <array>
#include <cmath>
#include <cstdint>
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
typedef std::vector<Id> Ids;
typedef std::string IdStr;
typedef std::string Name;
typedef std::array<double, 1> Vec1D;
typedef std::array<double, 2> Vec2D;
typedef std::array<double, 3> Vec3D;

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

typedef struct Geometry GeometryTypedef;
class Geometry {
 public:
  typedef std::shared_ptr<GeometryTypedef> Ptr;
  Geometry() = delete;
  Geometry(double s, double x, double y, double hdg, double length,
           GeometryType type)
      : s_(s), x_(x), y_(y), hdg_(hdg), length_(length), type_(type) {}
  virtual ~Geometry() = default;
  virtual Vec2D GetXY(double distance) = 0;
  virtual double s() const final { return s_; }
  virtual double x() const final { return s_; }
  virtual double y() const final { return s_; }
  virtual double hdg() const final { return s_; }
  virtual double length() const final { return s_; }
  virtual GeometryType type() const final { return type_; }

 protected:
  double s_;
  double x_;
  double y_;
  double hdg_;
  double length_;
  GeometryType type_;
};

class GeometryLine : public Geometry {
 public:
  GeometryLine(double s, double x, double y, double hdg, double length,
               GeometryType type)
      : Geometry(s, x, y, hdg, length, type) {}
  virtual Vec2D GetXY(double distance) override {
    distance = common::Clamp<double>(distance, 0, length_);
    double xd = x_ + (std::cos(hdg_) * (distance - s_));
    double yd = y_ + (std::sin(hdg_) * (distance - s_));
    return Vec2D{xd, yd};
  }
};

class GeometryArc : public Geometry {
 public:
  GeometryArc(double s, double x, double y, double hdg, double length,
              GeometryType type, double curvature)
      : Geometry(s, x, y, hdg, length, type), curvature_(curvature) {}

  double curvature() const { return curvature_; }
  virtual Vec2D GetXY(double distance) override {
    distance = common::Clamp<double>(distance, 0, length_);
    const double angle_at_s = (distance - s_) * curvature_ - M_PI / 2;
    const double radius = 1 / curvature_;
    double xd = radius * (std::cos(hdg_ + angle_at_s) - std::sin(hdg_)) + x_;
    double yd = radius * (std::sin(hdg_ + angle_at_s) + std::cos(hdg_)) + y_;
    return Vec2D{xd, yd};
  }

 private:
  double curvature_;
};

class GeometrySpiral : public Geometry {
 public:
  GeometrySpiral(double s, double x, double y, double hdg, double length,
                 GeometryType type, double curve_start, double curve_end)
      : Geometry(s, x, y, hdg, length, type),
        curve_start_(curve_start),
        curve_end_(curve_end) {
    Init();
  }

  double curve_start() const { return curve_start_; }
  double curve_end() const { return curve_end_; }
  virtual Vec2D GetXY(double distance) override {
    Vec2D ret;
    double x1, y1, a1;
    odrSpiral(distance - s_ + s0_, curve_dot_, &x1, &y1, &a1);
    double hdg = hdg_ - a0_;
    double xd =
        (std::cos(hdg) * (x1 - x0_)) - (std::sin(hdg) * (y1 - y0_)) + x_;
    double yd =
        (std::sin(hdg) * (x1 - x0_)) + (std::cos(hdg) * (y1 - y0_)) + y_;
    return Vec2D{xd, yd};
  }

 private:
  void Init() {
    curve_dot_ = (curve_end_ - curve_start_) / (length_);
    s_start_ = curve_start_ / curve_dot_;
    s_end_ = curve_end_ / curve_dot_;
    s0_ = curve_start_ / curve_dot_;
    odrSpiral(s0_, curve_dot_, &x0_, &y0_, &a0_);
  }
  double curve_dot_;
  double s_start_;
  double s_end_;
  double s0_;
  double x0_;
  double y0_;
  double a0_;
  double curve_start_;
  double curve_end_;
};

class GeometryPoly3 : public Geometry {
 public:
  GeometryPoly3(double s, double x, double y, double hdg, double length,
                GeometryType type, double a, double b, double c, double d)
      : Geometry(s, x, y, hdg, length, type), a_(a), b_(b), c_(c), d_(d) {}

  double a() const { return a_; }
  double b() const { return b_; }
  double c() const { return c_; }
  double d() const { return d_; }
  virtual Vec2D GetXY(double distance) override {
    Vec2D ret;
    return ret;
  }

 private:
  double a_ = 0.;
  double b_ = 0.;
  double c_ = 0.;
  double d_ = 0.;
};

class GeometryParamPoly3 : public Geometry {
 public:
  enum class PRange : std::uint8_t {
    UNKNOWN = 0,
    ARCLENGTH = 1,
    NORMALIZED = 2
  };
  GeometryParamPoly3(double s, double x, double y, double hdg, double length,
                     GeometryType type, double au, double bu, double cu,
                     double du, double av, double bv, double cv, double dv,
                     PRange p_range)
      : Geometry(s, x, y, hdg, length, type),
        au_(au),
        bu_(bu),
        cu_(cu),
        du_(du),
        av_(av),
        bv_(bv),
        cv_(cv),
        dv_(dv),
        p_range_(p_range) {}
  double au() const { return au_; }
  double bu() const { return bu_; }
  double cu() const { return cu_; }
  double du() const { return du_; }
  double av() const { return av_; }
  double bv() const { return bv_; }
  double cv() const { return cv_; }
  double dv() const { return dv_; }
  PRange p_range() const { return p_range_; }
  virtual Vec2D GetXY(double distance) override {
    Vec2D ret;
    return ret;
  }

 private:
  PRange p_range_;
  double au_;
  double bu_;
  double cu_;
  double du_;
  double av_;
  double bv_;
  double cv_;
  double dv_;
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
  std::vector<Geometry::Ptr> geometrys;
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

}  // namespace element
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_TYPES_H_
