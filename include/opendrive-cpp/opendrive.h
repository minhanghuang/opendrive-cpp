#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/map_parser.h"
#include "opendrive-cpp/parser/road_parser.h"
#include "opendrive-cpp/parser/section_parser.h"

namespace opendrive {

typedef class Parser ParserType;
class Parser {
 public:
  typedef std::shared_ptr<ParserType> Ptr;
  ~Parser() = default;
  Parser();
  std::string GetOpenDriveVersion() const;
  opendrive::Status ParseMap(const std::string& xml_file,
                             element::Map::Ptr ele_map);
  opendrive::Status ParseMap(const tinyxml2::XMLElement* xml_root,
                             element::Map::Ptr ele_map);

 private:
  tinyxml2::XMLDocument xml_doc_;
  std::unique_ptr<parser::MapXmlParser> map_parser_;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_H_
