#include <gtest/gtest.h>

class TestOpenDrive : public testing::Test {
 public:
  static void SetUpTestCase();     // 在第一个case之前执行
  static void TearDownTestCase();  // 在最后一个case之后执行
  void SetUp() override;           // 在每个case之前执行
  void TearDown() override;        // 在每个case之后执行
};

void TestOpenDrive::SetUpTestCase() {}
void TestOpenDrive::TearDownTestCase() {}
void TestOpenDrive::TearDown() {}
void TestOpenDrive::SetUp() {}

TEST_F(TestOpenDrive, TestDemo) {}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
