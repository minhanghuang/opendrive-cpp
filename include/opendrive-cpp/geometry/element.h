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

class Point {
 public:
  Point() : x_(0), y_(0), z_(0), heading_(0) {}
  Point(double x, double y) : x_(x), y_(y), z_(0), heading_(0) {}
  Point(double x, double y, double z) : x_(x), y_(y), z_(z), heading_(0) {}
  Point(double x, double y, double z, double heading)
      : x_(x), y_(y), z_(z), heading_(heading) {}
  void set_x(double d) { x_ = d; }
  void set_y(double d) { y_ = d; }
  void set_z(double d) { z_ = d; }
  void set_heading(double d) { heading_ = d; }
  double& mutable_x() { return x_; }
  double& mutable_y() { return y_; }
  double& mutable_z() { return z_; }
  double& mutable_heading() { return heading_; }
  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }
  double heading() const { return heading_; }

 protected:
  double x_;
  double y_;
  double z_;
  double heading_;
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
  typedef std::shared_ptr<Geometry const> ConstPtr;
  typedef std::vector<Ptr> Ptrs;
  typedef std::vector<ConstPtr> ConstPtrs;
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
    return Point{xd, yd, 0, hdg_};
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
    return Point{xd, yd, 0, tangent};
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
    return Point{xd, yd, 0, tangent};
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
    return Point{xd, yd, 0, tangent};
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
    return Point{xd, yd, 0, tangent};
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

class LaneAttribute {
 public:
  LaneAttribute()
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

class LaneWidth : public OffsetPoly3 {};
typedef std::vector<LaneWidth> LaneWidths;

class LaneBorder : public OffsetPoly3 {};
typedef std::vector<LaneBorder> LaneBorders;

class LaneLink {
 public:
  LaneLink() {}
  void set_predecessors(const Ids& v) { predecessors_ = v; }
  void set_successors(const Ids& v) { successors_ = v; }
  Ids& mutable_predecessors() { return predecessors_; }
  Ids& mutable_successors() { return successors_; }
  const Ids& predecessors() const { return predecessors_; }
  const Ids& successors() const { return successors_; }

 private:
  Ids predecessors_;
  Ids successors_;
};
typedef std::vector<LaneLink> LaneLinks;

class LaneSpeed {
 public:
  LaneSpeed() : s_(0), max_(0), unit_(SpeedUnit::MS) {}
  void set_s(double d) { s_ = d; }
  void set_max(float f) { max_ = f; }
  void set_unit(SpeedUnit i) { unit_ = i; }
  double& mutable_s() { return s_; }
  float& mutable_max() { return max_; }
  SpeedUnit& mutable_unit() { return unit_; }
  double s() const { return s_; }
  float max() const { return max_; }
  SpeedUnit unit() const { return unit_; }

 private:
  double s_;
  float max_;
  SpeedUnit unit_;
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
  void set_attribute(const LaneAttribute& c) { attr_ = c; }
  void set_link(const LaneLink& c) { link_ = c; }
  void set_widths(const LaneWidths& c) { widths_ = c; }
  void set_borders(const LaneBorders& c) { borders_ = c; }
  void set_road_marks(const RoadMarks& c) { road_marks_ = c; }
  void set_max_speeds(const LaneSpeeds& c) { max_speeds_ = c; }
  LaneAttribute& mutable_attribute() { return attr_; }
  LaneLink& mutable_link() { return link_; }
  LaneWidths& mutable_widths() { return widths_; }
  LaneBorders& mutable_borders() { return borders_; }
  RoadMarks& mutable_road_marks() { return road_marks_; }
  LaneSpeeds& mutable_max_speeds() { return max_speeds_; }
  const LaneAttribute& attribute() const { return attr_; }
  const LaneLink& link() const { return link_; }
  const LaneWidths& widths() const { return widths_; }
  const LaneBorders& borders() const { return borders_; }
  const RoadMarks& road_marks() const { return road_marks_; }
  const LaneSpeeds& max_speeds() const { return max_speeds_; }

 private:
  LaneAttribute attr_;
  LaneLink link_;
  LaneWidths widths_;
  LaneBorders borders_;
  RoadMarks road_marks_;
  LaneSpeeds max_speeds_;
};

class LanesInfo {
 public:
  LanesInfo() {}
  void set_lanes(const std::vector<Lane>& v) { lanes_ = v; }
  std::vector<Lane>& mutable_lanes() { return lanes_; }
  const std::vector<Lane>& lanes() const { return lanes_; }

 private:
  std::vector<Lane> lanes_;
};

class LaneOffset : public OffsetPoly3 {};
typedef std::vector<LaneOffset> LaneOffsets;

class LaneSection {
 public:
  LaneSection() : id_(-1), start_position_(0), end_position_(0) {}
  void set_id(Id i) { id_ = i; }
  void set_start_position(double d) { start_position_ = d; }
  void set_end_position(double d) { end_position_ = d; }
  void set_left(const LanesInfo& c) { left_ = c; }
  void set_center(const LanesInfo& c) { center_ = c; }
  void set_right(const LanesInfo& c) { right_ = c; }
  Id& mutable_id() { return id_; }
  double& mutable_start_position() { return start_position_; }
  double& mutable_end_position() { return end_position_; }
  LanesInfo& mutable_left() { return left_; }
  LanesInfo& mutable_center() { return center_; }
  LanesInfo& mutable_right() { return right_; }
  Id id() const { return id_; }
  double start_position() const { return start_position_; }
  double end_position() const { return end_position_; }
  const LanesInfo& left() const { return left_; }
  const LanesInfo& center() const { return center_; }
  const LanesInfo& right() const { return right_; }

 private:
  Id id_;                  // [>=0] (extended)
  double start_position_;  // start position
  double end_position_;    // end position(extended)
  LanesInfo left_;
  LanesInfo center_;
  LanesInfo right_;
};
typedef std::vector<LaneSection> LaneSections;

class Lanes {
 public:
  Lanes() {}
  void set_lane_offsets(const LaneOffsets& v) { lane_offsets_ = v; }
  void set_lane_sections(const LaneSections& v) { lane_sections_ = v; }
  LaneOffsets& mutable_lane_offsets() { return lane_offsets_; }
  LaneSections& mutable_lane_sections() { return lane_sections_; }
  const LaneOffsets& lane_offsets() const { return lane_offsets_; }
  const LaneSections& lane_sections() const { return lane_sections_; }

 private:
  LaneOffsets lane_offsets_;
  LaneSections lane_sections_;
};

class RoadAttribute {
 public:
  RoadAttribute()
      : name_(""),
        id_(-1),
        junction_id_(-1),
        length_(0),
        rule_(RoadRule::RHT) {}
  void set_name(const Name& s) { name_ = s; }
  void set_id(Id i) { id_ = i; }
  void set_junction_id(Id i) { junction_id_ = i; }
  void set_length(double d) { length_ = d; }
  void set_rule(RoadRule i) { rule_ = i; }
  Name& mutable_name() { return name_; }
  Id& mutable_id() { return id_; }
  Id& mutable_junction_id() { return junction_id_; }
  double& mutable_length() { return length_; }
  RoadRule& mutable_rule() { return rule_; }
  const Name& name() const { return name_; }
  Id id() const { return id_; }
  Id junction_id() const { return junction_id_; }
  double length() const { return length_; }
  RoadRule rule() const { return rule_; }

 private:
  Name name_;
  Id id_;           // [>=0]
  Id junction_id_;  // -1: Road; other: junction road
  double length_;
  RoadRule rule_;  // RHT=right-hand traffic, LHT=left-hand traffic. When
                   // this attribute is missing, RHT is assumed
};

class RoadLinkInfo {
 public:
  RoadLinkInfo()
      : id_(-1),
        start_position_(-1),
        type_(RoadLinkType::ROAD),
        contact_point_(ContactPointType::UNKNOWN),
        dir_(Dir::UNKNOWN) {}
  void set_id(Id i) { id_ = i; }
  void set_start_position(double d) { start_position_ = d; }
  void set_type(RoadLinkType i) { type_ = i; }
  void set_contact_point(ContactPointType i) { contact_point_ = i; }
  void set_dir(Dir i) { dir_ = i; }
  Id& mutable_id() { return id_; }
  double& mutable_start_position() { return start_position_; }
  RoadLinkType& mutable_type() { return type_; }
  ContactPointType& mutable_contact_point() { return contact_point_; }
  Dir& mutable_dir() { return dir_; }
  Id id() const { return id_; }
  double start_position() const { return start_position_; }
  RoadLinkType type() const { return type_; }
  ContactPointType contact_point() const { return contact_point_; }
  Dir dir() const { return dir_; }

 private:
  Id id_;                  // [>=0]
  double start_position_;  // [>=0]
  RoadLinkType type_;
  ContactPointType
      contact_point_;  // Contact point of link on the linked element
  Dir dir_;
};

class RoadLink {
 public:
  RoadLink() {}
  void set_predecessor(const RoadLinkInfo& c) { predecessor_ = c; }
  void set_successor(const RoadLinkInfo& c) { successor_ = c; }
  RoadLinkInfo& mutable_predecessor() { return predecessor_; }
  RoadLinkInfo& mutable_successor() { return successor_; }
  const RoadLinkInfo& predecessor() const { return predecessor_; }
  const RoadLinkInfo& successor() const { return successor_; }

 private:
  RoadLinkInfo predecessor_;
  RoadLinkInfo successor_;
};

class RoadTypeInfo {
 public:
  RoadTypeInfo()
      : start_position_(0),
        type_(RoadType::TOWN),
        country_(""),
        max_speed_(0),
        speed_unit_(SpeedUnit::MS) {}
  void set_start_position(double d) { start_position_ = d; }
  void set_type(RoadType i) { type_ = i; }
  void set_country(const std::string& s) { country_ = s; }
  void set_max_speed(float f) { max_speed_ = f; }
  void set_speed_unit(SpeedUnit i) { speed_unit_ = i; }
  double& mutable_start_position() { return start_position_; }
  RoadType& mutable_type() { return type_; }
  std::string& mutable_country() { return country_; }
  float& mutable_max_speed() { return max_speed_; }
  SpeedUnit& mutable_speed_unit() { return speed_unit_; }
  double start_position() const { return start_position_; }
  RoadType type() const { return type_; }
  std::string country() const { return country_; }
  float max_speed() const { return max_speed_; }
  SpeedUnit speed_unit() const { return speed_unit_; }

 private:
  double start_position_;
  RoadType type_;
  std::string country_;
  float max_speed_;
  SpeedUnit speed_unit_;
};

class RoadPlanView {
 public:
  RoadPlanView() {}
  void set_geometrys(const Geometry::Ptrs& v) { geometrys_ = v; }
  Geometry::Ptrs& mutable_geometrys() { return geometrys_; }
  Geometry::ConstPtrs geometrys() const {
    Geometry::ConstPtrs ret;
    for (const auto& g : geometrys_) {
      ret.emplace_back(g);
    }
    return ret;
  }

 private:
  Geometry::Ptrs geometrys_;
};

class Road {
 public:
  Road() {}
  void set_attribute(const RoadAttribute& c) { attribute_ = c; }
  void set_link(const RoadLink& c) { link_ = c; }
  void set_type_info(const std::vector<RoadTypeInfo>& c) { type_info_ = c; }
  void set_plan_view(const RoadPlanView& c) { plan_view_ = c; }
  void set_lanes(const Lanes& v) { lanes_ = v; }
  RoadAttribute& mutable_attribute() { return attribute_; }
  RoadLink& mutable_link() { return link_; }
  std::vector<RoadTypeInfo>& mutable_type_info() { return type_info_; }
  RoadPlanView& mutable_plan_view() { return plan_view_; }
  Lanes& mutable_lanes() { return lanes_; }
  const RoadAttribute& attribute() const { return attribute_; }
  const RoadLink& link() const { return link_; }
  const std::vector<RoadTypeInfo>& type_info() const { return type_info_; }
  const RoadPlanView& plan_view() const { return plan_view_; }
  const Lanes& lanes() const { return lanes_; }

 private:
  RoadAttribute attribute_;
  RoadLink link_;
  std::vector<RoadTypeInfo> type_info_;
  RoadPlanView plan_view_;
  Lanes lanes_;
};

class JunctionAttribute {
 public:
  JunctionAttribute() {}
  void set_id(Id i) { id_ = i; }
  void set_name(const std::string& s) { name_ = s; }
  void set_type(JunctionType i) { type_ = i; }
  void set_main_road(Id i) { main_road_ = i; }
  void set_start_position(double d) { start_position_ = d; }
  void set_end_position(double d) { end_position_ = d; }
  void set_dir(Dir i) { dir_ = i; }
  Id& mutable_id() { return id_; }
  std::string& mutable_name() { return name_; }
  JunctionType& mutable_type() { return type_; }
  Id& mutable_main_road() { return main_road_; }
  double& mutable_start_position() { return start_position_; }
  double& mutable_end_position() { return end_position_; }
  Dir& mutable_dir() { return dir_; }
  Id id() const { return id_; }
  const std::string& name() const { return name_; }
  JunctionType type() const { return type_; }
  Id main_road() const { return main_road_; }
  double start_position() const { return start_position_; }
  double end_position() const { return end_position_; }
  Dir dir() const { return dir_; }

 private:
  Id id_ = -1;  // [required]
  std::string name_;
  JunctionType type_ = JunctionType::DEFAULT;
  Id main_road_ = -1;           // virtual junctions v1.7
  double start_position_ = -1;  // virtual junctions v1.7
  double end_position_ = -1;    // virtual junctions v1.7
  Dir dir_ = Dir::UNKNOWN;      // virtual junctions v1.7
};

class JunctionLaneLink {
 public:
  JunctionLaneLink() : from_(-1), to_(-1) {}
  void set_from(Id i) { from_ = i; }
  void set_to(Id i) { to_ = i; }
  Id& mutable_from() { return from_; }
  Id& mutable_to() { return to_; }
  Id from() { return from_; }
  Id to() { return to_; }

 private:
  Id from_ = -1;  // [required] incomingRoad road id
  Id to_ = -1;    // [required] connectingRoad road id
};
typedef std::vector<JunctionLaneLink> JunctionLaneLinks;

class JunctionConnection {
 public:
  JunctionConnection()
      : id_(-1),
        type_(JunctionConnectionType::UNKNOWN),
        incoming_road_(-1),
        connecting_road_(-1),
        linked_road_(-1),
        contact_point_(ContactPointType::UNKNOWN) {}
  void set_id(Id i) { id_ = i; }
  void set_type(JunctionConnectionType i) { type_ = i; }
  void set_incoming_road(Id i) { incoming_road_ = i; }
  void set_connecting_road(Id i) { connecting_road_ = i; }
  void set_linked_road(Id i) { linked_road_ = i; }
  void set_contact_point(ContactPointType i) { contact_point_ = i; }
  void set_lane_links(const JunctionLaneLinks& v) { lane_links_ = v; }
  Id& mutable_id() { return id_; }
  JunctionConnectionType& mutable_type() { return type_; }
  Id& mutable_incoming_road() { return incoming_road_; }
  Id& mutable_connecting_road() { return connecting_road_; }
  Id& mutable_linked_road() { return linked_road_; }
  ContactPointType& mutable_contact_point() { return contact_point_; }
  JunctionLaneLinks& mutable_lane_links() { return lane_links_; }
  Id id() const { return id_; }
  JunctionConnectionType type() const { return type_; }
  Id incoming_road() const { return incoming_road_; }
  Id connecting_road() const { return connecting_road_; }
  Id linked_road() const { return linked_road_; }
  ContactPointType contact_point() const { return contact_point_; }
  const JunctionLaneLinks& lane_links() const { return lane_links_; }

 private:
  Id id_;  // [required]
  JunctionConnectionType type_;
  Id incoming_road_;
  Id connecting_road_;  // @type ="direct" shall not have the attribute v1.7
  Id linked_road_;      // Only to be used for junctions of @type="direct" v1.7
  ContactPointType contact_point_;
  JunctionLaneLinks lane_links_;
};
typedef std::vector<JunctionConnection> JunctionConnections;

class Junction {
 public:
  Junction() {}
  void set_attribute(const JunctionAttribute& c) { attribute_ = c; }
  void set_connections(const JunctionConnections& c) { connections_ = c; }
  JunctionAttribute& mutable_attribute() { return attribute_; }
  JunctionConnections& mutable_connections() { return connections_; }
  const JunctionAttribute& attribute() const { return attribute_; }
  const JunctionConnections& connections() const { return connections_; }

 private:
  JunctionAttribute attribute_;
  JunctionConnections connections_;
};

class Map {
 public:
  typedef std::shared_ptr<Map> Ptr;
  typedef std::shared_ptr<Map const> ConstPtr;
  Map() {}
  void set_header(const Header& c) { header_ = c; }
  void set_roads(const std::vector<Road>& c) { roads_ = c; }
  void set_junction(const std::vector<Junction>& c) { junctions_ = c; }
  Header& mutable_header() { return header_; }
  std::vector<Road>& mutable_roads() { return roads_; }
  std::vector<Junction>& mutable_junctions() { return junctions_; }
  const Header& header() const { return header_; }
  const std::vector<Road>& roads() const { return roads_; }
  const std::vector<Junction>& junctions() const { return junctions_; }

 private:
  Header header_;
  std::vector<Road> roads_;
  std::vector<Junction> junctions_;
};

}  // namespace element
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ELEMENT_H_
