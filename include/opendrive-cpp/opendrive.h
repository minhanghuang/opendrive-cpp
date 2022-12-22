#ifndef OPENDRIVE_CPP_H_
#define OPENDRIVE_CPP_H_
#include <tinyxml2.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/parser/adapter.h"
#include "opendrive-cpp/parser/map_parser.h"
#include "opendrive-cpp/parser/road_parser.h"
#include "opendrive-cpp/parser/section_parser.h"

namespace opendrive {

class Parser {
 public:
  ~Parser() = default;
  Parser();
  std::string GetOpenDriveVersion() const;
  opendrive::Status ParseMap(const std::string& xml_file,
                             element::Map* ele_map);
  opendrive::Status ParseMap(const tinyxml2::XMLElement* xml_root,
                             element::Map* ele_map);
  opendrive::Status Adapter(const element::Map* ele_map, core::Map::Ptr map_ptr,
                            float step = 0.5);
  opendrive::Status SaveData(core::Map::Ptr map_ptr,
                             const std::string& file_path);

 private:
  tinyxml2::XMLDocument xml_doc_;
  std::unique_ptr<parser::MapXmlParser> map_parser_;
  std::unique_ptr<adapter::AdapterMap> adapter_;
};

}  // namespace opendrive

#endif  // OPENDRIVE_CPP_H_
