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

void UtilAdapter::CalcLinePoint(double& out_x, double& out_y, double x,
                                double y, double hdg, double length,
                                double distance) {
  distance = common::Clamp<double>(distance, 0, length);
  out_x = x + distance * std::cos(hdg);
  out_y = y + distance * std::sin(hdg);
}

void UtilAdapter::CalcArcPoint(double& out_x, double& out_y, double& out_hdg,
                               double x, double y, double hdg, double length,
                               double curvature, double distance) {
  distance = common::Clamp<double>(distance, 0, length);
  double radius = 1.0 / curvature;
  double pi_half = M_PI / 2.0;
  out_x = x + radius * std::cos(hdg + pi_half);
  out_y = y + radius * std::sin(hdg + pi_half);
  hdg = hdg + distance * curvature;
  out_x = out_x - radius * std::cos(hdg + pi_half);
  out_y = out_y - radius * std::sin(hdg + pi_half);
  out_hdg = hdg;
}

void UtilAdapter::CalcSpiralPoint(double& out_x, double& out_y, double& out_hdg,
                                  double x, double y, double hdg, double length,
                                  double curve_start, double curve_end,
                                  double distance) {
  distance = common::Clamp<double>(distance, 0, length);
  const double curve_dot = (curve_end - curve_start) / (length);
  const double s_o = curve_start / curve_dot;
  double s = s_o + distance;
  double sx;
  double sy;
  double st;
  OdrSpiral(s, curve_dot, &sx, &sy, &st);
  double sx_o;
  double sy_o;
  double st_o;
  OdrSpiral(s_o, curve_dot, &sx_o, &sy_o, &st_o);
  sx = sx - sx_o;
  sy = sy - sy_o;
  st = st - st_o;
  double shdg = hdg - st_o;
  out_x = x + (sx * std::cos(shdg) - sy * std::sin(shdg));
  out_y = y + (sy * std::cos(shdg) + sx * std::sin(shdg));
  out_hdg = hdg + st;
}

void UtilAdapter::CalcPoly3Point(double& out_x, double& out_y, double s,
                                 double x, double y, double hdg, double length,
                                 double a, double b, double c, double d,
                                 double distance) {
  distance = common::Clamp<double>(distance, 0, length);
  double u = distance - s;
  double v = a + b * u + c * u * u + d * u * u * u;
  out_x = x + u * std::cos(hdg) + v * (-std::sin(hdg));
  out_y = y + u * std::sin(hdg) + v * (std::cos(hdg));
}

void UtilAdapter::CalcParamPoly3Point(double& out_x, double& out_y, double s,
                                      double x, double y, double hdg,
                                      double length, double au, double bu,
                                      double cu, double du, double av,
                                      double bv, double cv, double dv,
                                      double distance) {
  distance = common::Clamp<double>(distance, 0, length);
  double param = distance - s;
  double u = au + bu * param + cu * param * param + du * param * param * param;
  double v = av + bv * param + cv * param * param + dv * param * param * param;
  out_x = x + u * std::cos(hdg) + v * (-std::sin(hdg));
  out_y = y + u * std::sin(hdg) + v * (std::cos(hdg));
}

void UtilAdapter::OdrSpiral(double s, double cDot, double* x, double* y,
                            double* t) {
  double a;

  a = 1.0 / sqrt(fabs(cDot));
  a *= sqrt(M_PI);

  fresnel(s / a, y, x);

  *x *= a;
  *y *= a;

  if (cDot < 0.0) *y *= -1.0;

  *t = s * s * cDot * 0.5;
}

}  // namespace adapter
}  // namespace opendrive
