#ifndef OPENDRIVE_CPP_CORE_GEOMETRY_H_
#define OPENDRIVE_CPP_CORE_GEOMETRY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "opendrive-cpp/core/types.h"

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

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_GEOMETRY_H_
