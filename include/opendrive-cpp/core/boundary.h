#ifndef OPENDRIVE_CPP_CORE_BOUNDARY_H_
#define OPENDRIVE_CPP_CORE_BOUNDARY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "geometry.h"
#include "opendrive-cpp/core/types.hpp"

namespace opendrive {
namespace core {

struct LaneBoundary {
  base::RoadMark::Type type = base::RoadMark::Type::UNKNOWN;
  base::RoadMark::Color color = base::RoadMark::Color::UNKNOWN;
  double length = 0.;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_BOUNDARY_H_
