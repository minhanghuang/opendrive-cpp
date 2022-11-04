#ifndef OPENDRIVE_CPP_HEADER_XML_PARSER_H_
#define OPENDRIVE_CPP_HEADER_XML_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "parser.h"

namespace opendrive {
namespace parser {

class HeaderXmlParser : public XmlParser {
 public:
  opendrive::Status Parse(const tinyxml2::XMLElement* header_ele,
                          core::Header::Ptr header_ptr);

 private:
  virtual void Init() override;
  HeaderXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* header_ele_;
  core::Header::Ptr header_ptr_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_HEADER_XML_PARSER_H_
