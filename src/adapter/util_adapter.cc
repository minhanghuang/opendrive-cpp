#include "opendrive-cpp/adapter/util_adapter.h"

namespace opendrive {
namespace adapter {

UtilAdapter::UtilAdapter() {}

bool UtilAdapter::IsValid() {
  if (ErrorCode::OK != status_.error_code) {
    return false;
  }
  return true;
}

opendrive::Status UtilAdapter::status() { return status_; }

void UtilAdapter::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

}  // namespace adapter
}  // namespace opendrive
