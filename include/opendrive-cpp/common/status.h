#ifndef OPENDRIVE_CPP_STATUS_H_
#define OPENDRIVE_CPP_STATUS_H_

#include <string>

namespace opendrive {

enum class ErrorCode {
  OK = 0,

  /// xml parser status
  XML_HEADER_ELEMENT_ERROR = 10000,
  XML_ROAD_ELEMENT_ERROR = 10001,
  XML_LANES_ELEMENT_ERROR = 10002,

  /// adapter
  ADAPTER_LINE_ERROR = 20000,
  ADAPTER_LANE_ERROR = 20001,
  ADAPTER_SECTION_ERROR = 20002,
  ADAPTER_ROAD_ERROR = 20003,
  ADAPTER_GEOMETRY_ERROR = 20004,

};

struct Status {
  Status(ErrorCode code, const std::string& m) : error_code(code), msg(m) {}
  ErrorCode error_code = ErrorCode::OK;
  std::string msg;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_STATUS_H_
