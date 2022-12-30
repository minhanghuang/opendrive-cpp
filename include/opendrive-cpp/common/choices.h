#ifndef OPENDRIVE_CPP_CHOICES_H_
#define OPENDRIVE_CPP_CHOICES_H_

#include <unordered_map>

#include "opendrive-cpp/geometry/enums.h"

namespace opendrive {

static const std::unordered_map<opendrive::LaneType, std::string>
    LANE_TYPE_CHOICES{
        std::make_pair(opendrive::LaneType::UNKNOWN, "UNKNOWN"),
        std::make_pair(opendrive::LaneType::SHOULDER, "SHOULDER"),
        std::make_pair(opendrive::LaneType::BORDER, "BORDER"),
        std::make_pair(opendrive::LaneType::DRIVING, "DRIVING"),
        std::make_pair(opendrive::LaneType::STOP, "STOP"),
        std::make_pair(opendrive::LaneType::NONE, "NONE"),
        std::make_pair(opendrive::LaneType::RESTRICTED, "RESTRICTED"),
        std::make_pair(opendrive::LaneType::PARKING, "PARKING"),
        std::make_pair(opendrive::LaneType::MEDIAN, "MEDIAN"),
        std::make_pair(opendrive::LaneType::BIKING, "BIKING"),
        std::make_pair(opendrive::LaneType::SIDEWALK, "SIDEWALK"),
        std::make_pair(opendrive::LaneType::CURB, "CURB"),
        std::make_pair(opendrive::LaneType::EXIT, "EXIT"),
        std::make_pair(opendrive::LaneType::ENTRY, "ENTRY"),
        std::make_pair(opendrive::LaneType::ONRAMP, "ONRAMP"),
        std::make_pair(opendrive::LaneType::OFFRAMP, "OFFRAMP"),
        std::make_pair(opendrive::LaneType::CONNECTINGRAMP, "CONNECTINGRAMP"),
        std::make_pair(opendrive::LaneType::BIDIRECTIONAL, "BIDIRECTIONAL"),
        std::make_pair(opendrive::LaneType::SPECIAL1, "SPECIAL1"),
        std::make_pair(opendrive::LaneType::SPECIAL2, "SPECIAL2"),
        std::make_pair(opendrive::LaneType::SPECIAL3, "SPECIAL3"),
        std::make_pair(opendrive::LaneType::ROADWORKS, "ROADWORKS"),
        std::make_pair(opendrive::LaneType::TRAM, "TRAM"),
        std::make_pair(opendrive::LaneType::RAIL, "RAIL"),
        std::make_pair(opendrive::LaneType::BUS, "BUS"),
        std::make_pair(opendrive::LaneType::TAXI, "TAXI"),
        std::make_pair(opendrive::LaneType::HOV, "HOV"),
        std::make_pair(opendrive::LaneType::MWYENTRY, "MWYENTRY"),
        std::make_pair(opendrive::LaneType::MWYEXIT, "MWYEXIT"),
    };

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_CHOICES_H_
