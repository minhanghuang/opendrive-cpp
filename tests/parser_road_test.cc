#include <gtest/gtest.h>

#include <cassert>
#include <memory>
#include <set>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestRoadParser : public testing::Test {
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
        if (TestRoadParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestRoadParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestRoadParser::xml_file_path = "./tests/data/only-unittest.xodr";

void TestRoadParser::SetUpTestCase() {}
void TestRoadParser::TearDownTestCase() {}
void TestRoadParser::TearDown() {}
void TestRoadParser::SetUp() {}

TEST_F(TestRoadParser, TestRoad) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  ASSERT_TRUE(xml != nullptr);
  opendrive::element::Map ele_map;
  auto ret = parser->ParseMap(xml, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  auto ele_road = ele_map.roads.front();

  /// attributes
  ASSERT_TRUE("Road 0" == ele_road.attributes.name);
  ASSERT_DOUBLE_EQ(3.6360177306314796e+1, ele_road.attributes.length);
  ASSERT_TRUE(0 == ele_road.attributes.id);
  ASSERT_TRUE(-1 == ele_road.attributes.junction);
  ASSERT_TRUE(RoadRule::RHT == ele_road.attributes.rule);

  /// road type info
  ASSERT_TRUE(2 == ele_road.type_info.size());
  auto type_info_1 = ele_road.type_info.at(0);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+0, type_info_1.s);
  ASSERT_DOUBLE_EQ(25, type_info_1.max_speed);
  ASSERT_TRUE(RoadType::TOWN == type_info_1.type);
  ASSERT_TRUE("" == type_info_1.country);
  ASSERT_TRUE(RoadSpeedUnit::MPH == type_info_1.speed_unit);
  auto type_info_2 = ele_road.type_info.at(1);
  ASSERT_DOUBLE_EQ(2.0601003600277540e+01, type_info_2.s);
  ASSERT_DOUBLE_EQ(25, type_info_2.max_speed);
  ASSERT_TRUE(RoadType::TOWN == type_info_2.type);
  ASSERT_TRUE("DE" == type_info_2.country);
  ASSERT_TRUE(RoadSpeedUnit::KMH == type_info_2.speed_unit);

  ASSERT_TRUE(5 == ele_road.lanes.lane_sections.size());
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
