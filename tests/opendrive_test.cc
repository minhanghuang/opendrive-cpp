#include "opendrive-cpp/opendrive.h"

#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"

using namespace opendrive;

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

TEST_F(TestOpenDrive, TestParseRoad) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  const tinyxml2::XMLElement* curr_road_ele = xml->FirstChildElement("road");
  ASSERT_TRUE(curr_road_ele != nullptr);
  // auto road_ptr = std::make_shared<opendrive::g::Road>();
  opendrive::g::Road road;
  opendrive::parser::RoadXmlParser road_parser;
  road_parser.Parse(curr_road_ele, &road);

  /// road attributes
  ASSERT_EQ("Road 0", road.attributes.name);
  ASSERT_DOUBLE_EQ(3.6360177306314796e+1, road.attributes.length);
  ASSERT_EQ(0, road.attributes.id);
  ASSERT_EQ(-1, road.attributes.junction);
  ASSERT_EQ(opendrive::RoadRule::RHT, road.attributes.rule);

  /// road link
  ASSERT_EQ(g::RoadLinkInfo::Type::ROAD, road.link.predecessor.type);
  ASSERT_EQ(11, road.link.predecessor.id);
  ASSERT_EQ(g::RoadLinkInfo::PointType::START,
            road.link.predecessor.point_type);
  ASSERT_EQ(0, road.link.predecessor.s);
  ASSERT_EQ(g::RoadLinkInfo::Dir::UNKNOWN, road.link.predecessor.dir);

  ASSERT_EQ(g::RoadLinkInfo::Type::JUNCTION, road.link.successor.type);
  ASSERT_EQ(43, road.link.successor.id);
  ASSERT_EQ(g::RoadLinkInfo::PointType::UNKNOWN,
            road.link.successor.point_type);
  ASSERT_EQ(0, road.link.successor.s);
  ASSERT_EQ(g::RoadLinkInfo::Dir::UNKNOWN, road.link.successor.dir);

  /// road type
  ASSERT_EQ(2, road.type_info.size());
  auto type_info1 = road.type_info.front();
  auto type_info2 = road.type_info.back();
  ASSERT_EQ(0, type_info1.s);
  ASSERT_EQ(RoadType::TOWN, type_info1.type);
  ASSERT_EQ("", type_info1.country);
  ASSERT_EQ(25, type_info1.max_speed);
  ASSERT_EQ(RoadSpeedUnit::MPH, type_info1.speed_unit);

  ASSERT_DOUBLE_EQ(2.0601003600277540e+01, type_info2.s);
  ASSERT_EQ(RoadType::TOWN, type_info2.type);
  ASSERT_EQ("DE", type_info2.country);
  ASSERT_EQ(25, type_info2.max_speed);
  ASSERT_EQ(RoadSpeedUnit::KMH, type_info2.speed_unit);

  /// road planView
  ASSERT_EQ(5, road.plan_view.geometrys.size());
  auto geometry_info1 = road.plan_view.geometrys.at(0);
  auto geometry_info2 = std::dynamic_pointer_cast<g::GeometrySpiral>(
      road.plan_view.geometrys.at(1));
  auto geometry_info3 = std::dynamic_pointer_cast<g::GeometryAttributesArc>(
      road.plan_view.geometrys.at(2));
  auto geometry_info4 = std::dynamic_pointer_cast<g::GeometryPoly3>(
      road.plan_view.geometrys.at(3));
  auto geometry_info5 = std::dynamic_pointer_cast<g::GeometryParamPoly3>(
      road.plan_view.geometrys.at(4));
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info1->s);
  ASSERT_DOUBLE_EQ(1.5601319999999987e+02, geometry_info1->x);
  ASSERT_DOUBLE_EQ(1.1999995231614086e+02, geometry_info1->y);
  ASSERT_DOUBLE_EQ(0, geometry_info1->z);
  ASSERT_DOUBLE_EQ(3.1415926535848282e+00, geometry_info1->hdg);
  ASSERT_DOUBLE_EQ(5.1604964355176435e+00, geometry_info1->length);
  ASSERT_EQ(GeometryType::LINE, geometry_info1->type);

  ASSERT_DOUBLE_EQ(5.1604964355176435e+00, geometry_info2->s);
  ASSERT_DOUBLE_EQ(1.5085270356448223e+02, geometry_info2->x);
  ASSERT_DOUBLE_EQ(1.1999995231616649e+02, geometry_info2->y);
  ASSERT_DOUBLE_EQ(0, geometry_info2->z);
  ASSERT_DOUBLE_EQ(3.1415926535798597e+00, geometry_info2->hdg);
  ASSERT_DOUBLE_EQ(2.0833333333333330e+00, geometry_info2->length);
  ASSERT_EQ(GeometryType::SPIRAL, geometry_info2->type);
  ASSERT_DOUBLE_EQ(-0.0000000000000000e+00, geometry_info2->curve_start);
  ASSERT_DOUBLE_EQ(-8.3333333333333329e-02, geometry_info2->curve_end);

  ASSERT_DOUBLE_EQ(7.2438297688509765e+00, geometry_info3->s);
  ASSERT_DOUBLE_EQ(1.4877093951784059e+02, geometry_info3->x);
  ASSERT_DOUBLE_EQ(1.2006020151444957e+02, geometry_info3->y);
  ASSERT_DOUBLE_EQ(0, geometry_info3->z);
  ASSERT_DOUBLE_EQ(3.0547870980255456e+00, geometry_info3->hdg);
  ASSERT_DOUBLE_EQ(1.1186223687745517e+01, geometry_info3->length);
  ASSERT_EQ(GeometryType::ARC, geometry_info3->type);
  ASSERT_DOUBLE_EQ(-8.3333333333333329e-02, geometry_info3->curvature);

  ASSERT_DOUBLE_EQ(1.8430053456596493e+01, geometry_info4->s);
  ASSERT_DOUBLE_EQ(4.9416434943455940e+01, geometry_info4->x);
  ASSERT_DOUBLE_EQ(7.9753610549006124e+00, geometry_info4->y);
  ASSERT_DOUBLE_EQ(0, geometry_info4->z);
  ASSERT_DOUBLE_EQ(-3.8812339311141031e-02, geometry_info4->hdg);
  ASSERT_DOUBLE_EQ(2.0833333333333330e+00, geometry_info4->length);
  ASSERT_EQ(GeometryType::POLY3, geometry_info4->type);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info4->a);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info4->b);
  ASSERT_DOUBLE_EQ(7.8270095403552599e-03, geometry_info4->c);
  ASSERT_DOUBLE_EQ(-1.2981379520556725e-04, geometry_info4->d);

  ASSERT_DOUBLE_EQ(2.0513386789929825e+01, geometry_info5->s);
  ASSERT_DOUBLE_EQ(1.3860498286986785e+02, geometry_info5->x);
  ASSERT_DOUBLE_EQ(1.2755802030419328e+02, geometry_info5->y);
  ASSERT_DOUBLE_EQ(0, geometry_info5->z);
  ASSERT_DOUBLE_EQ(2.0357962351612984e+00, geometry_info5->hdg);
  ASSERT_DOUBLE_EQ(8.7616810347544899e-02, geometry_info5->length);
  ASSERT_EQ(GeometryType::PARAMPOLY3, geometry_info5->type);
  ASSERT_EQ(g::GeometryParamPoly3::PRange::ARCLENGTH, geometry_info5->p_range);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->aU);
  ASSERT_DOUBLE_EQ(1.0000000000000000e+00, geometry_info5->bU);
  ASSERT_DOUBLE_EQ(4.1336980395811755e-04, geometry_info5->cU);
  ASSERT_DOUBLE_EQ(-2.5462421774261008e-04, geometry_info5->dU);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->aV);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->bV);
  ASSERT_DOUBLE_EQ(-1.2079506903095295e-02, geometry_info5->cV);
  ASSERT_DOUBLE_EQ(-6.1084906856370778e-04, geometry_info5->dV);

  /// road lanes offset
  ASSERT_EQ(2, road.lanes.lane_offsets.size());
  auto lanes_offset1 = road.lanes.lane_offsets.at(0);
  auto lanes_offset2 = road.lanes.lane_offsets.at(1);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset1.s);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset1.a);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset1.b);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset1.c);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset1.d);
  ASSERT_DOUBLE_EQ(7.5000000000000000e+01, lanes_offset2.s);
  ASSERT_DOUBLE_EQ(3.2500000000000000e+00, lanes_offset2.a);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset2.b);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset2.c);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, lanes_offset2.d);

  /// road lanes sections
  ASSERT_EQ(1, road.lanes.lane_sections.size());
  auto lane_section1 = road.lanes.lane_sections.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+00, lane_section1.s);
  ASSERT_TRUE(lane_section1.left.lanes.size() == 3);
  ASSERT_TRUE(lane_section1.center.lanes.size() == 1);
  ASSERT_TRUE(lane_section1.right.lanes.size() == 3);
  /// road lanes sections left
  auto lane_section1_left1 = lane_section1.left.lanes.at(0);
  auto lane_section1_left2 = lane_section1.left.lanes.at(1);
  auto lane_section1_left3 = lane_section1.left.lanes.at(2);
  ASSERT_EQ(3, lane_section1_left1.attributes.id);
  ASSERT_EQ(2, lane_section1_left2.attributes.id);
  ASSERT_EQ(1, lane_section1_left3.attributes.id);
  ASSERT_EQ(LaneType::SIDEWALK, lane_section1_left1.attributes.type);
  ASSERT_EQ(Boolean::FALSE, lane_section1_left1.attributes.level);
  ASSERT_EQ(-3, lane_section1_left1.link.predecessor);
  ASSERT_EQ(1, lane_section1_left1.widths.size());
  auto lane_section1_width1 = lane_section1_left1.widths.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_width1.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width1.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_width1.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_width1.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width1.d);

  ASSERT_EQ(1, lane_section1_left1.borders.size());
  auto lane_section1_border1 = lane_section1_left1.borders.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_border1.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border1.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_border1.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_border1.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_border1.d);

  ASSERT_EQ(1, lane_section1_left1.road_marks.size());
  auto lane_section1_roadmarks1 = lane_section1_left1.road_marks.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_roadmarks1.s);
  ASSERT_EQ(RoadMarkType::NONE, lane_section1_roadmarks1.type);
  ASSERT_EQ(RoadMarkColor::WHITE, lane_section1_roadmarks1.color);
  ASSERT_EQ(RoadMarkWeight::UNKNOWN, lane_section1_roadmarks1.weigth);
  ASSERT_EQ(RoadMarkLaneChange::NONE, lane_section1_roadmarks1.lane_change);
  ASSERT_EQ("standard", lane_section1_roadmarks1.material);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_roadmarks1.width);
  ASSERT_DOUBLE_EQ(4.0000000000000000e+0, lane_section1_roadmarks1.height);

  /// road lanes sections center
  auto lane_section1_center = lane_section1.center.lanes.at(0);
  ASSERT_EQ(0, lane_section1_center.attributes.id);
  ASSERT_EQ(LaneType::NONE, lane_section1_center.attributes.type);
  ASSERT_EQ(Boolean::FALSE, lane_section1_center.attributes.level);

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

  /// road lanes sections right
  auto lane_section1_right1 = lane_section1.right.lanes.at(0);
  auto lane_section1_right2 = lane_section1.right.lanes.at(1);
  auto lane_section1_right3 = lane_section1.right.lanes.at(2);
  ASSERT_EQ(-1, lane_section1_right1.attributes.id);
  ASSERT_EQ(-2, lane_section1_right2.attributes.id);
  ASSERT_EQ(-3, lane_section1_right3.attributes.id);
  ASSERT_EQ(LaneType::DRIVING, lane_section1_right1.attributes.type);
  ASSERT_EQ(Boolean::FALSE, lane_section1_right1.attributes.level);
  ASSERT_EQ(1, lane_section1_right1.link.predecessor);
  ASSERT_EQ(1, lane_section1_right1.widths.size());
  auto lane_section1_width21 = lane_section1_right1.widths.front();
  ASSERT_DOUBLE_EQ(1.0000000000000000e+0, lane_section1_width21.s);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width21.a);
  ASSERT_DOUBLE_EQ(2.0000000000000000e+0, lane_section1_width21.b);
  ASSERT_DOUBLE_EQ(3.0000000000000000e+0, lane_section1_width21.c);
  ASSERT_DOUBLE_EQ(4.0000000000000009e+0, lane_section1_width21.d);

  ASSERT_EQ(1, lane_section1_left1.borders.size());
  auto lane_section1_border21 = lane_section1_right1.borders.front();
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
