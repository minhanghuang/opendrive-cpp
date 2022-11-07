#ifndef OPENDRIVE_CPP_CORE_LANE_H_
#define OPENDRIVE_CPP_CORE_LANE_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "boundary.h"
#include "geometry.h"
#include "opendrive-cpp/core/types.h"

namespace opendrive {
namespace core {

struct Lane {
  Id id;
  double length = 0.;
  double speed_limit = 0.;

  Ids predecessors;
  Ids successor;

  LaneBoundary left_boundary;
  LaneBoundary right_boundary;
};

}  // namespace core
}  // namespace opendrive

#endif  //   OPENDRIVE_CPP_CORE_LANE_H_
