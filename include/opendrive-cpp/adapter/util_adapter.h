#ifndef OPENDRIVE_CPP_ADAPTER_H_
#define OPENDRIVE_CPP_ADAPTER_H_

#include <cmath>
#include <mutex>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/parser/road_parser.h"

namespace opendrive {
namespace adapter {

class UtilAdapter {
 public:
  UtilAdapter();
  virtual bool IsValid() const final;
  virtual opendrive::Status status() const final;
  virtual void set_status(ErrorCode code, const std::string& msg) final;
  virtual float step() const final;
  virtual void set_step(float num) final;
  virtual void CheckStatus(ErrorCode code, const std::string& msg) final;
  virtual void CheckStatus(const Status& s) final;
  virtual void CheckStatus(Status&& s) final;

 private:
  std::mutex mutex_;
  float step_;
  opendrive::Status status_{ErrorCode::OK, "ok"};
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_H_
