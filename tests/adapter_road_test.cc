#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/types.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestAdapterRoad : public testing::Test {
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
        if (TestAdapterRoad::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestAdapterRoad::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestAdapterRoad::xml_file_path = "./tests/data/only-unittest.xodr";

void TestAdapterRoad::SetUpTestCase() {}
void TestAdapterRoad::TearDownTestCase() {}
void TestAdapterRoad::TearDown() {}
void TestAdapterRoad::SetUp() {}

TEST_F(TestAdapterRoad, TestAdapterRoad) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* curr_road_ele = xml->FirstChildElement("road");
  ASSERT_TRUE(curr_road_ele != nullptr);
  opendrive::g::Road g_road;
  auto core_road_ptr = std::make_shared<opendrive::core::Road>();

  opendrive::parser::RoadXmlParser road_parser;
  road_parser.Parse(curr_road_ele, &g_road);
  ASSERT_TRUE(g_road.lanes.lane_sections.size() > 0);

  adapter::RoadAdapter road_adapter;
  road_adapter.Run(&g_road, core_road_ptr);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
