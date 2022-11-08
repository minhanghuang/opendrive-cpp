#ifndef OPENDRIVE_CPP_ADAPTER_H_
#define OPENDRIVE_CPP_ADAPTER_H_

#include <mutex>

#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/parser/road_xml_parser.h"
#include "opendrive-cpp/core/section.h"

namespace opendrive {
namespace adapter {

class AdapterBase {
 public:
  AdapterBase();
  virtual bool IsValid() final;
  virtual opendrive::Status status() final;
  virtual void set_status(ErrorCode code, const std::string& msg) final;
  virtual void Init() = 0;

 private:
  std::mutex mutex_;
  bool next_ = true;
  opendrive::Status status_{ErrorCode::OK, "ok"};
};

}  // namespace adapter
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_ADAPTER_H_
