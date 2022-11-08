#ifndef OPENDRIVE_CPP_ENUMS_H_
#define OPENDRIVE_CPP_ENUMS_H_

#include <string>

namespace opendrive {

enum class Boolean : std::uint8_t { FALSE = 0, TRUE = 1, UNKNOWN = 2 };

enum class GeometryType : std::uint8_t {
  UNKNOWN = 0,
  ARC = 1,
  LINE = 2,
  SPIRAL = 3,
  POLY3 = 4,
  PARAMPOLY3 = 5
};

enum class LaneType : std::uint8_t {
  UNKNOWN = 0,
  SHOULDER = 1,
  BORDER = 2,
  DRIVING = 3,
  STOP = 4,
  NONE = 5,
  RESTRICTED = 6,
  PARKING = 7,
  MEDIAN = 8,
  BIKING = 9,
  SIDEWALK = 10,
  CURB = 11,
  EXIT = 12,
  ENTRY = 13,
  ONRAMP = 14,
  OFFRAMP = 15,
  CONNECTINGRAMP = 16,
  BIDIRECTIONAL = 17,
  SPECIAL1 = 18,
  SPECIAL2 = 19,
  SPECIAL3 = 20,
  ROADWORKS = 21,
  TRAM = 22,
  RAIL = 23,
  BUS = 24,
  TAXI = 25,
  HOV = 26,
  MWYENTRY = 27,
  MWYEXIT = 28
};

enum class RoadMarkType : std::uint8_t {
  UNKNOWN = 0,
  NONE = 1,
  SOLID = 2,
  BROKEN = 3,
  SOLIDSOLID = 4,
  SOLIDBROKEN = 5,
  BROKENSOLID = 6,
  BROKENBROKEN = 7,
  BOTTSDOTS = 8,
  GRASS = 9,
  CURB = 10,
  CUSTOM = 11,
  EDGE = 12
};

enum class RoadMarkColor : std::uint8_t {
  UNKNOWN = 0,
  STANDARD = 1,
  BLUE = 2,
  GREEN = 3,
  RED = 4,
  WHITE = 5,
  YELLOW = 6,
  ORANGE = 7
};

enum class RoadMarkWeight : std::uint8_t {
  UNKNOWN = 0,
  STANDARD = 1,
  BOLD = 2
};

enum class RoadMarkLaneChange : std::uint8_t {
  UNKNOWN = 0,
  NONE = 1,
  INCREASE = 2,
  DECREASE = 3,
  BOTH = 4,
};

enum class RoadRule { UNKNOWN = 0, RHT = 1, LHT = 2 };

enum class RoadType : std::uint8_t {
  UNKNOWN = 0,
  RURAL = 1,
  MOTORWAY = 2,
  TOWN = 3,
  LOWSPEED = 4,
  PEDESTRIAN = 5,
  BICYCLE = 6,
  TOWNEXPRESSWAY = 7,
  TOWNCOLLECTOR = 8,
  TOWNARTERIAL = 9,
  TOWNPRIVATE = 10,
  TOWNLOCAL = 11,
  TOWNPLAYSTREET = 12
};

enum class RoadSpeedUnit {
  UNKNOWN = 0,
  MS = 1,   // m/s
  MPH = 2,  // mph
  KMH = 3   // km/h
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ENUMS_H_
