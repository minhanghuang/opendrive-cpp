#include "opendrive-cpp/common/common.hpp"

#include <gtest/gtest.h>
#include <tinyxml2.h>

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "opendrive-cpp/common/choices.h"
#include "opendrive-cpp/geometry/element.h"
#include "opendrive-cpp/geometry/enums.h"
#include "opendrive-cpp/opendrive.h"

using namespace opendrive;

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

TEST_F(TestCommon, TestFormatChoices) {
  opendrive::LaneType type = opendrive::LaneType::DRIVING;
  ASSERT_EQ("driving",
            common::FormatChoices(opendrive::LANE_TYPE_CHOICES, type));
}

TEST_F(TestCommon, TestVectorSort) {
  std::vector<element::LaneOffset> lane_offsets;
  element::LaneOffset l1, l2, l3, l4, l5;
  int ret = common::GetGEValuePoloy3(lane_offsets, 11);
  ASSERT_EQ(-1, ret);
  l1.s = 11;
  l2.s = 13;
  l3.s = 15;
  l4.s = 16;
  l5.s = 19;
  lane_offsets.emplace_back(l1);
  lane_offsets.emplace_back(l2);
  lane_offsets.emplace_back(l5);
  lane_offsets.emplace_back(l3);
  lane_offsets.emplace_back(l4);
  common::VectorSortPoloy3(lane_offsets);
  std::vector<double> e_v{11, 13, 15, 16, 19};
  for (int i = 0; i < lane_offsets.size(); i++) {
    ASSERT_DOUBLE_EQ(e_v.at(i), lane_offsets.at(i).s);
  }
}

TEST_F(TestCommon, TestLeftValue) {
  std::vector<element::LaneOffset> lane_offsets;
  element::LaneOffset l1, l2, l3, l4, l5;
  int ret = common::GetGEValuePoloy3(lane_offsets, 11);
  ASSERT_EQ(-1, ret);
  l1.s = 11;
  l2.s = 13;
  l3.s = 15;
  l4.s = 16;
  l5.s = 19;
  lane_offsets.emplace_back(l1);
  lane_offsets.emplace_back(l2);
  lane_offsets.emplace_back(l3);
  lane_offsets.emplace_back(l4);
  lane_offsets.emplace_back(l5);
  ret = common::GetGEValuePoloy3(lane_offsets, 0);
  ASSERT_EQ(-1, ret);
  ret = common::GetGEValuePoloy3(lane_offsets, 11);
  ASSERT_EQ(0, ret);
  ret = common::GetGEValuePoloy3(lane_offsets, 15);
  ASSERT_EQ(2, ret);
  ret = common::GetGEValuePoloy3(lane_offsets, 19);
  ASSERT_EQ(4, ret);
  ret = common::GetGEValuePoloy3(lane_offsets, 100);
  ASSERT_EQ(4, ret);
  ret = common::GetGTValuePoloy3(lane_offsets, 15);
  ASSERT_EQ(1, ret);
  ret = common::GetGTValuePoloy3(lane_offsets, 19);
  ASSERT_EQ(3, ret);
  ret = common::GetGTValuePoloy3(lane_offsets, 20);
  ASSERT_EQ(4, ret);
  ret = common::GetGTValuePoloy3(lane_offsets, 11);
  ASSERT_EQ(0, ret);
  ret = common::GetGTValuePoloy3(lane_offsets, 10);
  ASSERT_EQ(-1, ret);
}

TEST_F(TestCommon, TestLeftPtr) {
  std::vector<element::Geometry::Ptr> geometrys;
  auto g1 = std::make_shared<element::GeometryLine>(11, 2, 3, 4, 5,
                                                    GeometryType::LINE);
  auto g2 = std::make_shared<element::GeometryLine>(13, 2, 3, 4, 5,
                                                    GeometryType::LINE);
  auto g3 = std::make_shared<element::GeometryLine>(15, 2, 3, 4, 5,
                                                    GeometryType::LINE);
  auto g4 = std::make_shared<element::GeometryLine>(18, 2, 3, 4, 5,
                                                    GeometryType::LINE);
  auto g5 = std::make_shared<element::GeometryLine>(19, 2, 3, 4, 5,
                                                    GeometryType::LINE);
  geometrys.emplace_back(g1);
  geometrys.emplace_back(g2);
  geometrys.emplace_back(g3);
  geometrys.emplace_back(g4);
  geometrys.emplace_back(g5);
  auto ret = common::GetGEPtrPoloy3(geometrys, 0);
  ASSERT_EQ(-1, ret);
  ret = common::GetGEPtrPoloy3(geometrys, 11);
  ASSERT_EQ(0, ret);
  ret = common::GetGEPtrPoloy3(geometrys, 19);
  ASSERT_EQ(4, ret);
  ret = common::GetGEPtrPoloy3(geometrys, 100);
  ASSERT_EQ(4, ret);
}

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
  auto header_ptr = std::make_shared<opendrive::element::Header>();
  ASSERT_TRUE(header_node != nullptr);
  opendrive::common::XmlQueryStringAttribute(header_node, "name",
                                             header_ptr->name);
  opendrive::common::XmlQueryStringAttribute(header_node, "revMinor",
                                             header_ptr->rev_minor);
  opendrive::common::XmlQueryDoubleAttribute(header_node, "north",
                                             header_ptr->north);
  ASSERT_TRUE("4" == header_ptr->rev_minor);
  ASSERT_TRUE("zhichun Rd" == header_ptr->name);
  ASSERT_DOUBLE_EQ(2.8349990809409476e+1, header_ptr->north);

  /// not found
  ASSERT_EQ(opendrive::common::XmlQueryStringAttribute(header_node, "aaa",
                                                       header_ptr->rev_minor),
            tinyxml2::XMLError::XML_NO_ATTRIBUTE);

  /// default
  header_ptr->rev_minor = "99";
  header_ptr->name = "qwert";
  header_ptr->north = -99.99;
  opendrive::common::XmlQueryStringAttribute(header_node, "name__",
                                             header_ptr->name);
  opendrive::common::XmlQueryStringAttribute(header_node, "revMinor___",
                                             header_ptr->rev_minor);
  opendrive::common::XmlQueryDoubleAttribute(header_node, "north___",
                                             header_ptr->north);
  ASSERT_TRUE("99" == header_ptr->rev_minor);
  ASSERT_TRUE("qwert" == header_ptr->name);
  ASSERT_DOUBLE_EQ(-99.99, header_ptr->north);
}

TEST_F(TestCommon, TestXml2) {
  auto parser = GetParser();
  const tinyxml2::XMLElement* xml = GetXml()->RootElement();
  auto road_ele = xml->FirstChildElement("road");
  auto road_ptr = std::make_shared<opendrive::element::Road>();
  ASSERT_TRUE(road_ele != nullptr);
  ASSERT_TRUE(road_ptr->attributes.rule == RoadRule::RHT);
  auto ret = opendrive::common::XmlQueryEnumAttribute(
      road_ele, "rule", road_ptr->attributes.rule,
      opendrive::ROAD_RULE_CHOICES);
  ASSERT_TRUE(road_ptr->attributes.rule == RoadRule::RHT);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
