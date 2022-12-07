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

namespace opendrive {
namespace core {

typedef std::string Id;
typedef std::vector<Id> Ids;

struct Point2D {
  double x = 0.;
  double y = 0.;
};

struct PointXD : public Point2D {
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
  Ids successor;
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
};

typedef struct Map MapTypedef;
struct Map {
  typedef std::shared_ptr<MapTypedef> Ptr;
  std::unordered_map<core::Id, Road::Ptr> roads;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_H_
