#include "opendrive-cpp/opendrive.h"

#include <memory>

namespace opendrive {

Parser::Parser()
    : map_parser_(std::make_unique<parser::MapXmlParser>()),
      header_parser_(std::make_unique<parser::HeaderXmlParser>()),
      road_parser_(std::make_unique<parser::RoadXmlParser>()),
      road_link_parser_(std::make_unique<parser::RoadLinkXmlParser>()),
      road_type_parser_(std::make_unique<parser::RoadTypeXmlParser>()),
      road_planview_parser_(std::make_unique<parser::RoadPlanViewXmlParser>()),
      road_lanes_parser_(std::make_unique<parser::RoadLanesXmlParser>()),
      junction_parser_(std::make_unique<parser::JunctionXmlParser>()) {}

opendrive::Status Parser::Map(const std::string& xml_file,
                              element::Map* ele_map) {
  tinyxml2::XMLDocument xml_doc;
  xml_doc.LoadFile(xml_file.c_str());
  if (xml_doc.Error()) {
    return Status{ErrorCode::XML_ROAD_ELEMENT_ERROR,
                  "Parse Xml File Exection."};
  }
  tinyxml2::XMLElement* xml_root = xml_doc.RootElement();
  return map_parser_->Parse(xml_root, ele_map);
}

opendrive::Status Parser::Map(const tinyxml2::XMLElement* xml_root,
                              element::Map* ele_map) {
  return map_parser_->Parse(xml_root, ele_map);
}

opendrive::Status Parser::Header(const tinyxml2::XMLElement* xml_header,
                                 element::Header* header) {
  return header_parser_->Parse(xml_header, header);
}

opendrive::Status Parser::Road(const tinyxml2::XMLElement* xml_road,
                               element::Road* road) {
  return road_parser_->Parse(xml_road, road);
}

opendrive::Status Parser::RoadLink(const tinyxml2::XMLElement* xml_road_link,
                                   element::RoadLink* ele_road_link) {
  return road_link_parser_->Parse(xml_road_link, ele_road_link);
}

opendrive::Status Parser::RoadType(const tinyxml2::XMLElement* xml_road_type,
                                   element::RoadTypeInfo* ele_road_type) {
  return road_type_parser_->Parse(xml_road_type, ele_road_type);
}

opendrive::Status Parser::RoadPlanView(
    const tinyxml2::XMLElement* xml_road_planview,
    element::RoadPlanView* ele_road_planview) {
  return road_planview_parser_->Parse(xml_road_planview, ele_road_planview);
}

opendrive::Status Parser::RoadLanes(const tinyxml2::XMLElement* xml_road_lanes,
                                    element::Lanes* ele_road_lanes) {
  return road_lanes_parser_->Parse(xml_road_lanes, ele_road_lanes);
}

opendrive::Status Parser::Junction(const tinyxml2::XMLElement* xml_junction,
                                   element::Junction* ele_junction) {
  return junction_parser_->Parse(xml_junction, ele_junction);
}

Adapter::Adapter()
    : map_adapter_(std::make_unique<adapter::MapAdapter>()),
      road_adapter_(std::make_unique<adapter::RoadAdapter>()) {}

opendrive::Status Adapter::Map(const element::Map* ele_map,
                               core::Map::Ptr map_ptr, float step) {
  if (step <= 0) {
    return opendrive::Status{ErrorCode::ADAPTER_ROAD_ERROR, "Input Execption."};
  }
  map_adapter_->set_step(step);
  return map_adapter_->Run(ele_map, map_ptr);
}

opendrive::Status Adapter::Road(const element::Road* ele_road,
                                core::Road::Ptr road_ptr, float step) {
  if (step <= 0) {
    return opendrive::Status{ErrorCode::ADAPTER_ROAD_ERROR, "Input Execption."};
  }

  road_adapter_->set_step(step);
  return road_adapter_->Run(ele_road, road_ptr);
}

opendrive::Status Adapter::SaveData(core::Map::Ptr map_ptr,
                                    const std::string& file_path) {
  opendrive::Status status;
  if (nullptr == map_ptr || file_path.empty()) {
    status.error_code = ErrorCode::SAVE_DATA_ERROR;
    return status;
  }
  const char* declaration = "<?xml version=\"1.0\" standalone=\"yes\"?>";
  xml_doc_.Parse(declaration);
  tinyxml2::XMLElement* ele_root = xml_doc_.NewElement("OpenDRIVE");
  xml_doc_.InsertEndChild(ele_root);
  tinyxml2::XMLElement* ele_head = xml_doc_.NewElement("header");
  /// head
  auto head = map_ptr->header;
  if (head) {
    ele_head->SetAttribute("revMajor", head->rev_major.c_str());
    ele_head->SetAttribute("revMinor", head->rev_minor.c_str());
    ele_head->SetAttribute("name", head->name.c_str());
    ele_head->SetAttribute("version", head->version.c_str());
    ele_head->SetAttribute("date", head->date.c_str());
    ele_head->SetAttribute("north", head->north);
    ele_head->SetAttribute("south", head->south);
    ele_head->SetAttribute("east", head->east);
    ele_head->SetAttribute("west", head->west);
    ele_head->SetAttribute("vendor", head->vendor.c_str());
  }
  ele_root->InsertEndChild(ele_head);

  /// roads
  std::string road_id;
  std::string section_id;
  std::string lane_id;
  for (const auto& road : map_ptr->roads) {
    tinyxml2::XMLElement* ele_road = xml_doc_.NewElement("road");
    ele_root->InsertEndChild(ele_road);
    ele_road->SetAttribute("id", road.first.c_str());
    ele_road->SetAttribute("length", road.second->length);
    for (const auto& section : road.second->sections) {
      tinyxml2::XMLElement* ele_section = xml_doc_.NewElement("section");
      ele_road->InsertEndChild(ele_section);
      ele_section->SetAttribute("id", section->id.c_str());
      tinyxml2::XMLElement* ele_reference =
          xml_doc_.NewElement("reference_line");
      ele_section->InsertEndChild(ele_reference);
      for (const auto& point : section->center_line->line.points) {
        tinyxml2::XMLElement* ele_point = xml_doc_.NewElement("point");
        ele_reference->InsertEndChild(ele_point);
        ele_point->SetAttribute("x", point.x);
        ele_point->SetAttribute("y", point.y);
        ele_point->SetAttribute("s", point.s);
        ele_point->SetAttribute("heading", point.hdg);
      }
      for (const auto& lane : section->lanes) {
        tinyxml2::XMLElement* ele_lane = xml_doc_.NewElement("lane");
        ele_section->InsertEndChild(ele_lane);
        ele_lane->SetAttribute("id", lane->id.c_str());
        tinyxml2::XMLElement* ele_left_boundary =
            xml_doc_.NewElement("left_boundary");
        ele_lane->InsertEndChild(ele_left_boundary);
        tinyxml2::XMLElement* ele_right_boundary =
            xml_doc_.NewElement("right_boundary");
        ele_lane->InsertEndChild(ele_right_boundary);
        for (const auto& point : lane->left_boundary.line.points) {
          tinyxml2::XMLElement* ele_point = xml_doc_.NewElement("point");
          ele_left_boundary->InsertEndChild(ele_point);
          ele_point->SetAttribute("x", point.x);
          ele_point->SetAttribute("y", point.y);
          ele_point->SetAttribute("s", point.s);
          ele_point->SetAttribute("heading", point.hdg);
        }
        for (const auto& point : lane->right_boundary.line.points) {
          tinyxml2::XMLElement* ele_point = xml_doc_.NewElement("point");
          ele_right_boundary->InsertEndChild(ele_point);
          ele_point->SetAttribute("x", point.x);
          ele_point->SetAttribute("y", point.y);
          ele_point->SetAttribute("s", point.s);
          ele_point->SetAttribute("heading", point.hdg);
        }
      }
    }
  }
  xml_doc_.SaveFile(file_path.c_str());
  return status;
}

}  // namespace opendrive
