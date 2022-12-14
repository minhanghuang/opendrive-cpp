#ifndef OPENDRIVE_CPP_STATUS_H_
#define OPENDRIVE_CPP_STATUS_H_

#include <string>

namespace opendrive {

enum class ErrorCode {
  OK = 0,

  /// xml
  XML_ROOT_ELEMENT_ERROR = 1000,
  XML_HEADER_ELEMENT_ERROR,
  XML_ROAD_ELEMENT_ERROR,
  XML_ROAD_LINK_ELEMENT_ERROR,
  XML_ROAD_PLANVIEW_ELEMENT_ERROR,
  XML_ROAD_TYPE_ELEMENT_ERROR,
  XML_LANES_ELEMENT_ERROR,
  XML_LANES_OFFSET_ELEMENT_ERROR,
  XML_LANES_SECTION_ELEMENT_ERROR,
  XML_JUNCTION_ELEMENT_ERROR,

  /// adapter
  ADAPTER_ROOT_ERROR = 2000,
  ADAPTER_HEADER_ERROR,
  ADAPTER_JUNCTION_ERROR,
  ADAPTER_ROAD_ERROR,
  ADAPTER_LINE_ERROR,
  ADAPTER_LANE_ERROR,
  ADAPTER_SECTION_ERROR,
  ADAPTER_GEOMETRY_ERROR,
  ADAPTER_ROADTYPE_ERROR,

  SAVE_DATA_ERROR,
};

struct Status {
  Status() = default;
  Status(ErrorCode code, const std::string& m) : error_code(code), msg(m) {}
  ErrorCode error_code = ErrorCode::OK;
  std::string msg;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_STATUS_H_
