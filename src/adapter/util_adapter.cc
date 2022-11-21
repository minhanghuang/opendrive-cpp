#include "opendrive-cpp/adapter/util_adapter.h"

namespace opendrive {
namespace adapter {

UtilAdapter::UtilAdapter() : step_(0.5) {}

bool UtilAdapter::IsValid() const {
  return ErrorCode::OK == status_.error_code;
}

opendrive::Status UtilAdapter::status() const { return status_; }

void UtilAdapter::set_status(ErrorCode code, const std::string& msg) {
  std::unique_lock<std::mutex> lock(mutex_);
  status_.error_code = code;
  status_.msg = msg;
}

float UtilAdapter::step() const { return step_; }

void UtilAdapter::set_step(float num) { step_ = num; }

void UtilAdapter::CheckStatus(ErrorCode code, const std::string& msg) {
  if (ErrorCode::OK == code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = code;
    status_.msg = msg;
  }
}

void UtilAdapter::CheckStatus(const Status& s) {
  if (ErrorCode::OK == s.error_code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = s.error_code;
    status_.msg = s.msg;
  }
}

void UtilAdapter::CheckStatus(Status&& s) {
  if (ErrorCode::OK == s.error_code) {
    std::unique_lock<std::mutex> lock(mutex_);
    status_.error_code = s.error_code;
    status_.msg = s.msg;
  }
}

}  // namespace adapter
}  // namespace opendrive
