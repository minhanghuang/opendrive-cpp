#include "opendrive-cpp/common/choices.h"

namespace opendrive {

const std::map<Boolean, std::string> BOOLEAN_CHOICES{
    std::make_pair(Boolean::kUnknown, "UNKNOWN"),
    std::make_pair(Boolean::kFalse, "false"),
    std::make_pair(Boolean::kTrue, "true"),
};

const std::map<GeometryType, std::string> GEOMETRY_TYPE_CHOICES{
    std::make_pair(GeometryType::kArc, "ARC"),
    std::make_pair(GeometryType::kLine, "LINE"),
    std::make_pair(GeometryType::kSpiral, "SPIRAL"),
    std::make_pair(GeometryType::kPoly3, "POLY3"),
    std::make_pair(GeometryType::kParamPoly3, "PARAMPOLY3"),
};

const std::map<LaneType, std::string> LANE_TYPE_CHOICES{
    std::make_pair(LaneType::kSholder, "shouldeR"),
    std::make_pair(LaneType::kBorder, "border"),
    std::make_pair(LaneType::kDriving, "driving"),
    std::make_pair(LaneType::kStop, "stop"),
    std::make_pair(LaneType::kNone, "none"),
    std::make_pair(LaneType::kRestricted, "restricted"),
    std::make_pair(LaneType::kParking, "parking"),
    std::make_pair(LaneType::kMedian, "median"),
    std::make_pair(LaneType::kBiking, "biking"),
    std::make_pair(LaneType::kSidewalk, "sidewalk"),
    std::make_pair(LaneType::kCurb, "curb"),
    std::make_pair(LaneType::kExit, "exit"),
    std::make_pair(LaneType::kEntry, "entry"),
    std::make_pair(LaneType::kOnramp, "onRamp"),
    std::make_pair(LaneType::kOfframp, "offRamp"),
    std::make_pair(LaneType::kConnectingramp, "connectingRamp"),
    std::make_pair(LaneType::kBidirectional, "bidirectional"),
    std::make_pair(LaneType::kSpecial1, "special1"),
    std::make_pair(LaneType::kSpecial2, "special2"),
    std::make_pair(LaneType::kSpecial3, "special3"),
    std::make_pair(LaneType::kRoadworks, "roadWorks"),
    std::make_pair(LaneType::kTram, "tram"),
    std::make_pair(LaneType::kRail, "rail"),
    std::make_pair(LaneType::kBus, "bus"),
    std::make_pair(LaneType::kTaxi, "taxi"),
    std::make_pair(LaneType::kHov, "HOV"),
    std::make_pair(LaneType::kMwyentry, "mwyEntry"),
    std::make_pair(LaneType::kMwyexit, "mwyExit"),
};

const std::map<RoadMarkType, std::string> ROADMARK_TYPE_CHOICES{
    std::make_pair(RoadMarkType::kNone, "none"),
    std::make_pair(RoadMarkType::kSolid, "solid"),
    std::make_pair(RoadMarkType::kBroken, "broken"),
    std::make_pair(RoadMarkType::kSolidsolid, "solid solid"),
    std::make_pair(RoadMarkType::kSolidbroken, "solid broken"),
    std::make_pair(RoadMarkType::kBrokensolid, "broken solid"),
    std::make_pair(RoadMarkType::kBrokenbroken, "broken broken"),
    std::make_pair(RoadMarkType::kBottsdots, "botts dots"),
    std::make_pair(RoadMarkType::kGrass, "grass"),
    std::make_pair(RoadMarkType::kCurb, "curb"),
    std::make_pair(RoadMarkType::kCustom, "custom"),
    std::make_pair(RoadMarkType::kEdge, "edge"),
};

const std::map<RoadMarkColor, std::string> ROAD_MARK_COLOR_CHOICES{
    std::make_pair(RoadMarkColor::kStandard, "standard"),
    std::make_pair(RoadMarkColor::kBlue, "blue"),
    std::make_pair(RoadMarkColor::kGreen, "green"),
    std::make_pair(RoadMarkColor::kRed, "red"),
    std::make_pair(RoadMarkColor::kWhite, "white"),
    std::make_pair(RoadMarkColor::kYellow, "yellow"),
    std::make_pair(RoadMarkColor::kOrange, "orange"),
};

const std::map<RoadMarkWeight, std::string> ROAD_MARK_WEIGHT_CHOICES{
    std::make_pair(RoadMarkWeight::kUnknown, "UNKNOWN"),
    std::make_pair(RoadMarkWeight::kStandard, "standard"),
    std::make_pair(RoadMarkWeight::kBold, "bold"),
};

const std::map<RoadMarkLaneChange, std::string> ROAD_MARK_LANE_CHANGE_CHOICES{
    std::make_pair(RoadMarkLaneChange::kUnknown, "UNKNOWN"),
    std::make_pair(RoadMarkLaneChange::kNone, "none"),
    std::make_pair(RoadMarkLaneChange::kIncrease, "increase"),
    std::make_pair(RoadMarkLaneChange::kDecrease, "decrease"),
    std::make_pair(RoadMarkLaneChange::kBoth, "both"),
};

const std::map<RoadRule, std::string> ROAD_RULE_CHOICES{
    std::make_pair(RoadRule::kRht, "RHT"),
    std::make_pair(RoadRule::kLht, "LHT"),
};

const std::map<RoadType, std::string> ROAD_TYPE_CHOICES{
    std::make_pair(RoadType::kRural, "rural"),
    std::make_pair(RoadType::kMotorway, "motorway"),
    std::make_pair(RoadType::kTown, "town"),
    std::make_pair(RoadType::kLowspeed, "lowspeed"),
    std::make_pair(RoadType::kPedestrian, "pedestrian"),
    std::make_pair(RoadType::kBicycle, "bicycle"),
    std::make_pair(RoadType::kTownexpressway, "townExpressway"),
    std::make_pair(RoadType::kTowncollector, "townCollector"),
    std::make_pair(RoadType::kTownarterial, "townArterial"),
    std::make_pair(RoadType::kTownprivate, "townPrivate"),
    std::make_pair(RoadType::kTownlocal, "townLocal"),
    std::make_pair(RoadType::kTownplaystreet, "townPlaystreet"),
};

const std::map<RoadLinkType, std::string> ROAD_LINK_TYPE_CHOICES{
    std::make_pair(RoadLinkType::kRoad, "road"),
    std::make_pair(RoadLinkType::kJunction, "junction"),
};

const std::map<SpeedUnit, std::string> SPEEDUNIT_CHOICES{
    std::make_pair(SpeedUnit::kMs, "m/s"),
    std::make_pair(SpeedUnit::kMph, "mph"),
    std::make_pair(SpeedUnit::kKmh, "km/h"),
};

const std::map<LaneDirection, std::string> LANE_DIRECTION_CHOICES{
    std::make_pair(LaneDirection::kUnknown, "UNKNOWN"),
    std::make_pair(LaneDirection::kLeft, "left"),
    std::make_pair(LaneDirection::kCenter, "center"),
    std::make_pair(LaneDirection::kRight, "right"),
};

const std::map<JunctionType, std::string> JUNCTION_TYPE_CHOICES{
    std::make_pair(JunctionType::kDefault, "default"),
    std::make_pair(JunctionType::kDirect, "direct"),
    std::make_pair(JunctionType::kVirtual, "virtual"),
};

const std::map<JunctionConnectionType, std::string>
    JUNCTION_CONNECTION_TYPE_CHOICES{
        std::make_pair(JunctionConnectionType::kUnknown, "UNKNOWN"),
        std::make_pair(JunctionConnectionType::kDefault, "default"),
        std::make_pair(JunctionConnectionType::kVirtual, "virtual"),
    };

const std::map<ContactPointType, std::string> CONTACT_POINT_TYPE_CHOICES{
    std::make_pair(ContactPointType::kUnknown, "UNKNOWN"),
    std::make_pair(ContactPointType::kStart, "start"),
    std::make_pair(ContactPointType::kEnd, "end"),
};

const std::map<Dir, std::string> DIR_CHOICES{
    std::make_pair(Dir::kUnknown, "UNKNOWN"),
    std::make_pair(Dir::kPlus, "+"),
    std::make_pair(Dir::kMinus, "-"),
};

}  // namespace opendrive
