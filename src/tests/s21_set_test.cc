#include "../containers/s21_set.h"

#include <gtest/gtest.h>

#include <set>

TEST(S21SetTests, initializer_list_constructor_test) {
  s21::set<int> a{1, 2, 4, 5, 6, 7, 8, 9};
  std::set<int> b{1, 2, 4, 5, 6, 7, 8, 9};
  s21::set<int>::iterator a_it = a.begin();
  std::set<int>::iterator b_it = b.begin();
  for (; a_it != a.end(); ++a_it, ++b_it) {
    ASSERT_TRUE(*a_it == *b_it);
  }
}

TEST(S21SetTests, default_constructor_empty_test) {
  s21::set<int> a;
  std::set<int> b;
  ASSERT_TRUE(a.empty() == b.empty());
}

TEST(S21SetTests, copy_constructor_test) {
  s21::set<int> a_orig{1, 2, 4, 5, 6, 7, 8, 9};
  s21::set<int> a_copy(a_orig);
  s21::set<int>::iterator a_orig_it = a_orig.begin();
  s21::set<int>::iterator a_copy_it = a_copy.begin();
  for (; a_orig_it != a_orig.end(); ++a_orig_it, ++a_copy_it) {
    ASSERT_TRUE(*a_orig_it == *a_copy_it);
  }
  --a_copy_it;
}

TEST(S21SetTests, move_constructor_test) {
  s21::set<int> a_orig{1, 2, 4, 5, 6, 7, 8, 9};
  size_t orig_size = a_orig.size();
  s21::set<int> a_copy(std::move(a_orig));
  //   std::cout << orig_size << std::endl;
  ASSERT_EQ(orig_size, a_copy.size());
  ASSERT_TRUE(a_orig.empty());
}

TEST(S21SetTests, size_test) {
  s21::set<int> a_1{1, 2, 4, 5, 6, 7, 8, 9};
  std::set<int> b_1{1, 2, 4, 5, 6, 7, 8, 9};
  s21::set<int> a_2;
  std::set<int> b_2;

  ASSERT_TRUE(a_1.size() == b_1.size());
  ASSERT_TRUE(a_2.size() == b_2.size());
}

TEST(S21SetTests, clear_test) {
  s21::set<int> a{1, 2, 4, 5, 6, 7, 8, 9};
  std::set<int> b{1, 2, 4, 5, 6, 7, 8, 9};
  a.clear();
  b.clear();
  ASSERT_TRUE(a.empty() == b.empty());
}

TEST(S21SetTests, insert_erase_test) {
  s21::set<int> a{1, 3, 7};
  std::set<int> b{1, 3, 7};
  std::pair<s21::set<int>::iterator, bool> a_pair = a.insert(4);
  std::pair<std::set<int>::iterator, bool> b_pair = b.insert(4);
  //   ASSERT_TRUE(*(a_pair.first) == *(b_pair.first));
  ASSERT_TRUE(a_pair.second == b_pair.second);
  s21::set<int>::iterator a_it = a.begin();
  std::set<int>::iterator b_it = b.begin();
  for (; a_it != a.end(); ++a_it, ++b_it) {
    ASSERT_TRUE(*a_it == *b_it);
  }
  a.erase(a_pair.first);
  b.erase(b_pair.first);
  ASSERT_TRUE(a_pair.second == b_pair.second);
}

TEST(S21SetTests, swap) {
  s21::set<int> a{1, 3, 7};
  s21::set<int> b{2, 4, 8};

  s21::set<int> a_2{2, 4, 8};
  s21::set<int> b_2{1, 3, 7};

  a.swap(b);

  s21::set<int>::iterator a_it = a.begin();
  s21::set<int>::iterator a_2it = a.begin();
  s21::set<int>::iterator b_it = b_2.begin();
  s21::set<int>::iterator b_2it = b_2.begin();
  for (; a_it != a.end(); ++a_it, ++a_2it, ++b_it, ++b_2it) {
    ASSERT_TRUE(*a_it == *a_2it);
    ASSERT_TRUE(*b_it == *b_2it);
  }
}

TEST(S21SetTests, contains_test) {
  s21::set<int> a{1, 2, 4, 5, 6, 7, 8, 9};
  ASSERT_TRUE(a.contains(1));
  ASSERT_FALSE(a.contains(10));
}

TEST(S21SetTests, insert_many_test) {
  s21::set<int> a;
  std::set<int> b{1, 3, 7};
  a.insert_many(1, 3, 7);
  s21::set<int>::iterator a_it = a.begin();
  std::set<int>::iterator b_it = b.begin();
  for (; a_it != a.end(); ++a_it, ++b_it) {
    ASSERT_TRUE(*a_it == *b_it);
  }
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }