#ifndef OPENDRIVE_CPP_CORE_ROAD_H_
#define OPENDRIVE_CPP_CORE_ROAD_H_ 

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "boundary.h"
#include "geometry.h"
#include "opendrive-cpp/core/lane.h"
#include "opendrive-cpp/core/section.h"
#include "opendrive-cpp/core/types.h"

namespace opendrive {
namespace core {

typedef struct Road RoadType;
struct Road {
  typedef std::shared_ptr<RoadType> Ptr;
  Id id;
  double length = 0.;
  std::vector<Section> sections;
};

}  // namespace core
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CORE_SECTION_H_
