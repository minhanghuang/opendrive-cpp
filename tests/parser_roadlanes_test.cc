#include <gtest/gtest.h>

#include <cassert>
#include <memory>
#include <set>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestRoadLanesParser : public testing::Test {
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
        if (TestRoadLanesParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestRoadLanesParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestRoadLanesParser::xml_file_path =
    "./tests/data/only-unittest.xodr";

void TestRoadLanesParser::SetUpTestCase() {}
void TestRoadLanesParser::TearDownTestCase() {}
void TestRoadLanesParser::TearDown() {}
void TestRoadLanesParser::SetUp() {}

TEST_F(TestRoadLanesParser, TestRoadLanes) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  ASSERT_TRUE(xml_root != nullptr);
  auto ele_map = std::make_shared<opendrive::element::Map>();
  auto ret = parser->ParseMap(xml_root, ele_map);
  ASSERT_TRUE(opendrive::ErrorCode::OK == ret.error_code);
  auto ele_lanes = ele_map->roads.front().lanes;
  /// offset
  ASSERT_TRUE(2 == ele_lanes.lane_offsets.size());
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(0).s);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(0).a);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(0).b);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(0).c);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(0).d);
  ASSERT_DOUBLE_EQ(7.5000000000000000e+01, ele_lanes.lane_offsets.at(1).s);
  ASSERT_DOUBLE_EQ(3.2500000000000000e+00, ele_lanes.lane_offsets.at(1).a);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(1).b);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(1).c);
  ASSERT_DOUBLE_EQ(0, ele_lanes.lane_offsets.at(1).d);

  /// section
  ASSERT_DOUBLE_EQ(5.1604964355176435e+00, ele_lanes.lane_sections.at(1).s0);
  ASSERT_DOUBLE_EQ(7.2438297688509765e+00, ele_lanes.lane_sections.at(2).s0);
  ASSERT_DOUBLE_EQ(1.8430053456596493e+01, ele_lanes.lane_sections.at(3).s0);
  ASSERT_DOUBLE_EQ(2.0513386789929825e+01, ele_lanes.lane_sections.at(4).s0);

  ASSERT_TRUE(5 == ele_lanes.lane_sections.size());
  auto lane_section1 = ele_lanes.lane_sections.front();
  ASSERT_DOUBLE_EQ(0, lane_section1.s0);
  ASSERT_TRUE(lane_section1.left.lanes.size() == 3);
  ASSERT_TRUE(lane_section1.center.lanes.size() == 1);
  ASSERT_TRUE(lane_section1.right.lanes.size() == 3);

  /// ele_lanes sections left
  auto lane_section1_l3 = lane_section1.left.lanes.at(2);
  auto lane_section1_l2 = lane_section1.left.lanes.at(1);
  auto lane_section1_l1 = lane_section1.left.lanes.at(0);
  ASSERT_EQ(3, lane_section1_l3.attributes.id());
  ASSERT_EQ(2, lane_section1_l2.attributes.id());
  ASSERT_EQ(1, lane_section1_l1.attributes.id());
  ASSERT_EQ(LaneType::SIDEWALK, lane_section1_l3.attributes.type());
  ASSERT_EQ(Boolean::FALSE, lane_section1_l3.attributes.level());
  ASSERT_EQ(-3, lane_section1_l3.link.predecessors.at(0));
  ASSERT_EQ(1, lane_section1_l3.widths.size());
  auto lane_section1_width1 = *lane_section1_l3.widths.begin();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_width1.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width1.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_width1.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_width1.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width1.d);

  ASSERT_EQ(1, lane_section1_l3.borders.size());
  auto lane_section1_border1 = *lane_section1_l3.borders.begin();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_border1.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border1.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_border1.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_border1.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border1.d);

  ASSERT_EQ(1, lane_section1_l3.road_marks.size());
  auto lane_section1_roadmarks1 = lane_section1_l3.road_marks.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_roadmarks1.s);
  ASSERT_EQ(RoadMarkType::NONE, lane_section1_roadmarks1.type);
  ASSERT_EQ(RoadMarkColor::WHITE, lane_section1_roadmarks1.color);
  ASSERT_EQ(RoadMarkWeight::UNKNOWN, lane_section1_roadmarks1.weigth);
  ASSERT_EQ(RoadMarkLaneChange::NONE, lane_section1_roadmarks1.lane_change);
  ASSERT_EQ("standard", lane_section1_roadmarks1.material);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_roadmarks1.width);
  ASSERT_DOUBLE_EQ(4.0000000000000000e+0, lane_section1_roadmarks1.height);

  /// ele_lanes sections center
  auto lane_section1_center = lane_section1.center.lanes.at(0);
  ASSERT_EQ(0, lane_section1_center.attributes.id());
  ASSERT_EQ(LaneType::NONE, lane_section1_center.attributes.type());
  ASSERT_EQ(Boolean::FALSE, lane_section1_center.attributes.level());

  ASSERT_EQ(1, lane_section1_center.road_marks.size());
  auto lane_section1_center_roadmarks1 =
      lane_section1_center.road_marks.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_center_roadmarks1.s);
  ASSERT_EQ(RoadMarkType::BROKEN, lane_section1_center_roadmarks1.type);
  ASSERT_EQ(RoadMarkColor::YELLOW, lane_section1_center_roadmarks1.color);
  ASSERT_EQ(RoadMarkWeight::UNKNOWN, lane_section1_center_roadmarks1.weigth);
  ASSERT_EQ(RoadMarkLaneChange::NONE,
            lane_section1_center_roadmarks1.lane_change);
  ASSERT_EQ("standard", lane_section1_center_roadmarks1.material);
  ASSERT_DOUBLE_EQ(1.2500000000000000e-1,
                   lane_section1_center_roadmarks1.width);
  ASSERT_DOUBLE_EQ(0., lane_section1_center_roadmarks1.height);

  /// ele_lanes sections right
  auto lane_section1_right1 = lane_section1.right.lanes.at(0);
  auto lane_section1_right2 = lane_section1.right.lanes.at(1);
  auto lane_section1_right3 = lane_section1.right.lanes.at(2);
  ASSERT_EQ(-1, lane_section1_right1.attributes.id());
  ASSERT_EQ(-2, lane_section1_right2.attributes.id());
  ASSERT_EQ(-3, lane_section1_right3.attributes.id());
  ASSERT_EQ(LaneType::DRIVING, lane_section1_right1.attributes.type());
  ASSERT_EQ(Boolean::FALSE, lane_section1_right1.attributes.level());
  ASSERT_EQ(1, lane_section1_right1.link.predecessors.at(0));
  ASSERT_EQ(1, lane_section1_right1.widths.size());
  auto lane_section1_width21 = *lane_section1_right1.widths.begin();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_width21.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width21.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_width21.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_width21.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width21.d);

  ASSERT_EQ(1, lane_section1_l3.borders.size());
  auto lane_section1_border21 = *lane_section1_right1.borders.begin();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_border21.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border21.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_border21.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_border21.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border21.d);

  ASSERT_EQ(1, lane_section1_right1.road_marks.size());
  auto lane_section1_roadmarks21 = lane_section1_right1.road_marks.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_roadmarks21.s);
  ASSERT_EQ(RoadMarkType::NONE, lane_section1_roadmarks21.type);
  ASSERT_EQ(RoadMarkColor::WHITE, lane_section1_roadmarks21.color);
  ASSERT_EQ(RoadMarkWeight::UNKNOWN, lane_section1_roadmarks21.weigth);
  ASSERT_EQ(RoadMarkLaneChange::NONE, lane_section1_roadmarks21.lane_change);
  ASSERT_EQ("standard", lane_section1_roadmarks21.material);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_roadmarks21.width);
  ASSERT_DOUBLE_EQ(4.0000000000000000e+0, lane_section1_roadmarks21.height);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
