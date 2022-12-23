#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestRoadLinkParser : public testing::Test {
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
        if (TestRoadLinkParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestRoadLinkParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestRoadLinkParser::xml_file_path =
    "./tests/data/only-unittest.xodr";

void TestRoadLinkParser::SetUpTestCase() {}
void TestRoadLinkParser::TearDownTestCase() {}
void TestRoadLinkParser::TearDown() {}
void TestRoadLinkParser::SetUp() {}

TEST_F(TestRoadLinkParser, TestRoadLink) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  ASSERT_TRUE(xml_root != nullptr);
  opendrive::element::Map ele_map;
  auto ret = parser->ParseMap(xml_root, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  auto ele_roadlink = ele_map.roads.front().link;
  ASSERT_TRUE(11 == ele_roadlink.predecessor.id);
  ASSERT_DOUBLE_EQ(0, ele_roadlink.predecessor.s);
  ASSERT_TRUE(RoadLinkType::ROAD == ele_roadlink.predecessor.type);
  ASSERT_TRUE(ContactPointType::START ==
              ele_roadlink.predecessor.contact_point);
  ASSERT_TRUE(Dir::PLUS == ele_roadlink.predecessor.dir);
  ASSERT_TRUE(43 == ele_roadlink.successor.id);
  ASSERT_DOUBLE_EQ(1.0000000000000000e+00, ele_roadlink.successor.s);
  ASSERT_TRUE(RoadLinkType::JUNCTION == ele_roadlink.successor.type);
  ASSERT_TRUE(ContactPointType::UNKNOWN ==
              ele_roadlink.successor.contact_point);
  ASSERT_TRUE(Dir::UNKNOWN == ele_roadlink.successor.dir);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
