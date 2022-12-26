#include "opendrive-cpp/parser/adapter.h"

#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestAdapterMap : public testing::Test {
 public:
  static void SetUpTestCase();     // 在第一个case之前执行
  static void TearDownTestCase();  // 在最后一个case之后执行
  void SetUp() override;           // 在每个case之前执行
  void TearDown() override;        // 在每个case之后执行

  static std::shared_ptr<opendrive::Parser> GetParser() {
    static std::shared_ptr<opendrive::Parser> parser =
        std::make_shared<opendrive::Parser>();
    return parser;
  }

  static const tinyxml2::XMLDocument* GetXml() {
    static tinyxml2::XMLDocument* instance = nullptr;
    if (!instance) {
      static std::once_flag flag;
      std::call_once(flag, [&] {
        instance = new (std::nothrow) tinyxml2::XMLDocument();
        if (TestAdapterMap::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestAdapterMap::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestAdapterMap::xml_file_path = "./tests/data/only-unittest.xodr";

void TestAdapterMap::SetUpTestCase() {}
void TestAdapterMap::TearDownTestCase() {}
void TestAdapterMap::TearDown() {}
void TestAdapterMap::SetUp() {}

TEST_F(TestAdapterMap, TestAdapterMap) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  const tinyxml2::XMLElement* xml_road = xml_root->FirstChildElement("road");
  ASSERT_TRUE(xml_road != nullptr);
  opendrive::element::Road ele_road;
  auto ele_map = std::make_shared<opendrive::element::Map>();
  auto core_map = std::make_shared<opendrive::core::Map>();

  /// parse
  auto ret = parser->ParseMap(xml_root, ele_map);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  ASSERT_EQ(2, ele_map->roads.size());

  /// adapter
  ret = parser->Adapter(ele_map, core_map);
  std::cout << "ret msg: " << ret.msg << std::endl;
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);

  /// check header
  ASSERT_EQ("1", core_map->header->rev_major);
  ASSERT_EQ("4", core_map->header->rev_minor);
  ASSERT_EQ("zhichun Rd", core_map->header->name);
  ASSERT_EQ("1", core_map->header->version);
  ASSERT_EQ("2019-04-06T10:38:28", core_map->header->date);
  ASSERT_EQ(2.8349990809409476e+1, core_map->header->north);
  ASSERT_EQ(-3.5690998535156251e+2, core_map->header->south);
  ASSERT_EQ(4.2268105762411665e+2, core_map->header->east);
  ASSERT_EQ(-2.8359911988457576e+1, core_map->header->west);
  ASSERT_EQ("VectorZero", core_map->header->vendor);

  /// check junction
  ASSERT_EQ(12, core_map->junctions.size());
  auto junction_1 = core_map->junctions["26"];
  ASSERT_EQ("junction26", junction_1->name);
  ASSERT_EQ(6, junction_1->connections.size());
  ASSERT_TRUE(0 != junction_1->connections.count("1_27"));
  auto connection_1_1 = junction_1->connections.at("1_27");
  ASSERT_EQ("0", connection_1_1.id);
  ASSERT_EQ("1", connection_1_1.incoming_road);
  ASSERT_EQ("27", connection_1_1.connecting_road);
  ASSERT_EQ(ContactPointType::END, connection_1_1.contact_point);
  ASSERT_EQ(1, connection_1_1.lane_links.size());
  auto link_1_1_1 = connection_1_1.lane_links.at(0);
  ASSERT_EQ("-1", link_1_1_1.first);
  ASSERT_EQ("1", link_1_1_1.second);

  /// check roads
  ASSERT_EQ(2, core_map->roads.size());

  const std::string file_path = "./oxrd.xml";
  parser->SaveData(core_map, file_path);
}

TEST_F(TestAdapterMap, TestLaneLink) {
  const std::string file_path = "./tests/data/UC_Simple-X-Junction.xodr";
  std::shared_ptr<opendrive::Parser> parser =
      std::make_shared<opendrive::Parser>();
  tinyxml2::XMLDocument* instance = nullptr;
  instance = new (std::nothrow) tinyxml2::XMLDocument();
  instance->LoadFile(file_path.c_str());
  const tinyxml2::XMLElement* xml_root = instance->RootElement();
  auto ele_map = std::make_shared<opendrive::element::Map>();
  auto core_map = std::make_shared<opendrive::core::Map>();

  /// parse
  auto ret = parser->ParseMap(xml_root, ele_map);
  std::cout << "parse ret msg: " << ret.msg << std::endl;
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  ASSERT_EQ(16, ele_map->roads.size());

  /// adapter
  ret = parser->Adapter(ele_map, core_map);
  std::cout << "adapte ret msg: " << ret.msg << std::endl;
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);

  /// check
  for (const auto& road : core_map->roads) {
    for (const auto& section : road.second->sections) {
      for (const auto& lane : section->left_lanes) {
        ASSERT_TRUE(lane->id > "0");
      }
      ASSERT_TRUE(section->center_lane->id == "0");
      for (const auto& lane : section->right_lanes) {
        ASSERT_TRUE(lane->id < "0");
      }
    }
  }

  auto road_0 = core_map->roads.at("0");
  ASSERT_TRUE(nullptr != road_0);
  ASSERT_EQ("", road_0->successor);
  ASSERT_EQ("1", road_0->predecessor);
  auto lane_0_0_1 = road_0->sections.front()->left_lanes.at(0);
  auto lane_0_0_0 = road_0->sections.front()->center_lane;
  ASSERT_TRUE(nullptr != lane_0_0_1);
  ASSERT_TRUE(nullptr != lane_0_0_0);
  ASSERT_EQ(opendrive::LaneType::DRIVING, lane_0_0_0->type);
  std::set<core::Id> ex_lane_0_0_1_sucs = {""};
  for (const auto& suc_id : lane_0_0_1->successors) {
  }
  delete instance;
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
