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
  SHOULDER,  // Describes a soft border at the edge of the road.
  BORDER,    // Describes a hard border at the edge of the road. It has the same
             // height as the drivable lane.
  DRIVING,   // Describes a "normal" drivable road that is not one of the other
             // types.
  STOP,      // Hard shoulder on motorways for emergency stops
  NONE,  // Describes the space on the outermost edge of the road and does not
         // have actual content Its only purpose is for applications to register
         // that ASAM OpenDRIVE is still present in case the (human) driver
         // leaves the road.
  RESTRICTED,  // Describes a lane on which cars should not drive. The lane has
               // the same height as drivable lanes. Typically, the lane is
               // separated with lines and often contains dotted lines as well.
  PARKING,     // Describes a lane with parking spaces.
  MEDIAN,      // Describes a lane that sits between driving lanes that lead in
           // opposite directions. It is typically used to separate traffic in
           // towns on large roads.
  BIKING,    // Describes a lane that is reserved for cyclists.
  SIDEWALK,  // Describes a lane on which pedestrians can walk.
  CURB,   // Describes curb stones. Curb stones have a different height than the
          // adjacent drivable lanes.
  EXIT,   // Describes a lane that is used for sections that are parallel to the
          // main road. It is mainly used for deceleration lanes.
  ENTRY,  // Describes a lane type that is used for sections that are parallel
          // to the main road. It is mainly used for acceleration lanes.
  ONRAMP,   // A ramp leading to a motorway from rural or urban roads.
  OFFRAMP,  //  A ramp leading away from a motorway and onto rural urban roads.
  CONNECTINGRAMP,  //  A ramp that connects two motorways, for example, motorway
                   //  junctions.
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

enum class RoadRule { RHT, LHT };

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

enum class RoadLinkType { UNKNOWN = 0, ROAD, JUNCTION };

enum class RoadSpeedUnit {
  UNKNOWN = 0,
  MS,   // m/s
  MPH,  // mph
  KMH   // km/h
};

enum class LaneDirection { UNKNOWN = 0, LEFT, CENTER, RIGHT };

enum class JunctionType { DEFAULT, DIRECT, VIRTUAL };

enum class JunctionConnectionType { UNKNOWN = 0, DEFAULT, VIRTUAL };

enum class ContactPointType { UNKNOWN = 0, START, END };

enum class Dir { UNKNOWN = 0, PLUS, MINUS };

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ENUMS_H_
