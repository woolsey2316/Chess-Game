#ifndef KINGTEST_H
#define KINGTEST_H

#include "King.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class KingTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  KingTest() {
     // You can do set-up work for each test here.

  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(KingTest, MethodBarDoesAbc) {
  std::vector<std::array<int, 2>> answer;
  answer.push_back(std::array<int,2>{0,5});
  answer.push_back(std::array<int,2>{1,5});
  answer.push_back(std::array<int,2>{1,4});
  answer.push_back(std::array<int,2>{1,3});
  answer.push_back(std::array<int,2>{0,3});
  King king("WHITE", "KING");
  ASSERT_EQ(king.possibleMoves(0, 4), answer);
}

// Tests that Foo does Xyz.
TEST_F(KingTest, DoesXyz) {
  std::vector<std::array<int, 2>> answer;
  answer.push_back(std::array<int,2>{1,5});
  answer.push_back(std::array<int,2>{3,5});
  answer.push_back(std::array<int,2>{1,4});
  answer.push_back(std::array<int,2>{3,4});
  answer.push_back(std::array<int,2>{1,6});
  answer.push_back(std::array<int,2>{2,6});
  answer.push_back(std::array<int,2>{3,6});
  King king("WHITE", "KING");
  ASSERT_EQ(king.possibleMoves(2, 5), answer);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif // KINGTEST_H
