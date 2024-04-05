#include "../containers/s21_list.h"

#include <gtest/gtest.h>

TEST(List_Access, front) {
  s21::list<int> a{1, 2, 3};
  EXPECT_EQ(a.front(), 1);
}

TEST(List_Access, back) {
  s21::list<int> a = {1, 2, 3};
  EXPECT_EQ(a.back(), 3);
}

TEST(List_Capacity, empty_True) {
  s21::list<int> a = s21::list<int>();

  EXPECT_EQ(a.empty(), 1);
}

TEST(List_Capacity, empty_False) {
  s21::list<int> a = {1, 2};

  EXPECT_EQ(a.empty(), 0);
}

TEST(List_Capacity, size) {
  s21::list<int> a = {1, 2};

  EXPECT_EQ(a.size(), 2);
}

TEST(List_Capacity, max_size) {
  s21::list<int> a = {1, 2};

  EXPECT_EQ(a.max_size(), (s21::list<int>::size_type) - 1);
}

TEST(List_Iinitialize, Default) {
  s21::list<int> a = s21::list<int>();
  EXPECT_EQ(a.size(), 0);
}

TEST(List_Iinitialize, Initializer_List) {
  s21::list<int> a = {1, 2, 3};
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.front(), 1);
}

TEST(List_Iinitialize, Copy) {
  s21::list<int> a = {1, 2, 3};
  s21::list<int> b = a;
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(b.front(), 1);
}

TEST(List_Iinitialize, Move) {
  s21::list<int> a = {1, 2, 3};
  s21::list<int> b = s21::list<int>((s21::list<int> &&) a);
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 1);
}

TEST(List_Iinitialize, Move_Operation) {
  s21::list<int> a{1, 2, 3};
  s21::list<int> b{1, 2, 3};
  b = (s21::list<int> &&) a;
  EXPECT_EQ(a.size(), 0);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 1);
}

TEST(List_Iterators, begin) {
  s21::list<int> a = {1, 2, 3};
  EXPECT_EQ(*a.begin(), 1);
}

TEST(List_Iterators, end) {
  s21::list<int> a = {1, 2, 3};
  EXPECT_EQ(*--(a.end()), 3);
}

TEST(List_Modifiers, clear) {
  s21::list<int> a = {1, 2, 3};
  a.clear();
  EXPECT_EQ(a.size(), 0);
}

TEST(List_Modifiers, insert) {
  s21::list<int> a = {1, 2, 3};
  a.insert(++(a.begin()), 4);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(*++(a.begin()), 4);
}

TEST(List_Modifiers, erase) {
  s21::list<int> a = {1, 2, 3};
  a.erase(a.begin());
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(*a.begin(), 2);
}

TEST(List_Modifiers, push_back) {
  s21::list<int> a = {1, 2, 3};
  a.push_back(4);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(a.back(), 4);
}

TEST(List_Modifiers, pop_back) {
  s21::list<int> a = {1, 2, 3};
  a.pop_back();
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a.back(), 2);
}

TEST(List_Modifiers, push_front) {
  s21::list<int> a = {1, 2, 3};
  a.push_front(0);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(a.front(), 0);
}

TEST(List_Modifiers, pop_front) {
  s21::list<int> a = {1, 2, 3};
  a.pop_front();
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a.front(), 2);
}

TEST(List_Modifiers, swap) {
  s21::list<int> a = {1, 2, 3};
  s21::list<int> b = {4, 5, 6, 7};
  a.swap(b);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(a.front(), 4);
  EXPECT_EQ(b.front(), 1);
}

TEST(List_Modifiers, merge) {
  s21::list<int> a = {1, 2, 3};
  s21::list<int> b = {7, 5, 6, 4};
  a.merge(b);
  EXPECT_EQ(a.size(), 7);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 7);
}

TEST(List_Modifiers, splice) {
  s21::list<int> a = {1, 2, 3};
  s21::list<int> b = {4};
  a.splice(++(a.begin()), b);
  EXPECT_EQ(a.size(), 4);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 3);
}

TEST(List_Modifiers, reverse) {
  s21::list<int> a = {1, 2, 3};
  a.reverse();
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.front(), 3);
  EXPECT_EQ(a.back(), 1);
}

TEST(List_Modifiers, unique) {
  s21::list<int> a = {1, 2, 3, 3, 4, 5, 6, 4};
  a.unique();
  EXPECT_EQ(a.size(), 7);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 4);
}

TEST(List_Modifiers, sort) {
  s21::list<int> a{1, 3, 0, 7, -5};
  a.sort();
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.front(), -5);
  EXPECT_EQ(a.back(), 7);
}

TEST(List_Modifiers, insert_many) {
  s21::list<int> a = {1};
  a.insert_many(++a.begin(), 2, 3, 4, 5);
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 5);
}

TEST(List_Modifiers, insert_many_back) {
  s21::list<int> a = {1};
  a.insert_many_back(2, 3, 4, 5);
  EXPECT_EQ(a.size(), 5);
  EXPECT_EQ(a.front(), 1);
  EXPECT_EQ(a.back(), 5);
}

TEST(List_Modifiers, insert_many_front) {
  s21::list<int> a{1};
  a.insert_many_front(-5, -4, -3, -2, -1, 0);
  EXPECT_EQ(a.size(), 7);
  EXPECT_EQ(a.front(), -5);
  EXPECT_EQ(a.back(), 1);
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);

//   return RUN_ALL_TESTS();
// }
