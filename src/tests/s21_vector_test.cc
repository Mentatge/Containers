#include "../containers/s21_vector.h"

#include <gtest/gtest.h>

TEST(Vector_Access, at_Good) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.at(0), 1);
}

TEST(Vector_Access, at_Bad) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_ANY_THROW(a.at(3));
}

TEST(Vector_Access, at_Operation_Good) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a[0], 1);
}

TEST(Vector_Access, front) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.front(), 1);
}

TEST(Vector_Access, back) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.back(), 3);
}

TEST(Vector_Access, data) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(*(a.data()), 1);
}

TEST(Vector_Capacity, empty_True) {
  s21::vector<int> a = {};
  EXPECT_EQ(a.empty(), 1);
}

TEST(Vector_Capacity, empty_False) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.empty(), 0);
}

TEST(Vector_Capacity, size) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
}

TEST(Vector_Capacity, max_size) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.max_size(), (s21::vector<int>::size_type) - 1);
}

TEST(Vector_Capacity, reserve) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.capacity(), 3);
  a.reserve(10);
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.capacity(), 10);
}

TEST(Vector_Capacity, capacity) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.capacity(), 3);
}

TEST(Vector_Capacity, shrink_to_fit) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.capacity(), 3);
  a.reserve(10);
  EXPECT_EQ(a.capacity(), 10);
  a.shrink_to_fit();
  EXPECT_EQ(a.capacity(), 3);
}

TEST(Vector_Initialize, Default) {
  s21::vector<int> a = s21::vector<int>();
  EXPECT_EQ(a.size(), 0);
}

TEST(Vector_Initialize, Sized) {
  s21::vector<int> a = s21::vector<int>(10);
  EXPECT_EQ(a.size(), 10);
}

TEST(Vector_Initialize, Initializer_List) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
}

TEST(Vector_Initialize, Copy) {
  s21::vector<int> a{1, 2, 3};
  s21::vector<int> b = s21::vector<int>(a);
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(b.size(), 3);
}

TEST(Vector_Initialize, Move) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  s21::vector<int> b = s21::vector<int>((s21::vector<int> &&) a);
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(b.size(), 3);
}

TEST(Vector_Initialize, Move_Operation) {
  s21::vector<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  s21::vector<int> b = s21::vector<int>();
  b = (s21::vector<int> &&) a;
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(b.size(), 3);
}

TEST(Vector_Modifiers, clear) {
  s21::vector<int> a = {1, 2, 3};
  a.clear();
  EXPECT_EQ(a.size(), 0);
}

TEST(Vector_Modifiers, insert) {
  s21::vector<int> a = {1, 2, 3};
  a.insert(a.begin(), 0);
  EXPECT_EQ(a.size(), 4);
}

TEST(Vector_Modifiers, erase) {
  s21::vector<int> a = {1, 2, 3};
  a.erase(a.begin());
  EXPECT_EQ(a.size(), 2);
}

TEST(Vector_Modifiers, push_back) {
  s21::vector<int> a = {1, 2, 3};
  a.push_back(4);
  EXPECT_EQ(a.size(), 4);
}

TEST(Vector_Modifiers, pop_back) {
  s21::vector<int> a = {1, 2, 3};
  a.pop_back();
  EXPECT_EQ(a.size(), 2);
}

TEST(Vector_Modifiers, swap) {
  s21::vector<int> a = {1, 2, 3};
  s21::vector<int> b = {0};
  a.swap(b);
  EXPECT_EQ(a.size(), 1);
  EXPECT_EQ(b.size(), 3);
}

TEST(Vector_Modifiers, insert_many) {
  s21::vector<int> a = {1, 2, 3};
  a.insert_many(a.begin(), -3, -2, -1, 0);
  EXPECT_EQ(a.size(), 7);
}

TEST(Vector_Modifiers, insert_many_back) {
  s21::vector<int> a = {1, 2, 3};
  a.insert_many_back(4, 5, 6, 7);
  EXPECT_EQ(a.size(), 7);
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);

//   return RUN_ALL_TESTS();
// }
