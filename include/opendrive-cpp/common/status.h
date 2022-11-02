#ifndef OPENDRIVE_CPP_STATUS_H_
#define OPENDRIVE_CPP_STATUS_H_

#include <string>

namespace opendrive {

enum class ErrorCode {
  OK = 0,
  /// xml parser status
  XML_HEADER_ELEMENT_ERROR = 10000,
};

struct Status {
  Status(ErrorCode code, const std::string& m) : error_code(code), msg(m) {}
  ErrorCode error_code = ErrorCode::OK;
  std::string msg;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_STATUS_H_
