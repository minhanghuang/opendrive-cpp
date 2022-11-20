#ifndef OPENDRIVE_CPP_PARSER_H_
#define OPENDRIVE_CPP_PARSER_H_
#include <tinyxml2.h>

#include <memory>
#include <mutex>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"

namespace opendrive {
namespace parser {

class XmlParser {
 public:
  XmlParser() = default;
  virtual bool IsValid() const final;
  virtual opendrive::Status status() const final;
  virtual void set_status(ErrorCode code, const std::string& msg) final;
  virtual void set_status(const Status& s) final;
  virtual void set_status(Status&& s) final;
  virtual void update_status(ErrorCode code, const std::string& msg) final;
  virtual void update_status(const Status& s) final;
  virtual void update_status(Status&& s) final;

 private:
  std::mutex mutex_;
  opendrive::Status status_{ErrorCode::OK, "ok"};
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_PARSER_H_
