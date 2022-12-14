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
typedef std::vector<Id> Ids;

struct Point2D {
  double x = 0.;
  double y = 0.;
};

struct PointXD : public Point2D {
  Id id;
  double s = 0.;
  double hdg = 0.;
  double tangent = 0.;
};

struct Line {
  std::vector<PointXD> points;
};

struct LaneBoundary {
  Line line;
  RoadMarkType type = RoadMarkType::UNKNOWN;
  RoadMarkColor color = RoadMarkColor::UNKNOWN;
};

typedef struct Lane LaneTypedef;
struct Lane {
  typedef std::shared_ptr<LaneTypedef> Ptr;
  Id id;
  LaneType type = LaneType::UNKNOWN;
  Line line;
  Ids predecessors;
  Ids successors;
  Ids junction_pres;  // junction connecting lane ids
  Ids junction_sucs;  // junction incoming lane ids
  LaneBoundary left_boundary;
  LaneBoundary right_boundary;
};

typedef struct Section SectionTypedef;
struct Section {
  typedef std::shared_ptr<SectionTypedef> Ptr;
  Id id;
  double start_position = 0.;
  double length = 0.;
  double speed_limit = 0.;  // km/h
  Lane::Ptr center_line;
  std::vector<Lane::Ptr> lanes;
};

typedef struct Road RoadTypedef;
struct Road {
  typedef std::shared_ptr<RoadTypedef> Ptr;
  Id id;
  double length = 0.;
  std::vector<Section::Ptr> sections;
  Id predecessor;
  Id successor;
  RoadLinkType predecessor_type = RoadLinkType::UNKNOWN;
  RoadLinkType successor_type = RoadLinkType::UNKNOWN;
};

struct JunctionConnection {
  Id id;  // [required]
  JunctionType type = JunctionType::UNKNOWN;
  Id incoming_road;
  Id connecting_road;
  JunctionContactPointType contact_point = JunctionContactPointType::UNKNOWN;
  std::vector<std::map<Id, Id>> lane_links;  // {from : to}
};

typedef struct Junction JunctionTypedef;
struct Junction {
  typedef std::shared_ptr<JunctionTypedef> Ptr;
  Id id;
  std::string name;
  JunctionType type = JunctionType::UNKNOWN;
  std::vector<JunctionConnection> connections;
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
