#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestRoadTypeParser : public testing::Test {
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
        if (TestRoadTypeParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestRoadTypeParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestRoadTypeParser::xml_file_path =
    "./tests/data/only-unittest.xodr";

void TestRoadTypeParser::SetUpTestCase() {}
void TestRoadTypeParser::TearDownTestCase() {}
void TestRoadTypeParser::TearDown() {}
void TestRoadTypeParser::SetUp() {}

TEST_F(TestRoadTypeParser, TestRoadType) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  ASSERT_TRUE(xml_root != nullptr);
  opendrive::element::Map ele_map;
  auto ret = parser->ParseMap(xml_root, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  auto ele_roadtype = ele_map.roads.front().type_info.front();
  ASSERT_DOUBLE_EQ(0.0000000000000000e+0, ele_roadtype.s);
  ASSERT_TRUE(RoadType::TOWN == ele_roadtype.type);
  ASSERT_TRUE(ele_roadtype.country.empty());
  ASSERT_DOUBLE_EQ(25, ele_roadtype.max_speed);
  ASSERT_TRUE(RoadSpeedUnit::MPH == ele_roadtype.speed_unit);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
