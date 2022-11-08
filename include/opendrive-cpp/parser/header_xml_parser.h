#ifndef OPENDRIVE_CPP_HEADER_XML_PARSER_H_
#define OPENDRIVE_CPP_HEADER_XML_PARSER_H_

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/core/base.h"
#include "opendrive-cpp/core/types.h"
#include "parser.h"

namespace opendrive {
namespace parser {

class HeaderXmlParser : public XmlParser {
 public:
  HeaderXmlParser() = default;
  opendrive::Status Parse(const tinyxml2::XMLElement* header_ele,
                          base::Header* header);

 private:
  virtual void Init() override;
  HeaderXmlParser& ParseAttributes();
  const tinyxml2::XMLElement* header_ele_;
  base::Header* header_;
};

}  // namespace parser
}  // namespace opendrive

#endif  // OPENDRIVE_CPP_HEADER_XML_PARSER_H_
