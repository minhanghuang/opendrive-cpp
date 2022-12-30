#ifndef OPENDRIVE_CPP_CHOICES_H_
#define OPENDRIVE_CPP_CHOICES_H_

#include <unordered_map>
#include <utility>

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {

static const std::unordered_map<Boolean, std::string> BOOLEAN_CHOICES{
    std::make_pair(Boolean::UNKNOWN, "UNKNOWN"),
    std::make_pair(Boolean::FALSE, "FALSE"),
    std::make_pair(Boolean::TRUE, "TRUE"),
};

static const std::unordered_map<GeometryType, std::string>
    GEOMETRY_TYPE_CHOICES{
        std::make_pair(GeometryType::UNKNOWN, "UNKNOWN"),
        std::make_pair(GeometryType::ARC, "ARC"),
        std::make_pair(GeometryType::LINE, "LINE"),
        std::make_pair(GeometryType::SPIRAL, "SPIRAL"),
        std::make_pair(GeometryType::POLY3, "POLY3"),
        std::make_pair(GeometryType::PARAMPOLY3, "PARAMPOLY3"),
    };

static const std::unordered_map<LaneType, std::string> LANE_TYPE_CHOICES{
    std::make_pair(LaneType::UNKNOWN, "UNKNOWN"),
    std::make_pair(LaneType::SHOULDER, "SHOULDER"),
    std::make_pair(LaneType::BORDER, "BORDER"),
    std::make_pair(LaneType::DRIVING, "DRIVING"),
    std::make_pair(LaneType::STOP, "STOP"),
    std::make_pair(LaneType::NONE, "NONE"),
    std::make_pair(LaneType::RESTRICTED, "RESTRICTED"),
    std::make_pair(LaneType::PARKING, "PARKING"),
    std::make_pair(LaneType::MEDIAN, "MEDIAN"),
    std::make_pair(LaneType::BIKING, "BIKING"),
    std::make_pair(LaneType::SIDEWALK, "SIDEWALK"),
    std::make_pair(LaneType::CURB, "CURB"),
    std::make_pair(LaneType::EXIT, "EXIT"),
    std::make_pair(LaneType::ENTRY, "ENTRY"),
    std::make_pair(LaneType::ONRAMP, "ONRAMP"),
    std::make_pair(LaneType::OFFRAMP, "OFFRAMP"),
    std::make_pair(LaneType::CONNECTINGRAMP, "CONNECTINGRAMP"),
    std::make_pair(LaneType::BIDIRECTIONAL, "BIDIRECTIONAL"),
    std::make_pair(LaneType::SPECIAL1, "SPECIAL1"),
    std::make_pair(LaneType::SPECIAL2, "SPECIAL2"),
    std::make_pair(LaneType::SPECIAL3, "SPECIAL3"),
    std::make_pair(LaneType::ROADWORKS, "ROADWORKS"),
    std::make_pair(LaneType::TRAM, "TRAM"),
    std::make_pair(LaneType::RAIL, "RAIL"),
    std::make_pair(LaneType::BUS, "BUS"),
    std::make_pair(LaneType::TAXI, "TAXI"),
    std::make_pair(LaneType::HOV, "HOV"),
    std::make_pair(LaneType::MWYENTRY, "MWYENTRY"),
    std::make_pair(LaneType::MWYEXIT, "MWYEXIT"),
};

static const std::unordered_map<opendrive::RoadMarkType, std::string>
    ROADMARK_TYPE_CHOICES{
        std::make_pair(RoadMarkType::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkType::NONE, "NONE"),
        std::make_pair(RoadMarkType::SOLID, "SOLID"),
        std::make_pair(RoadMarkType::BROKEN, "BROKEN"),
        std::make_pair(RoadMarkType::SOLIDSOLID, "SOLIDSOLID"),
        std::make_pair(RoadMarkType::SOLIDBROKEN, "SOLIDBROKEN"),
        std::make_pair(RoadMarkType::BROKENSOLID, "BROKENSOLID"),
        std::make_pair(RoadMarkType::BROKENBROKEN, "BROKENBROKEN"),
        std::make_pair(RoadMarkType::BOTTSDOTS, "BOTTSDOTS"),
        std::make_pair(RoadMarkType::GRASS, "GRASS"),
        std::make_pair(RoadMarkType::CURB, "CURB"),
        std::make_pair(RoadMarkType::CUSTOM, "CUSTOM"),
        std::make_pair(RoadMarkType::EDGE, "EDGE"),
    };

static const std::unordered_map<RoadMarkColor, std::string>
    ROAD_MARK_COLOR_CHOICES{
        std::make_pair(RoadMarkColor::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkColor::STANDARD, "STANDARD"),
        std::make_pair(RoadMarkColor::BLUE, "BLUE"),
        std::make_pair(RoadMarkColor::GREEN, "GREEN"),
        std::make_pair(RoadMarkColor::RED, "RED"),
        std::make_pair(RoadMarkColor::WHITE, "WHITE"),
        std::make_pair(RoadMarkColor::YELLOW, "YELLOW"),
        std::make_pair(RoadMarkColor::ORANGE, "ORANGE"),
    };

static const std::unordered_map<RoadMarkWeight, std::string>
    ROAD_MARK_WEIGHT_CHOICES{
        std::make_pair(RoadMarkWeight::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkWeight::STANDARD, "STANDARD"),
        std::make_pair(RoadMarkWeight::BOLD, "BOLD"),
    };

static const std::unordered_map<RoadMarkLaneChange, std::string>
    ROAD_MARK_LANE_CHANGE_CHOICES{
        std::make_pair(RoadMarkLaneChange::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadMarkLaneChange::NONE, "NONE"),
        std::make_pair(RoadMarkLaneChange::INCREASE, "INCREASE"),
        std::make_pair(RoadMarkLaneChange::DECREASE, "DECREASE"),
        std::make_pair(RoadMarkLaneChange::BOTH, "BOTH"),
    };

static const std::unordered_map<RoadRule, std::string> ROAD_RULE_CHOICES{
    std::make_pair(RoadRule::RHT, "RHT"),
    std::make_pair(RoadRule::LHT, "LHT"),
};

static const std::unordered_map<RoadType, std::string> ROAD_TYPE_CHOICES{
    std::make_pair(RoadType::UNKNOWN, "UNKNOWN"),
    std::make_pair(RoadType::RURAL, "RURAL"),
    std::make_pair(RoadType::MOTORWAY, "MOTORWAY"),
    std::make_pair(RoadType::TOWN, "TOWN"),
    std::make_pair(RoadType::LOWSPEED, "LOWSPEED"),
    std::make_pair(RoadType::PEDESTRIAN, "PEDESTRIAN"),
    std::make_pair(RoadType::BICYCLE, "BICYCLE"),
    std::make_pair(RoadType::TOWNEXPRESSWAY, "TOWNEXPRESSWAY"),
    std::make_pair(RoadType::TOWNCOLLECTOR, "TOWNCOLLECTOR"),
    std::make_pair(RoadType::TOWNARTERIAL, "TOWNARTERIAL"),
    std::make_pair(RoadType::TOWNPRIVATE, "TOWNPRIVATE"),
    std::make_pair(RoadType::TOWNLOCAL, "TOWNLOCAL"),
    std::make_pair(RoadType::TOWNPLAYSTREET, "TOWNPLAYSTREET"),
};

static const std::unordered_map<RoadLinkType, std::string>
    ROAD_LINK_TYPE_CHOICES{
        std::make_pair(RoadLinkType::UNKNOWN, "UNKNOWN"),
        std::make_pair(RoadLinkType::ROAD, "ROAD"),
        std::make_pair(RoadLinkType::JUNCTION, "JUNCTION"),
    };

static const std::unordered_map<SpeedUnit, std::string> SPEEDUNIT_CHOICES{
    std::make_pair(SpeedUnit::UNKNOWN, "UNKNOWN"),
    std::make_pair(SpeedUnit::MS, "m/s"),
    std::make_pair(SpeedUnit::MPH, "mph"),
    std::make_pair(SpeedUnit::KMH, "km/h"),
};

static const std::unordered_map<LaneDirection, std::string>
    LANE_DIRECTION_CHOICES{
        std::make_pair(LaneDirection::UNKNOWN, "UNKNOWN"),
        std::make_pair(LaneDirection::LEFT, "LEFT"),
        std::make_pair(LaneDirection::CENTER, "CENTER"),
        std::make_pair(LaneDirection::RIGHT, "RIGHT"),
    };

static const std::unordered_map<JunctionType, std::string>
    JUNCTION_TYPE_CHOICES{
        std::make_pair(JunctionType::DEFAULT, "UNKNOWN"),
        std::make_pair(JunctionType::DIRECT, "DIRECT"),
        std::make_pair(JunctionType::VIRTUAL, "VIRTUAL"),
    };

static const std::unordered_map<JunctionConnectionType, std::string>
    JUNCTION_CONNECTION_TYPE_CHOICES{
        std::make_pair(JunctionConnectionType::UNKNOWN, "UNKNOWN"),
        std::make_pair(JunctionConnectionType::DEFAULT, "DEFAULT"),
        std::make_pair(JunctionConnectionType::VIRTUAL, "VIRTUAL"),
    };

static const std::unordered_map<ContactPointType, std::string>
    CONTACT_POINT_TYPE_CHOICES{
        std::make_pair(ContactPointType::UNKNOWN, "UNKNOWN"),
        std::make_pair(ContactPointType::START, "START"),
        std::make_pair(ContactPointType::END, "END"),
    };

static const std::unordered_map<Dir, std::string> DIR_CHOICES{
    std::make_pair(Dir::UNKNOWN, "UNKNOWN"),
    std::make_pair(Dir::PLUS, "+"),
    std::make_pair(Dir::MINUS, "-"),
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CHOICES_H_
