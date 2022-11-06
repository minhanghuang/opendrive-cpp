#include "opendrive-cpp/opendrive.h"

#include <gtest/gtest.h>

#include <cassert>
#include <memory>

class TestOpenDrive : public testing::Test {
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
        if (TestOpenDrive::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestOpenDrive::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestOpenDrive::xml_file_path = "./tests/data/only-unittest.xodr";

void TestOpenDrive::SetUpTestCase() {}
void TestOpenDrive::TearDownTestCase() {}
void TestOpenDrive::TearDown() {}
void TestOpenDrive::SetUp() {}

TEST_F(TestOpenDrive, TestDemo) {}

TEST_F(TestOpenDrive, TestParseHeader) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* header_node = xml->FirstChildElement("header");
  ASSERT_TRUE(header_node != nullptr);
  opendrive::core::Header::Ptr header_ptr =
      std::make_shared<opendrive::core::Header>();
  auto ret = parser->Header(header_node, header_ptr);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  ASSERT_TRUE("1" == header_ptr->rev_major);
  ASSERT_TRUE("4" == header_ptr->rev_minor);
  ASSERT_TRUE("1" == header_ptr->version);
  ASSERT_TRUE("2019-04-06T10:38:28" == header_ptr->date);
  ASSERT_TRUE("zhichun Rd" == header_ptr->name);
  ASSERT_TRUE("VectorZero" == header_ptr->vendor);
  ASSERT_DOUBLE_EQ(2.8349990809409476e+1, header_ptr->north);
  ASSERT_DOUBLE_EQ(-3.5690998535156251e+2, header_ptr->south);
  ASSERT_DOUBLE_EQ(-2.8359911988457576e+1, header_ptr->west);
  ASSERT_DOUBLE_EQ(4.2268105762411665e+2, header_ptr->east);
}

TEST_F(TestOpenDrive, TestParseRoad) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* curr_road_ele = xml->FirstChildElement("road");
  ASSERT_TRUE(curr_road_ele != nullptr);
  auto road_ptr = std::make_shared<opendrive::core::Road>();
  opendrive::parser::RoadXmlParser road_parser;
  road_parser.Parse(curr_road_ele, road_ptr);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
