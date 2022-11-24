#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

bool XmlParser::IsValid() const { return ErrorCode::OK == status_.error_code; }

opendrive::Status XmlParser::status() const { return status_; }

void XmlParser::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

void XmlParser::set_status(const Status& s) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = s.error_code;
  status_.msg = s.msg;
}

void XmlParser::set_status(Status&& s) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = s.error_code;
  status_.msg = s.msg;
}

void XmlParser::CheckStatus(ErrorCode code, const std::string& msg) {
  if (ErrorCode::OK != code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = code;
    status_.msg = msg;
  }
}

void XmlParser::CheckStatus(const Status& s) {
  if (ErrorCode::OK != s.error_code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = s.error_code;
    status_.msg = s.msg;
  }
}

void XmlParser::CheckStatus(Status&& s) {
  if (ErrorCode::OK != s.error_code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = s.error_code;
    status_.msg = s.msg;
  }
}

}  // namespace parser
}  // namespace opendrive
