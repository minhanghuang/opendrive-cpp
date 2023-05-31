#ifndef OPENDRIVE_CPP_CHOICES_H_
#define OPENDRIVE_CPP_CHOICES_H_

#include <map>

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {

extern const std::map<Boolean, std::string> BOOLEAN_CHOICES;

extern const std::map<GeometryType, std::string> GEOMETRY_TYPE_CHOICES;

extern const std::map<LaneType, std::string> LANE_TYPE_CHOICES;

extern const std::map<RoadMarkType, std::string> ROADMARK_TYPE_CHOICES;

extern const std::map<RoadMarkColor, std::string> ROAD_MARK_COLOR_CHOICES;

extern const std::map<RoadMarkWeight, std::string> ROAD_MARK_WEIGHT_CHOICES;

extern const std::map<RoadMarkLaneChange, std::string>
    ROAD_MARK_LANE_CHANGE_CHOICES;

extern const std::map<RoadRule, std::string> ROAD_RULE_CHOICES;

extern const std::map<RoadType, std::string> ROAD_TYPE_CHOICES;

extern const std::map<RoadLinkType, std::string> ROAD_LINK_TYPE_CHOICES;

extern const std::map<SpeedUnit, std::string> SPEEDUNIT_CHOICES;

extern const std::map<LaneDirection, std::string> LANE_DIRECTION_CHOICES;

extern const std::map<JunctionType, std::string> JUNCTION_TYPE_CHOICES;

extern const std::map<JunctionConnectionType, std::string>
    JUNCTION_CONNECTION_TYPE_CHOICES;

extern const std::map<ContactPointType, std::string> CONTACT_POINT_TYPE_CHOICES;

extern const std::map<Dir, std::string> DIR_CHOICES;

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CHOICES_H_
