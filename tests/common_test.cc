#include "opendrive-cpp/common/common.hpp"

#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <memory>
#include <string>

#include "opendrive-cpp/opendrive.h"

class TestCommon : public testing::Test {
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
        if (TestCommon::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestCommon::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestCommon::xml_file_path = "./tests/data/only-unittest.xodr";

void TestCommon::SetUpTestCase() {}
void TestCommon::TearDownTestCase() {}
void TestCommon::TearDown() {}
void TestCommon::SetUp() {}

TEST_F(TestCommon, TestDemo) {}

TEST_F(TestCommon, TestStr) {
  std::string a = "abc";
  std::string b = "ABC";
  ASSERT_EQ("ABC", opendrive::common::StrToUpper(a));
  ASSERT_EQ("ABC", opendrive::common::StrToUpper(b));

  ASSERT_EQ("abc", opendrive::common::StrToLower(a));
  ASSERT_EQ("abc", opendrive::common::StrToLower(b));

  ASSERT_TRUE(opendrive::common::StrEquals("abc", a));
  ASSERT_TRUE(opendrive::common::StrEquals("ABC", b));
}

TEST_F(TestCommon, TestXml) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  auto header_node = xml->FirstChildElement("header");
  ASSERT_TRUE(header_node != nullptr);
  std::string name;
  int revMinor;
  double north;
  bool b = false;
  opendrive::common::XmlQueryStringAttribute(header_node, "name", name);
  opendrive::common::XmlQueryIntAttribute(header_node, "revMinor", revMinor);
  opendrive::common::XmlQueryDoubleAttribute(header_node, "north", north);
  ASSERT_TRUE(4 == revMinor);
  ASSERT_TRUE("zhichun Rd" == name);
  ASSERT_DOUBLE_EQ(2.8349990809409476e+1, north);
  ASSERT_EQ(opendrive::common::XmlQueryIntAttribute(header_node, "revMinor___",
                                                    revMinor),
            tinyxml2::XMLError::XML_NO_ATTRIBUTE);
  ASSERT_EQ(
      opendrive::common::XmlQueryBoolAttribute(header_node, "revMinor___", b),
      tinyxml2::XMLError::XML_NO_ATTRIBUTE);
  ASSERT_EQ(opendrive::common::XmlQueryDoubleAttribute(header_node,
                                                       "revMinor___", north),
            tinyxml2::XMLError::XML_NO_ATTRIBUTE);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
