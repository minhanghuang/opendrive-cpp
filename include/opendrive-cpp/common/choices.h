#ifndef OPENDRIVE_CPP_CHOICES_H_
#define OPENDRIVE_CPP_CHOICES_H_

#include <unordered_map>
#include <utility>

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {

static const std::unordered_map<Boolean, std::string> BOOLEAN_CHOICES{
    std::make_pair(Boolean::UNKNOWN, "UNKNOWN"),
    std::make_pair(Boolean::FALSE, "false"),
    std::make_pair(Boolean::TRUE, "true"),
};

static const std::unordered_map<GeometryType, std::string>
    GEOMETRY_TYPE_CHOICES{
        std::make_pair(GeometryType::ARC, "ARC"),
        std::make_pair(GeometryType::LINE, "LINE"),
        std::make_pair(GeometryType::SPIRAL, "SPIRAL"),
        std::make_pair(GeometryType::POLY3, "POLY3"),
        std::make_pair(GeometryType::PARAMPOLY3, "PARAMPOLY3"),
    };

static const std::unordered_map<LaneType, std::string> LANE_TYPE_CHOICES{
    std::make_pair(LaneType::SHOULDER, "shouldeR"),
    std::make_pair(LaneType::BORDER, "border"),
    std::make_pair(LaneType::DRIVING, "driving"),
    std::make_pair(LaneType::STOP, "stop"),
    std::make_pair(LaneType::NONE, "none"),
    std::make_pair(LaneType::RESTRICTED, "restricted"),
    std::make_pair(LaneType::PARKING, "parking"),
    std::make_pair(LaneType::MEDIAN, "median"),
    std::make_pair(LaneType::BIKING, "biking"),
    std::make_pair(LaneType::SIDEWALK, "sidewalk"),
    std::make_pair(LaneType::CURB, "curb"),
    std::make_pair(LaneType::EXIT, "exit"),
    std::make_pair(LaneType::ENTRY, "entry"),
    std::make_pair(LaneType::ONRAMP, "onRamp"),
    std::make_pair(LaneType::OFFRAMP, "offRamp"),
    std::make_pair(LaneType::CONNECTINGRAMP, "connectingRamp"),
    std::make_pair(LaneType::BIDIRECTIONAL, "bidirectional"),
    std::make_pair(LaneType::SPECIAL1, "special1"),
    std::make_pair(LaneType::SPECIAL2, "special2"),
    std::make_pair(LaneType::SPECIAL3, "special3"),
    std::make_pair(LaneType::ROADWORKS, "roadWorks"),
    std::make_pair(LaneType::TRAM, "tram"),
    std::make_pair(LaneType::RAIL, "rail"),
    std::make_pair(LaneType::BUS, "bus"),
    std::make_pair(LaneType::TAXI, "taxi"),
    std::make_pair(LaneType::HOV, "HOV"),
    std::make_pair(LaneType::MWYENTRY, "mwyEntry"),
    std::make_pair(LaneType::MWYEXIT, "mwyExit"),
};

static const std::unordered_map<RoadMarkType, std::string>
    ROADMARK_TYPE_CHOICES{
        std::make_pair(RoadMarkType::NONE, "none"),
        std::make_pair(RoadMarkType::SOLID, "solid"),
        std::make_pair(RoadMarkType::BROKEN, "broken"),
        std::make_pair(RoadMarkType::SOLIDSOLID, "solid solid"),
        std::make_pair(RoadMarkType::SOLIDBROKEN, "solid broken"),
        std::make_pair(RoadMarkType::BROKENSOLID, "broken solid"),
        std::make_pair(RoadMarkType::BROKENBROKEN, "broken broken"),
        std::make_pair(RoadMarkType::BOTTSDOTS, "botts dots"),
        std::make_pair(RoadMarkType::GRASS, "grass"),
        std::make_pair(RoadMarkType::CURB, "curb"),
        std::make_pair(RoadMarkType::CUSTOM, "custom"),
        std::make_pair(RoadMarkType::EDGE, "edge"),
    };

static const std::unordered_map<RoadMarkColor, std::string>
    ROAD_MARK_COLOR_CHOICES{
        std::make_pair(RoadMarkColor::STANDARD, "standard"),
        std::make_pair(RoadMarkColor::BLUE, "blue"),
        std::make_pair(RoadMarkColor::GREEN, "green"),
        std::make_pair(RoadMarkColor::RED, "red"),
        std::make_pair(RoadMarkColor::WHITE, "white"),
        std::make_pair(RoadMarkColor::YELLOW, "yellow"),
        std::make_pair(RoadMarkColor::ORANGE, "orange"),
    };

static const std::unordered_map<RoadMarkWeight, std::string>
    ROAD_MARK_WEIGHT_CHOICES{
        std::make_pair(RoadMarkWeight::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkWeight::STANDARD, "standard"),
        std::make_pair(RoadMarkWeight::BOLD, "bold"),
    };

static const std::unordered_map<RoadMarkLaneChange, std::string>
    ROAD_MARK_LANE_CHANGE_CHOICES{
        std::make_pair(RoadMarkLaneChange::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkLaneChange::NONE, "none"),
        std::make_pair(RoadMarkLaneChange::INCREASE, "increase"),
        std::make_pair(RoadMarkLaneChange::DECREASE, "decrease"),
        std::make_pair(RoadMarkLaneChange::BOTH, "both"),
    };

static const std::unordered_map<RoadRule, std::string> ROAD_RULE_CHOICES{
    std::make_pair(RoadRule::RHT, "RHT"),
    std::make_pair(RoadRule::LHT, "LHT"),
};

static const std::unordered_map<RoadType, std::string> ROAD_TYPE_CHOICES{
    std::make_pair(RoadType::RURAL, "rural"),
    std::make_pair(RoadType::MOTORWAY, "motorway"),
    std::make_pair(RoadType::TOWN, "town"),
    std::make_pair(RoadType::LOWSPEED, "lowspeed"),
    std::make_pair(RoadType::PEDESTRIAN, "pedestrian"),
    std::make_pair(RoadType::BICYCLE, "bicycle"),
    std::make_pair(RoadType::TOWNEXPRESSWAY, "townExpressway"),
    std::make_pair(RoadType::TOWNCOLLECTOR, "townCollector"),
    std::make_pair(RoadType::TOWNARTERIAL, "townArterial"),
    std::make_pair(RoadType::TOWNPRIVATE, "townPrivate"),
    std::make_pair(RoadType::TOWNLOCAL, "townLocal"),
    std::make_pair(RoadType::TOWNPLAYSTREET, "townPlaystreet"),
};

static const std::unordered_map<RoadLinkType, std::string>
    ROAD_LINK_TYPE_CHOICES{
        std::make_pair(RoadLinkType::ROAD, "road"),
        std::make_pair(RoadLinkType::JUNCTION, "junction"),
    };

static const std::unordered_map<SpeedUnit, std::string> SPEEDUNIT_CHOICES{
    std::make_pair(SpeedUnit::MS, "m/s"),
    std::make_pair(SpeedUnit::MPH, "mph"),
    std::make_pair(SpeedUnit::KMH, "km/h"),
};

static const std::unordered_map<LaneDirection, std::string>
    LANE_DIRECTION_CHOICES{
        std::make_pair(LaneDirection::UNKNOWN, "UNKNOWN"),
        std::make_pair(LaneDirection::LEFT, "left"),
        std::make_pair(LaneDirection::CENTER, "center"),
        std::make_pair(LaneDirection::RIGHT, "right"),
    };

static const std::unordered_map<JunctionType, std::string>
    JUNCTION_TYPE_CHOICES{
        std::make_pair(JunctionType::DEFAULT, "default"),
        std::make_pair(JunctionType::DIRECT, "direct"),
        std::make_pair(JunctionType::VIRTUAL, "virtual"),
    };

static const std::unordered_map<JunctionConnectionType, std::string>
    JUNCTION_CONNECTION_TYPE_CHOICES{
        std::make_pair(JunctionConnectionType::UNKNOWN, "UNKNOWN"),
        std::make_pair(JunctionConnectionType::DEFAULT, "default"),
        std::make_pair(JunctionConnectionType::VIRTUAL, "virtual"),
    };

static const std::unordered_map<ContactPointType, std::string>
    CONTACT_POINT_TYPE_CHOICES{
        std::make_pair(ContactPointType::UNKNOWN, "UNKNOWN"),
        std::make_pair(ContactPointType::START, "start"),
        std::make_pair(ContactPointType::END, "end"),
    };

static const std::unordered_map<Dir, std::string> DIR_CHOICES{
    std::make_pair(Dir::UNKNOWN, "UNKNOWN"),
    std::make_pair(Dir::PLUS, "+"),
    std::make_pair(Dir::MINUS, "-"),
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CHOICES_H_
