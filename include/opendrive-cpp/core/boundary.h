#ifndef OPENDRIVE_CPP_CORE_BOUNDARY_H_
#define OPENDRIVE_CPP_CORE_BOUNDARY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "geometry.h"
#include "opendrive-cpp/core/types.h"

namespace opendrive {
namespace core {

struct LaneBoundary {
  double length = 0.;
  Line line;
  RoadMarkType type = RoadMarkType::UNKNOWN;
  RoadMarkColor color = RoadMarkColor::UNKNOWN;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_BOUNDARY_H_
