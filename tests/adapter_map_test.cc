#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/core.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestAdapterMap : public testing::Test {
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
        if (TestAdapterMap::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestAdapterMap::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestAdapterMap::xml_file_path = "./tests/data/only-unittest.xodr";

void TestAdapterMap::SetUpTestCase() {}
void TestAdapterMap::TearDownTestCase() {}
void TestAdapterMap::TearDown() {}
void TestAdapterMap::SetUp() {}

TEST_F(TestAdapterMap, TestAdapterMap) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  const tinyxml2::XMLElement* xml_road = xml_root->FirstChildElement("road");
  ASSERT_TRUE(xml_road != nullptr);
  opendrive::element::Road ele_road;
  opendrive::element::Map ele_map;
  auto core_map = std::make_shared<opendrive::core::Map>();

  /// parse
  auto ret = parser->Map(xml_root, &ele_map);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  ASSERT_EQ(2, ele_map.roads.size());

  /// adapter
  opendrive::Adapter adapter;
  ret = adapter.Map(&ele_map, core_map);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  ASSERT_EQ(2, core_map->roads.size());
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
