#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestHeaderParser : public testing::Test {
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
        if (TestHeaderParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestHeaderParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestHeaderParser::xml_file_path = "./tests/data/only-unittest.xodr";

void TestHeaderParser::SetUpTestCase() {}
void TestHeaderParser::TearDownTestCase() {}
void TestHeaderParser::TearDown() {}
void TestHeaderParser::SetUp() {}

TEST_F(TestHeaderParser, TestDemo) {}

TEST_F(TestHeaderParser, TestHeader) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* header_node = xml->FirstChildElement("header");
  ASSERT_TRUE(header_node != nullptr);
  opendrive::element::Header header;
  auto ret = parser->Header(header_node, &header);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  ASSERT_TRUE("1" == header.rev_major);
  ASSERT_TRUE("4" == header.rev_minor);
  ASSERT_TRUE("1" == header.version);
  ASSERT_TRUE("2019-04-06T10:38:28" == header.date);
  ASSERT_TRUE("zhichun Rd" == header.name);
  ASSERT_TRUE("VectorZero" == header.vendor);
  ASSERT_DOUBLE_EQ(2.8349990809409476e+1, header.north);
  ASSERT_DOUBLE_EQ(-3.5690998535156251e+2, header.south);
  ASSERT_DOUBLE_EQ(-2.8359911988457576e+1, header.west);
  ASSERT_DOUBLE_EQ(4.2268105762411665e+2, header.east);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
