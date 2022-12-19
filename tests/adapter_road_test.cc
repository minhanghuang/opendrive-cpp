#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "opendrive-cpp/adapter/road_adapter.h"
#include "opendrive-cpp/common/common.hpp"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestAdapterRoad : public testing::Test {
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
        if (TestAdapterRoad::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestAdapterRoad::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestAdapterRoad::xml_file_path =
    "./tests/data/Ex_Simple-LaneOffset.xodr";

void TestAdapterRoad::SetUpTestCase() {}
void TestAdapterRoad::TearDownTestCase() {}
void TestAdapterRoad::TearDown() {}
void TestAdapterRoad::SetUp() {}

TEST_F(TestAdapterRoad, TestAdapterRoad) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* xml_road = xml->FirstChildElement("road");
  ASSERT_TRUE(xml_road != nullptr);
  opendrive::element::Road ele_road;
  auto road_ptr = std::make_shared<opendrive::core::Road>();

  /// parse
  opendrive::parser::RoadXmlParser road_parser;
  auto ret = road_parser.Parse(xml_road, &ele_road);
  ASSERT_TRUE(ele_road.lanes.lane_sections.size() > 0);
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);

  /// adapter
  opendrive::Adapter adapter;
  ret = adapter.Road(&ele_road, road_ptr);
  std::cout << "adapter ret msg: " << ret.msg << std::endl;
  ASSERT_TRUE(ret.error_code == ErrorCode::OK);
  /// check link
  ASSERT_EQ("11", road_ptr->predecessor);
  ASSERT_EQ("43", road_ptr->successor);
  ASSERT_EQ(RoadLinkType::ROAD, road_ptr->predecessor_type);
  ASSERT_EQ(RoadLinkType::JUNCTION, road_ptr->successor_type);

  /// check sections
  ASSERT_EQ(3, road_ptr->sections.size());
  auto section_0 = road_ptr->sections.at(0);
  ASSERT_EQ(5, section_0->lanes.size());
  ASSERT_EQ("1_0_1", section_0->lanes.front()->id);
  ASSERT_EQ("1", *section_0->lanes.front()->successors.begin());
  ASSERT_EQ("1_0_2", section_0->lanes.at(1)->id);
  ASSERT_EQ("2", *section_0->lanes.at(1)->successors.begin());
  ASSERT_EQ("1_0_3", section_0->lanes.at(2)->id);
  ASSERT_EQ("3", *section_0->lanes.at(2)->successors.begin());
  ASSERT_EQ("1_0_-1", section_0->lanes.at(3)->id);
  ASSERT_EQ("-1", *section_0->lanes.at(3)->successors.begin());
  ASSERT_EQ("1_0_-2", section_0->lanes.at(4)->id);
  ASSERT_EQ("-3", *section_0->lanes.at(4)->successors.begin());
  ASSERT_DOUBLE_EQ(0, section_0->start_position);
  ASSERT_DOUBLE_EQ(2.5000000000000000e+01, section_0->length);
  auto section_1 = road_ptr->sections.at(1);
  ASSERT_EQ(6, section_1->lanes.size());
  ASSERT_EQ("1_1_1", section_1->lanes.front()->id);
  ASSERT_EQ("1_1_2", section_1->lanes.at(1)->id);
  ASSERT_EQ("1_1_3", section_1->lanes.at(2)->id);
  ASSERT_EQ("1_1_-1", section_1->lanes.at(3)->id);
  ASSERT_EQ("1_1_-2", section_1->lanes.at(4)->id);
  ASSERT_EQ("1_1_-3", section_1->lanes.at(5)->id);
  ASSERT_DOUBLE_EQ(2.5000000000000000e+01, section_1->start_position);
  ASSERT_DOUBLE_EQ(5.0000000000000000e+01, section_1->length);
  auto section_2 = road_ptr->sections.at(2);
  ASSERT_EQ(5, section_2->lanes.size());
  ASSERT_EQ("1_2_1", section_2->lanes.front()->id);
  ASSERT_EQ("1_2_2", section_2->lanes.at(1)->id);
  ASSERT_EQ("1_2_-1", section_2->lanes.at(2)->id);
  ASSERT_EQ("1_2_-2", section_2->lanes.at(3)->id);
  ASSERT_EQ("1_2_-3", section_2->lanes.at(4)->id);
  ASSERT_DOUBLE_EQ(7.5000000000000000e+01, section_2->start_position);
  ASSERT_DOUBLE_EQ(2.5000000000000000e+01, section_2->length);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
