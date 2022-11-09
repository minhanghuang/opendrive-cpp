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

void UtilAdapter::CalcLinePoint(double& x, double& y, double start_x,
                                double start_y, double hdg, double length) {
  x = start_x + length * std::cos(hdg);
  y = start_y + length * std::sin(hdg);
}

void UtilAdapter::CalcArcPoint(double& x, double& y, double start_x,
                               double start_y, double hdg, double length,
                               double curvature) {
  hdg = hdg - M_PI / 2.0;
  double a = 2.0 / curvature * std::sin(length * curvature / 2.0);
  double alpha = (M_PI - length * curvature) / 2.0 - hdg;
  x = -1 * a * cos(alpha) + start_x;
  y = a * sin(alpha) + start_y;
}

}  // namespace adapter
}  // namespace opendrive
