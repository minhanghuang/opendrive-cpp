#include "opendrive-cpp/adapter/adapter.h"

namespace opendrive {
namespace adapter {

AdapterBase::AdapterBase() {}

bool AdapterBase::IsValid() {
  if (ErrorCode::OK != status_.error_code) {
    return false;
  }
  return true;
}

opendrive::Status AdapterBase::status() { return status_; }

void AdapterBase::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

}  // namespace adapter
}  // namespace opendrive
