#ifndef OPENDRIVE_CPP_HEADER_PARSER_H_
#define OPENDRIVE_CPP_HEADER_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/util_parser.h"

namespace opendrive {
namespace parser {

class HeaderXmlParser : public XmlParser {
 public:
  HeaderXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* ele_header,
                          element::Header* header);

 private:
  HeaderXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* ele_header_;
  element::Header* header_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_HEADER_PARSER_H_
