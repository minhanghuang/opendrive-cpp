#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestGeometry : public testing::Test {
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
        if (TestGeometry::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestGeometry::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestGeometry::xml_file_path = "./tests/data/only-unittest.xodr";

void TestGeometry::SetUpTestCase() {}
void TestGeometry::TearDownTestCase() {}
void TestGeometry::TearDown() {}
void TestGeometry::SetUp() {}

TEST_F(TestGeometry, TestOffsetPoloy3) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  element::Lanes ele_lanes;
  element::LaneOffset offset_1, offset_2, offset_3, offset_tmp;
  offset_1.s = 25.;
  offset_2.s = 75.;
  offset_3.s = 75.;
  // ele_lanes.lane_offsets.insert(offset_1);
  // ele_lanes.lane_offsets.insert(offset_3);
  // ele_lanes.lane_offsets.insert(offset_2);
  // for (const auto& offset : ele_lanes.lane_offsets) {
    // std::cout << "offset: " << offset.s << std::endl;
  // }
  // offset_tmp.s = 75;
  // auto offset_it = ele_lanes.lane_offsets.lower_bound(offset_tmp);
  // auto offset_it = ele_lanes.lane_offsets.upper_bound(offset_tmp);
  // if (offset_it == ele_lanes.lane_offsets.end()) {
    // std::cout << "未找到" << std::endl;
  // } else {
    // std::cout << "找到:" << offset_it->s << std::endl;
  // }
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
