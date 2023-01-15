#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"
#include "opendrive-cpp/parser/converter.h"

using namespace opendrive;

class TestSaveData : public testing::Test {
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
        if (TestSaveData::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestSaveData::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestSaveData::xml_file_path =
    // "/Users/cox/work/code/github/opendrive-files/carla-simulator/Town01.xodr";
    "./tests/data/Ex_Simple-LaneOffset.xodr";

void TestSaveData::SetUpTestCase() {}
void TestSaveData::TearDownTestCase() {}
void TestSaveData::TearDown() {}
void TestSaveData::SetUp() {}

TEST_F(TestSaveData, TestSaveData) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  const tinyxml2::XMLElement* xml_road = xml_root->FirstChildElement("road");
  ASSERT_TRUE(xml_road != nullptr);
  auto ele_map = std::make_shared<opendrive::element::Map>();
  auto core_map = std::make_shared<opendrive::core::Map>();

  /// parse
  auto ret = parser->ParseMap(xml_root, ele_map);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);

  /// adapter
  ret = parser->Convert(ele_map, core_map);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  const std::string file_path = "./build/oxrd.xml";
  // const std::string file_path = "./carla-01.xml";
  parser->SaveData(core_map, file_path);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
