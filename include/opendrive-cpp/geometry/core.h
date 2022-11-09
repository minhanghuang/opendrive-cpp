#ifndef OPENDRIVE_CPP_CORE_H_
#define OPENDRIVE_CPP_CORE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "opendrive-cpp/geometry/types.h"

namespace opendrive {
namespace core {

typedef std::string Id;
typedef std::vector<Id> Ids;

struct Point2D {
  double x = 0.;
  double y = 0.;
};

struct Point3D : public Point2D {
  double y = 0.;
};

struct PointXD : public Point3D {
  double hdg = 0.;
};

struct Line {
  std::vector<PointXD> points;
};

struct LaneBoundary {
  double length = 0.;
  Line line;
  RoadMarkType type = RoadMarkType::UNKNOWN;
  RoadMarkColor color = RoadMarkColor::UNKNOWN;
};

struct Lane {
  Id id;
  double length = 0.;
  double speed_limit = 0.;
  LaneType type = LaneType::UNKNOWN;
  Line line;
  Ids predecessors;
  Ids successor;
  LaneBoundary left_boundary;
  LaneBoundary right_boundary;
};

typedef struct Section SectionType;
struct Section {
  typedef std::shared_ptr<SectionType> Ptr;
  Id id;
  double length = 0.;
  std::vector<Lane> lanes;
};

typedef struct Road RoadType;
struct Road {
  typedef std::shared_ptr<RoadType> Ptr;
  Id id;
  double length = 0.;
  std::vector<Section> sections;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_H_