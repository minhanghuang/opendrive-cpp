#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <memory>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/core/types.hpp"
#include "opendrive-cpp/parser/header_xml_parser.h"
#include "opendrive-cpp/parser/road_xml_parser.h"

namespace opendrive {

class Parser {
 public:
  ~Parser();
  Parser();
  opendrive::Status Header(const tinyxml2::XMLElement* xml_node,
                           core::Header::Ptr header_ptr);
  opendrive::Status Road(const tinyxml2::XMLElement* xml_node,
                         core::Road::Ptr road_ptr);

 private:
  std::shared_ptr<parser::HeaderXmlParser> header_parser_;
  std::shared_ptr<parser::RoadXmlParser> road_parser_;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_H_
