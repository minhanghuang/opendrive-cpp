#ifndef OPENDRIVE_CPP_ENUMS_H_
#define OPENDRIVE_CPP_ENUMS_H_

#include <string>

namespace opendrive {

enum class Boolean : std::uint8_t { FALSE = 0, TRUE, UNKNOWN };

enum class GeometryType : std::uint8_t {
  UNKNOWN = 0,
  ARC,
  LINE,
  SPIRAL,
  POLY3,
  PARAMPOLY3,
};

enum class LaneType : std::uint8_t {
  UNKNOWN = 0,
  SHOULDER,
  BORDER,
  DRIVING,
  STOP,
  NONE,
  RESTRICTED,
  PARKING,
  MEDIAN,
  BIKING,
  SIDEWALK,
  CURB,
  EXIT,
  ENTRY,
  ONRAMP,
  OFFRAMP,
  CONNECTINGRAMP,
  BIDIRECTIONAL,
  SPECIAL1,
  SPECIAL2,
  SPECIAL3,
  ROADWORKS,
  TRAM,
  RAIL,
  BUS,
  TAXI,
  HOV,
  MWYENTRY,
  MWYEXIT
};

enum class RoadMarkType : std::uint8_t {
  UNKNOWN = 0,
  NONE,
  SOLID,
  BROKEN,
  SOLIDSOLID,
  SOLIDBROKEN,
  BROKENSOLID,
  BROKENBROKEN,
  BOTTSDOTS,
  GRASS,
  CURB,
  CUSTOM,
  EDGE
};

enum class RoadMarkColor : std::uint8_t {
  UNKNOWN = 0,
  STANDARD,
  BLUE,
  GREEN,
  RED,
  WHITE,
  YELLOW,
  ORANGE
};

enum class RoadMarkWeight : std::uint8_t { UNKNOWN = 0, STANDARD, BOLD };

enum class RoadMarkLaneChange : std::uint8_t {
  UNKNOWN,
  NONE,
  INCREASE,
  DECREASE,
  BOTH,
};

enum class RoadRule { UNKNOWN = 0, RHT, LHT };

enum class RoadType : std::uint8_t {
  UNKNOWN = 0,
  RURAL,
  MOTORWAY,
  TOWN,
  LOWSPEED,
  PEDESTRIAN,
  BICYCLE,
  TOWNEXPRESSWAY,
  TOWNCOLLECTOR,
  TOWNARTERIAL,
  TOWNPRIVATE,
  TOWNLOCAL,
  TOWNPLAYSTREET
};

enum class RoadLinkType { UNKNOWN = 0, ROAD = 1, JUNCTION = 2 };

enum class RoadSpeedUnit {
  UNKNOWN = 0,
  MS,   // m/s
  MPH,  // mph
  KMH   // km/h
};

enum class LaneDirection { UNKNOWN = 0, LEFT, CENTER, RIGHT };

enum class JunctionType { UNKNOWN = 0, DEFAULT, DIRECT, VIRTUAL };

enum class JunctionContactPointType { UNKNOWN = 0, START, END };

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ENUMS_H_
