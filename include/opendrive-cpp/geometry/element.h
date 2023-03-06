#ifndef OPENDRIVE_CPP_ELEMENT_H_
#define OPENDRIVE_CPP_ELEMENT_H_

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

class Header {
 public:
  Header() : north_(0), south_(0), west_(0), east_(0) {}
  void set_rev_major(const std::string& s) { rev_major_ = s; }
  void set_rev_minor(const std::string& s) { rev_minor_ = s; }
  void set_version(const std::string& s) { version_ = s; }
  void set_name(const std::string& s) { name_ = s; }
  void set_date(const std::string& s) { date_ = s; }
  void set_vendor(const std::string& s) { vendor_ = s; }
  void set_north(double d) { north_ = d; }
  void set_south(double d) { south_ = d; }
  void set_west(double d) { west_ = d; }
  void set_east(double d) { east_ = d; }
  std::string& mutable_rev_major() { return rev_major_; }
  std::string& mutable_rev_minor() { return rev_minor_; }
  std::string& mutable_version() { return version_; }
  std::string& mutable_name() { return name_; }
  std::string& mutable_date() { return date_; }
  std::string& mutable_vendor() { return vendor_; }
  double& mutable_north() { return north_; }
  double& mutable_south() { return south_; }
  double& mutable_west() { return west_; }
  double& mutable_east() { return east_; }
  const std::string& rev_major() const { return rev_major_; }
  const std::string& rev_minor() const { return rev_minor_; }
  const std::string& version() const { return version_; }
  const std::string& name() const { return name_; }
  const std::string& date() const { return date_; }
  const std::string& vendor() const { return vendor_; }
  double north() const { return north_; }
  double south() const { return south_; }
  double west() const { return west_; }
  double east() const { return east_; }

 private:
  std::string rev_major_;
  std::string rev_minor_;
  std::string version_;
  std::string name_;
  std::string date_;
  std::string vendor_;
  double north_;
  double south_;
  double west_;
  double east_;
};

class Geometry {
 public:
  typedef std::shared_ptr<Geometry> Ptr;
  typedef std::vector<Ptr> Ptrs;
  Geometry(double _s, double _x, double _y, double _hdg, double _length,
           GeometryType _type)
      : s_(_s),
        x_(_x),
        y_(_y),
        hdg_(_hdg),
        length_(_length),
        type_(_type),
        sin_hdg_(std::sin(_hdg)),
        cos_hdg_(std::cos(_hdg)) {}
  virtual ~Geometry() = default;
  virtual Point GetPoint(double ref_line_ds) const = 0;
  double s() const { return s_; }
  double x() const { return x_; }
  double y() const { return y_; }
  double heading() const { return hdg_; }
  double length() const { return length_; }
  GeometryType type() const { return type_; }
  double sin_hdg() const { return sin_hdg_; }
  double cos_hdg() const { return cos_hdg_; }

 protected:
  const double s_;
  const double x_;
  const double y_;
  const double hdg_;
  const double length_;
  const double sin_hdg_;
  const double cos_hdg_;
  const GeometryType type_;
};

class GeometryLine final : public Geometry {
 public:
  GeometryLine(double s, double x, double y, double hdg, double length,
               GeometryType type)
      : Geometry(s, x, y, hdg, length, type) {}
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s_;
    const double xd = x_ + (cos_hdg_ * ref_line_ds);
    const double yd = y_ + (sin_hdg_ * ref_line_ds);
    return Point{.x = xd, .y = yd, .hdg = hdg_};
  }
};

class GeometryArc final : public Geometry {
 public:
  GeometryArc(double s, double x, double y, double hdg, double length,
              GeometryType type, double curvature)
      : Geometry(s, x, y, hdg, length, type),
        curvature_(curvature),
        radius_(1.0 / curvature) {}
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s_;
    const double angle_at_s = ref_line_ds * curvature_ - M_PI / 2;
    const double xd = radius_ * (std::cos(hdg_ + angle_at_s) - sin_hdg_) + x_;
    const double yd = radius_ * (std::sin(hdg_ + angle_at_s) + cos_hdg_) + y_;
    const double tangent = hdg_ + ref_line_ds * curvature_;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  double curvature() const { return curvature_; }
  double radius() const { return radius_; }

 private:
  const double curvature_;
  const double radius_;
};

class GeometrySpiral final : public Geometry {
 public:
  GeometrySpiral(double s, double x, double y, double hdg, double length,
                 GeometryType type, double curve_start, double curve_end)
      : Geometry(s, x, y, hdg, length, type),
        curve_start_(curve_start),
        curve_end_(curve_end),
        curve_dot_((curve_end - curve_start) / (length)) {}

  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s_;
    const double s1 = curve_start_ / curve_dot_ + ref_line_ds;
    double x1;
    double y1;
    double t1;
    odrSpiral(s1, curve_dot_, &x1, &y1, &t1);

    const double s0 = curve_start_ / curve_dot_;
    double x0;
    double y0;
    double t0;
    odrSpiral(s1, curve_dot_, &x0, &y0, &t0);

    x1 -= x0;
    y1 -= y0;
    t1 -= t0;
    const double angle = hdg_ - t0;
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    const double xd = x_ + x1 * cos_a - y1 * sin_a;
    const double yd = y_ + y1 * cos_a + x1 * sin_a;
    const double tangent = hdg_ + t1;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  double curve_start() const { return curve_start_; }
  double curve_end() const { return curve_end_; }
  double curve_dot() const { return curve_dot_; }

 private:
  const double curve_start_;
  const double curve_end_;
  const double curve_dot_;
};

class GeometryPoly3 final : public Geometry {
 public:
  GeometryPoly3(double s, double x, double y, double hdg, double length,
                GeometryType type, double a, double b, double c, double d)
      : Geometry(s, x, y, hdg, length, type), a_(a), b_(b), c_(c), d_(d) {}

  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s_;
    const double u = ref_line_ds;
    const double v = a_ + b_ * u + c_ * std::pow(u, 2) + d_ * std::pow(u, 3);
    const double x1 = u * cos_hdg_ - v * sin_hdg_;
    const double y1 = u * sin_hdg_ + v * cos_hdg_;
    const double tangent_v = b_ + 2.0 * c_ * u + 3.0 * d_ * std::pow(u, 2);
    const double theta = std::atan2(tangent_v, 1.0);
    const double xd = x_ + x1;
    const double yd = y_ + y1;
    const double tangent = hdg_ + theta;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }
  double a() const { return a_; }
  double b() const { return b_; }
  double c() const { return c_; }
  double d() const { return d_; }

 private:
  const double a_;
  const double b_;
  const double c_;
  const double d_;
};

class GeometryParamPoly3 final : public Geometry {
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
  virtual Point GetPoint(double road_ds) const override {
    const double ref_line_ds = road_ds - s_;
    double p = ref_line_ds;
    if (PRange::NORMALIZED == p_range_) {
      p = std::min(1.0, ref_line_ds / length_);
    }
    const double u =
        au_ + bu_ * p + cu_ * std::pow(p, 2) + du_ * std::pow(p, 3);
    const double v =
        av_ + bv_ * p + cv_ * std::pow(p, 2) + dv_ * std::pow(p, 3);
    const double x1 = u * cos_hdg_ - v * sin_hdg_;
    const double y1 = u * sin_hdg_ + v * cos_hdg_;
    const double tangent_u = bu_ + 2 * cu_ * p + 3 * du_ * std::pow(p, 2);
    const double tangent_v = bv_ + 2 * cv_ * p + 3 * dv_ * std::pow(p, 2);
    const double theta = std::atan2(tangent_v, tangent_u);
    const double xd = x_ + x1;
    const double yd = y_ + y1;
    const double tangent = hdg_ + theta;
    return Point{.x = xd, .y = yd, .hdg = tangent};
  }

  PRange p_range() const { return p_range_; }
  double au() const { return au_; }
  double bu() const { return bu_; }
  double cu() const { return cu_; }
  double du() const { return du_; }
  double av() const { return av_; }
  double bv() const { return bv_; }
  double cv() const { return cv_; }
  double dv() const { return dv_; }

 private:
  const PRange p_range_;
  const double au_;
  const double bu_;
  const double cu_;
  const double du_;
  const double av_;
  const double bv_;
  const double cv_;
  const double dv_;
};

class LaneAttributes {
 public:
  LaneAttributes()
      : id_(std::numeric_limits<Id>::max()),
        type_(LaneType::DRIVING),
        level_(Boolean::UNKNOWN) {}
  void set_id(Id i) { id_ = i; }
  void set_type(LaneType i) { type_ = i; }
  void set_level(Boolean b) { level_ = b; }
  Id& mutable_id() { return id_; }
  LaneType& mutable_type() { return type_; }
  Boolean& mutable_level() { return level_; }
  Id id() const { return id_; }
  LaneType type() const { return type_; }
  Boolean level() const { return level_; }

 private:
  Id id_;
  LaneType type_;
  Boolean level_;
};

class OffsetPoly3 {
 public:
  OffsetPoly3() : s_(0), a_(0), b_(0), c_(0), d_(0) {}
  bool operator<(const OffsetPoly3& obj) const { return this->s_ > obj.s_; }
  virtual double GetOffsetValue(double road_ds) const final {
    const double ds = road_ds - s_;
    return a_ + b_ * ds + c_ * std::pow(ds, 2) + d_ * std::pow(ds, 3);
  }
  void set_s(double d) { s_ = d; }
  void set_a(double d) { a_ = d; }
  void set_b(double d) { b_ = d; }
  void set_c(double d) { c_ = d; }
  void set_d(double d) { d_ = d; }
  double& mutable_s() { return s_; }
  double& mutable_a() { return a_; }
  double& mutable_b() { return b_; }
  double& mutable_c() { return c_; }
  double& mutable_d() { return d_; }
  double s() const { return s_; }
  double a() const { return a_; }
  double b() const { return b_; }
  double c() const { return c_; }
  double d() const { return d_; }

 protected:
  // f(s) = a + b*s + c*s*s + d*s*s*s
  double s_;  // start position (s - start_offset)[meters]]
  double a_;  // a - polynomial value at start_offset=0
  double b_;  // b
  double c_;  // c
  double d_;  // d
};

class RoadMark {
 public:
  RoadMark()
      : s_(0),
        type_(RoadMarkType::NONE),
        color_(RoadMarkColor::STANDARD),
        weigth_(RoadMarkWeight::UNKNOWN),
        lane_change_(RoadMarkLaneChange::UNKNOWN),
        width_(0),
        height_(0),
        material_("standard") {}
  void set_s(double d) { s_ = d; }
  void set_type(RoadMarkType i) { type_ = i; }
  void set_color(RoadMarkColor i) { color_ = i; }
  void set_weigth(RoadMarkWeight i) { weigth_ = i; }
  void set_lane_change(RoadMarkLaneChange i) { lane_change_ = i; }
  void set_width(double d) { width_ = d; }
  void set_height(double d) { height_ = d; }
  void set_material(const std::string& s) { material_ = s; }
  double& mutable_s() { return s_; }
  RoadMarkType& mutable_type() { return type_; }
  RoadMarkColor& mutable_color() { return color_; }
  RoadMarkWeight& mutable_weigth() { return weigth_; }
  RoadMarkLaneChange& mutable_lane_change() { return lane_change_; }
  double& mutable_width() { return width_; }
  double& mutable_height() { return height_; }
  std::string& mutable_material() { return material_; }
  double s() const { return s_; }
  RoadMarkType type() const { return type_; }
  RoadMarkColor color() const { return color_; }
  RoadMarkWeight weigth() const { return weigth_; }
  RoadMarkLaneChange lane_change() const { return lane_change_; }
  double width() const { return width_; }
  double height() const { return height_; }
  const std::string& material() const { return material_; }

 private:
  double s_;
  RoadMarkType type_;
  RoadMarkColor color_;
  RoadMarkWeight weigth_;
  RoadMarkLaneChange lane_change_;
  double width_;
  double height_;
  std::string material_;
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
  SpeedUnit unit = SpeedUnit::MS;
};
typedef std::vector<LaneSpeed> LaneSpeeds;

class Lane {
 public:
  Lane() {}
  double GetLaneWidth(double road_ds) const {
    // width >> border
    if (road_ds < 0) {
      return GetLaneWidth(0.);
    }
    if (widths_.empty()) {
      if (borders_.empty()) {
        return 0.;
      }
      /// border
      int border_index = common::GetGtValuePoloy3(borders_, road_ds);
      if (border_index < 0) {
        return 0.;
      }
      auto border = widths_.at(border_index);
      return border.GetOffsetValue(road_ds);
    } else {
      /// width
      int width_index = common::GetGtValuePoloy3(widths_, road_ds);
      if (width_index < 0) {
        return 0.;
      }
      auto width = widths_.at(width_index);
      return width.GetOffsetValue(road_ds);
    }
    return 0.;
  }

 private:
  LaneAttributes attrs_;
  LaneLink link_;
  LaneWidths widths_;
  LaneBorders borders_;
  RoadMarks road_marks_;
  LaneSpeeds max_speeds_;
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
  RoadRule rule =
      RoadRule::RHT;  // RHT=right-hand traffic, LHT=left-hand traffic. When
                      // this attribute is missing, RHT is assumed
};

struct RoadLinkInfo {
  Id id = -1;     // [>=0]
  double s = -1;  // [>=0]
  RoadLinkType type = RoadLinkType::ROAD;
  ContactPointType contact_point =
      ContactPointType::UNKNOWN;  // Contact point of link on the linked element
  Dir dir = Dir::UNKNOWN;
};

struct RoadLink {
  RoadLinkInfo predecessor;
  RoadLinkInfo successor;
};

struct RoadTypeInfo {
  double s = 0.;
  RoadType type = RoadType::TOWN;
  std::string country;
  float max_speed = 0.;
  SpeedUnit speed_unit = SpeedUnit::MS;
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

#endif  // OPENDRIVE_CPP_ELEMENT_H_
