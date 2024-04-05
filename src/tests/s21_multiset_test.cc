#include "../containers/s21_multiset.h"

#include <gtest/gtest.h>

#include <iostream>
#include <set>

TEST(S21MultiSetTests, initializer_list_constructor_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9, 8, 2};
  std::multiset<int> b{1, 1, 2, 4, 5, 6, 7, 8, 9, 8, 2};
  s21::multiset<int>::iterator a_it = a.begin();
  std::multiset<int>::iterator b_it = b.begin();
  for (; a_it != a.end(); ++a_it, ++b_it) {
    ASSERT_TRUE(*a_it == *b_it);
  }
}

TEST(S21MultiSetTests, default_constructor_empty_test) {
  s21::multiset<int> a;
  std::multiset<int> b;
  ASSERT_TRUE(a.empty() == b.empty());
}

TEST(S21MultiSetTests, size_test) {
  s21::multiset<int> a_1{1, 1, 2, 4, 5, 6, 7, 8, 9};
  std::multiset<int> b_1{1, 1, 2, 4, 5, 6, 7, 8, 9};
  s21::multiset<int> a_2;
  std::multiset<int> b_2;

  ASSERT_TRUE(a_1.size() == b_1.size());
  ASSERT_TRUE(a_2.size() == b_2.size());
}

TEST(S21MultiSetTests, clear_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9};
  std::multiset<int> b{1, 1, 2, 4, 5, 6, 7, 8, 9};
  a.clear();
  b.clear();
  ASSERT_TRUE(a.empty() == b.empty());
}

TEST(S21MultiSetTests, insert_erase_test) {
  s21::multiset<int> a{1, 3, 7};
  std::multiset<int> b{1, 3, 7};
  a.insert(4);
  b.insert(4);
  s21::multiset<int>::iterator a_it = a.begin();
  std::multiset<int>::iterator b_it = b.begin();
  for (; a_it != a.end(); ++a_it, ++b_it) {
    ASSERT_TRUE(*a_it == *b_it);
  }
}

TEST(S21multisetTests, swap) {
  s21::multiset<int> a{1, 3, 7};
  s21::multiset<int> b{2, 4, 8};

  s21::multiset<int> a_2{2, 4, 8};
  s21::multiset<int> b_2{1, 3, 7};

  a.swap(b);

  s21::multiset<int>::iterator a_it = a.begin();
  s21::multiset<int>::iterator a_2it = a.begin();
  s21::multiset<int>::iterator b_it = b_2.begin();
  s21::multiset<int>::iterator b_2it = b_2.begin();
  for (; a_it != a.end(); ++a_it, ++a_2it, ++b_it, ++b_2it) {
    ASSERT_TRUE(*a_it == *a_2it);
    ASSERT_TRUE(*b_it == *b_2it);
  }
}

TEST(S21multisetTests, merge_test) {
  s21::multiset<char> p{'A', 'B', 'C'};
  s21::multiset<char> q{'D', 'E'};
  s21::multiset<char> p_check{'A', 'B', 'C', 'D', 'E'};
  s21::multiset<char> q_check{'C'};
  p.merge(q);
  s21::multiset<char>::iterator p_it = p.begin();
  s21::multiset<char>::iterator p_check_it = p_check.begin();
  for (; p_it != p.end(); ++p_it, ++p_check_it) {
    ASSERT_TRUE(*p_it == *p_check_it);
  }
  s21::multiset<char>::iterator q_it = q.begin();
  s21::multiset<char>::iterator q_check_it = q_check.begin();
  for (; q_it != q.end(); ++q_it, ++q_check_it) {
    ASSERT_TRUE(*q_it == *q_check_it);
  }
}

TEST(S21multisetTests, count_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9};
  s21::multiset<int> b;

  ASSERT_TRUE(a.count(1) == 2);
  ASSERT_TRUE(a.count(10) == 0);
  ASSERT_TRUE(b.count(10) == 0);
}

TEST(S21multisetTests, find_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9};
  s21::multiset<int> b;

  ASSERT_TRUE(*a.find(1) == *a.begin());
  ASSERT_TRUE(*(a.find(2)) == 2);
}

TEST(S21multisetTests, contains_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9};
  s21::multiset<int> b;

  ASSERT_TRUE(a.contains(1));
  ASSERT_FALSE(a.contains(10));
  ASSERT_FALSE(b.contains(10));
}

TEST(S21multisetTests, equal_range_test) {
  s21::multiset<int> a{1, 1, 2, 4, 5, 6, 7, 8, 9};
  std::pair<typename s21::multiset<int>::iterator,
            typename s21::multiset<int>::iterator>
      res;
  res = a.equal_range(4);
  ASSERT_TRUE(*(res.first) == 5);
  ASSERT_TRUE(*(res.second) == 5);
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }