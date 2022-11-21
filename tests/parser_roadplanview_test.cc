#include <gtest/gtest.h>

#include <cassert>
#include <memory>

#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

class TestRoadPlanViewParser : public testing::Test {
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
        if (TestRoadPlanViewParser::xml_file_path.empty()) {
          assert(false);
        }
        instance->LoadFile(TestRoadPlanViewParser::xml_file_path.c_str());
      });
    }
    return instance;
  }

  static std::string xml_file_path;
};

std::string TestRoadPlanViewParser::xml_file_path =
    "./tests/data/only-unittest.xodr";

void TestRoadPlanViewParser::SetUpTestCase() {}
void TestRoadPlanViewParser::TearDownTestCase() {}
void TestRoadPlanViewParser::TearDown() {}
void TestRoadPlanViewParser::SetUp() {}

TEST_F(TestRoadPlanViewParser, TestRoadPlanView) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml_root = GetXml()->RootElement();
  const tinyxml2::XMLElement* xml_road = xml_root->FirstChildElement("road");
  const tinyxml2::XMLElement* xml_planview =
      xml_road->FirstChildElement("planView");
  ASSERT_TRUE(xml_planview != nullptr);
  opendrive::element::RoadPlanView ele_planview;
  auto ret = parser->RoadPlanView(xml_planview, &ele_planview);
  ASSERT_EQ(ele_planview.geometrys.size(), 5);
  ASSERT_EQ(5, ele_planview.geometrys.size());
  auto geometry_info1 = ele_planview.geometrys.at(0);
  auto geometry_info2 = std::dynamic_pointer_cast<element::GeometrySpiral>(
      ele_planview.geometrys.at(1));
  auto geometry_info3 = std::dynamic_pointer_cast<element::GeometryArc>(
      ele_planview.geometrys.at(2));
  auto geometry_info4 = std::dynamic_pointer_cast<element::GeometryPoly3>(
      ele_planview.geometrys.at(3));
  auto geometry_info5 = std::dynamic_pointer_cast<element::GeometryParamPoly3>(
      ele_planview.geometrys.at(4));
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info1->s);
  ASSERT_DOUBLE_EQ(1.5601319999999987e+02, geometry_info1->x);
  ASSERT_DOUBLE_EQ(1.1999995231614086e+02, geometry_info1->y);
  ASSERT_DOUBLE_EQ(3.1415926535848282e+00, geometry_info1->hdg);
  ASSERT_DOUBLE_EQ(5.1604964355176435e+00, geometry_info1->length);
  ASSERT_EQ(GeometryType::LINE, geometry_info1->type);

  ASSERT_DOUBLE_EQ(5.1604964355176435e+00, geometry_info2->s);
  ASSERT_DOUBLE_EQ(1.5085270356448223e+02, geometry_info2->x);
  ASSERT_DOUBLE_EQ(1.1999995231616649e+02, geometry_info2->y);
  ASSERT_DOUBLE_EQ(3.1415926535798597e+00, geometry_info2->hdg);
  ASSERT_DOUBLE_EQ(2.0833333333333330e+00, geometry_info2->length);
  ASSERT_EQ(GeometryType::SPIRAL, geometry_info2->type);
  ASSERT_DOUBLE_EQ(-0.0000000000000000e+00, geometry_info2->curve_start);
  ASSERT_DOUBLE_EQ(-8.3333333333333329e-02, geometry_info2->curve_end);

  ASSERT_DOUBLE_EQ(7.2438297688509765e+00, geometry_info3->s);
  ASSERT_DOUBLE_EQ(1.4877093951784059e+02, geometry_info3->x);
  ASSERT_DOUBLE_EQ(1.2006020151444957e+02, geometry_info3->y);
  ASSERT_DOUBLE_EQ(3.0547870980255456e+00, geometry_info3->hdg);
  ASSERT_DOUBLE_EQ(1.1186223687745517e+01, geometry_info3->length);
  ASSERT_EQ(GeometryType::ARC, geometry_info3->type);
  ASSERT_DOUBLE_EQ(-8.3333333333333329e-02, geometry_info3->curvature);

  ASSERT_DOUBLE_EQ(1.8430053456596493e+01, geometry_info4->s);
  ASSERT_DOUBLE_EQ(4.9416434943455940e+01, geometry_info4->x);
  ASSERT_DOUBLE_EQ(7.9753610549006124e+00, geometry_info4->y);
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
  ASSERT_DOUBLE_EQ(2.0357962351612984e+00, geometry_info5->hdg);
  ASSERT_DOUBLE_EQ(8.7616810347544899e-02, geometry_info5->length);
  ASSERT_EQ(GeometryType::PARAMPOLY3, geometry_info5->type);
  ASSERT_EQ(element::GeometryParamPoly3::PRange::ARCLENGTH,
            geometry_info5->p_range);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->au);
  ASSERT_DOUBLE_EQ(1.0000000000000000e+00, geometry_info5->bu);
  ASSERT_DOUBLE_EQ(4.1336980395811755e-04, geometry_info5->cu);
  ASSERT_DOUBLE_EQ(-2.5462421774261008e-04, geometry_info5->du);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->av);
  ASSERT_DOUBLE_EQ(0.0000000000000000e+00, geometry_info5->bv);
  ASSERT_DOUBLE_EQ(-1.2079506903095295e-02, geometry_info5->cv);
  ASSERT_DOUBLE_EQ(-6.1084906856370778e-04, geometry_info5->dv);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
