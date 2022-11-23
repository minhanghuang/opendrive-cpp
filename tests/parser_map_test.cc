#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestMapParser : public testing::Test {
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
        if (TestMapParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestMapParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestMapParser::xml_file_path = "./tests/data/only-unittest.xodr";

void TestMapParser::SetUpTestCase() {}
void TestMapParser::TearDownTestCase() {}
void TestMapParser::TearDown() {}
void TestMapParser::SetUp() {}

TEST_F(TestMapParser, TestMap) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  opendrive::element::Map ele_map;
  auto ret = parser->Map(xml, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
}

TEST_F(TestMapParser, TestMapFile) {
  auto parser = GetParser();
  opendrive::element::Map ele_map;
  auto ret = parser->Map(xml_file_path, &ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
