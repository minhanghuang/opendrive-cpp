#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "opendrive-cpp/common/choices.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/common/status.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestCase : public testing::Test {
 public:
  static void SetUpTestCase();     // 在第一个case之前执行
  static void TearDownTestCase();  // 在最后一个case之后执行
  void SetUp() override;           // 在每个case之前执行
  void TearDown() override;        // 在每个case之后执行
};

void TestCase::SetUpTestCase() {}
void TestCase::TearDownTestCase() {}
void TestCase::TearDown() {}
void TestCase::SetUp() {}

TEST_F(TestCase, TestCase1) {
  /// carla Town06.xodr 地图在394_1section解析后显示变形
  const std::string file_path = "./tests/data/case1.xodr";
  opendrive::Parser parser;
  auto ele_map = std::make_shared<opendrive::element::Map>();
  auto core_map = std::make_shared<opendrive::core::Map>();
  auto ret = parser.ParseMap(file_path, ele_map);
  ASSERT_EQ(opendrive::ErrorCode::OK, ret.error_code);
  ret = parser.Adapter(ele_map, core_map);
  ASSERT_EQ(opendrive::ErrorCode::OK, ret.error_code);
  parser.SaveData(core_map, "./case1.xml");
}

// TEST_F(TestCase, TestCase2) {
//   /// carla Town06.xodr 地图在55_0section匝道解析显示变形
//   const std::string file_path = "./tests/data/case2.xodr";
//   opendrive::Parser parser;
//   auto ele_map = std::make_shared<opendrive::element::Map>();
//   auto core_map = std::make_shared<opendrive::core::Map>();
//   auto ret = parser.ParseMap(file_path, ele_map);
//   ASSERT_EQ(opendrive::ErrorCode::OK, ret.error_code);
//   ret = parser.Adapter(ele_map, core_map);
//   ASSERT_EQ(opendrive::ErrorCode::OK, ret.error_code);
//   parser.SaveData(core_map, "./case2.xml");
// }

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
