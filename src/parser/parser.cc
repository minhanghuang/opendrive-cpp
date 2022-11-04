#include "opendrive-cpp/parser/parser.h"

#include <mutex>

namespace opendrive {
namespace parser {

XmlParser::XmlParser() {}

bool XmlParser::IsValid() {
  if (ErrorCode::OK != status_.error_code) {
    return false;
  }
  return true;
}

opendrive::Status XmlParser::status() { return status_; }

void XmlParser::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

}  // namespace parser
}  // namespace opendrive
