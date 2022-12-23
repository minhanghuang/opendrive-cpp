#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestJunctionParser : public testing::Test {
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
        if (TestJunctionParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestJunctionParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestJunctionParser::xml_file_path =
    "./tests/data/UC_Simple-X-Junction.xodr";

void TestJunctionParser::SetUpTestCase() {}
void TestJunctionParser::TearDownTestCase() {}
void TestJunctionParser::TearDown() {}
void TestJunctionParser::SetUp() {}

TEST_F(TestJunctionParser, TestJunction) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  ASSERT_TRUE(xml != nullptr);
  opendrive::element::Map ele_map;
  auto ret = parser->ParseMap(xml, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  auto junction = ele_map.junctions.front();
  ASSERT_EQ(1, junction.attributes.id);
  ASSERT_EQ("", junction.attributes.name);
  ASSERT_EQ(JunctionType::DEFAULT, junction.attributes.type);
  ASSERT_EQ(12, junction.connections.size());
  auto connection_0 = junction.connections.at(0);
  ASSERT_EQ(0, connection_0.id);
  ASSERT_EQ(6, connection_0.incoming_road);
  ASSERT_EQ(2, connection_0.connecting_road);
  ASSERT_EQ(ContactPointType::START, connection_0.contact_point);
  ASSERT_EQ(3, connection_0.lane_links.size());
  auto lane_link_0_0 = connection_0.lane_links.at(0);
  ASSERT_EQ(1, lane_link_0_0.from);
  ASSERT_EQ(-1, lane_link_0_0.to);
  auto lane_link_0_1 = connection_0.lane_links.at(1);
  ASSERT_EQ(2, lane_link_0_1.from);
  ASSERT_EQ(-2, lane_link_0_1.to);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
