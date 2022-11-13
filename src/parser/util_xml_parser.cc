#include "opendrive-cpp/parser/util_xml_parser.h"

namespace opendrive {
namespace parser {

bool XmlParser::IsValid() { return ErrorCode::OK == status_.error_code; }

opendrive::Status XmlParser::status() { return status_; }

void XmlParser::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

}  // namespace parser
}  // namespace opendrive
