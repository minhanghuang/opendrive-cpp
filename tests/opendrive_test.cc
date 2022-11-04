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
TEST_F(TestOpenDrive, TestParserHeader) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* header_node = xml->FirstChildElement("header");
  ASSERT_TRUE(header_node != nullptr);
  opendrive::core::Header::Ptr header_ptr =
      std::make_shared<opendrive::core::Header>();
  auto ret = parser->Header(header_node, header_ptr);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);

  std::cout << "rev_major:" << header_ptr->rev_major << std::endl;
  std::cout << "rev_minor:" << header_ptr->rev_minor << std::endl;
  std::cout << "version:" << header_ptr->version << std::endl;
  std::cout << "date:" << header_ptr->date << std::endl;
  std::cout << "name:" << header_ptr->name << std::endl;
  std::cout << "north:" << header_ptr->north << std::endl;
  std::cout << "south:" << header_ptr->south << std::endl;
  std::cout << "west:" << header_ptr->east << std::endl;
  std::cout << "east:" << header_ptr->west << std::endl;
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
