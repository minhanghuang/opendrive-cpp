#ifndef OPENDRIVE_CPP_CORE_H_
#define OPENDRIVE_CPP_CORE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {
namespace core {

typedef std::string Id;
typedef std::set<Id> Ids;
typedef std::vector<Id> Path;

struct Point2D {
  double x = 0.;
  double y = 0.;
};
typedef std::vector<Point2D> Point2Ds;

struct PointXD : public Point2D {
  Id id;
  double s = 0.;
  double hdg = 0.;
};
typedef std::vector<PointXD> PointXDs;

struct Line {
  PointXDs points;
};

struct LaneBoundary {
  Line line;
  RoadMarkType type = RoadMarkType::UNKNOWN;
  RoadMarkColor color = RoadMarkColor::UNKNOWN;
};

typedef struct Lane LaneTypedef;
struct Lane {
  typedef std::shared_ptr<LaneTypedef> Ptr;
  typedef std::vector<Ptr> Ptrs;
  typedef std::shared_ptr<LaneTypedef const> ConstPtr;
  Id id;  // [required]
  LaneType type = LaneType::UNKNOWN;
  Line center_line;
  LaneBoundary left_boundary;
  LaneBoundary right_boundary;
  Ids predecessors;
  Ids successors;
  Id left_lane;   // current lane left lane
  Id right_lane;  // current lane right lane
};

typedef struct Section SectionTypedef;
struct Section {
  typedef std::shared_ptr<SectionTypedef> Ptr;
  typedef std::vector<Ptr> Ptrs;
  typedef std::shared_ptr<SectionTypedef const> ConstPtr;
  Id id;  // [required]
  double start_position = 0.;
  double length = 0.;
  double speed_limit = 0.;  // km/h
  Lane::Ptr center_lane;
  Lane::Ptrs left_lanes;
  Lane::Ptrs right_lanes;
};

typedef struct Road RoadTypedef;
struct Road {
  typedef std::shared_ptr<RoadTypedef> Ptr;
  typedef std::shared_ptr<RoadTypedef const> ConstPtr;
  Id id;  // [required]
  double length = 0.;
  Section::Ptrs sections;
  Id predecessor;
  Id successor;
  RoadLinkType predecessor_type = RoadLinkType::UNKNOWN;
  RoadLinkType successor_type = RoadLinkType::UNKNOWN;
};

struct JunctionConnection {
  Id id;       // [required]
  Id link_id;  // [required] [extended] (incoming_road + "_" + connecting_road)
  JunctionType type = JunctionType::UNKNOWN;
  Id incoming_road;
  Id connecting_road;
  JunctionContactPointType contact_point = JunctionContactPointType::UNKNOWN;
  std::vector<std::pair<Id, Id>> lane_links;  // {from : to}
};
typedef std::unordered_map<core::Id, JunctionConnection> JunctionConnections;

typedef struct Junction JunctionTypedef;
struct Junction {
  typedef std::shared_ptr<JunctionTypedef> Ptr;
  Id id;
  std::string name;
  JunctionType type = JunctionType::UNKNOWN;
  JunctionConnections connections;
};

typedef struct Header HeaderTypedef;
struct Header {
  typedef std::shared_ptr<HeaderTypedef> Ptr;
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

typedef struct Map MapTypedef;
struct Map {
  typedef std::shared_ptr<MapTypedef> Ptr;
  Header::Ptr header;
  std::unordered_map<core::Id, Road::Ptr> roads;
  std::unordered_map<core::Id, Junction::Ptr> junctions;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_H_
