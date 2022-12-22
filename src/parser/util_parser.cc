#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

XmlParser::XmlParser(const std::string& version)
    : opendrive_version_(version) {}

void XmlParser::set_opendrive_version(const std::string& version) {
  opendrive_version_ = version;
}

std::string XmlParser::opendrive_version() const { return opendrive_version_; }

bool XmlParser::IsValid() const { return ErrorCode::OK == status_.error_code; }

opendrive::Status XmlParser::status() const { return status_; }

void XmlParser::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

bool XmlParser::CheckStatus(const Status& s) {
  if (ErrorCode::OK != s.error_code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = s.error_code;
    status_.msg = s.msg;
    return false;
  }
  return true;
}

}  // namespace parser
}  // namespace opendrive
