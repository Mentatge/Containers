
#include "../containers/s21_queue.h"

#include <queue>

#include "gtest/gtest.h"

TEST(create_queue, check_create_queue_1) { s21::queue<int> ex; }

TEST(create_queue, check_create_queue_2) { s21::queue<char> ex; }

TEST(create_queue, check_copy_constructor_1) {
  s21::queue<int> st_1;
  st_1.push(21);
  st_1.push(2);
  st_1.push(8);
  st_1.push(-12);
  s21::queue<int> st_2(st_1);
  size_t size_1 = st_1.size();
  size_t size_2 = st_2.size();
  EXPECT_EQ(st_1.size(), st_2.size());
  for (size_t i = 0; i < size_1 && i < size_2; i++) {
    EXPECT_EQ(st_1.front(), st_2.front());
    st_1.pop();
    st_2.pop();
  }
}

TEST(create_queue, check_move_constructor) {
  s21::queue<int> st_1;
  st_1.push(1);
  st_1.push(2);
  st_1.push(3);

  std::queue<int> st_1_buf;
  st_1_buf.push(1);
  st_1_buf.push(2);
  st_1_buf.push(3);

  s21::queue<int> st_2(std::move(st_1));
  std::queue<int> st_2_buf(std::move(st_1_buf));

  while (st_2.size() > 0 || st_2_buf.size() > 0) {
    EXPECT_EQ(st_2.front(), st_2_buf.front());
    st_2.pop();
    st_2_buf.pop();
  }
}

TEST(create_queue, check_constructor_with_initialization_list_1) {
  s21::queue<int> st_1{1, 2, 3, 4, 5};
  std::deque<int> st_1_buf_que{1, 2, 3, 4, 5};
  std::queue<int> st_1_buf{st_1_buf_que};

  while (st_1.size() > 0 || st_1_buf.size() > 0) {
    EXPECT_EQ(st_1.front(), st_1_buf.front());
    st_1.pop();
    st_1_buf.pop();
  }
}

TEST(test_swap, cheack_swap_1) {
  s21::queue<int> st_1{1, 2, 3};
  s21::queue<int> st_2{3, 2, 1};

  std::deque<int> for_st_1_buf{1, 2, 3};
  std::deque<int> for_st_2_buf{3, 2, 1};

  std::queue<int> st_1_buf(for_st_1_buf);
  std::queue<int> st_2_buf(for_st_2_buf);

  st_1.swap(st_2);
  st_1_buf.swap(st_2_buf);

  while (st_2.size() > 0 || st_2_buf.size() > 0) {
    EXPECT_EQ(st_2.front(), st_2_buf.front());
    st_2.pop();
    st_2_buf.pop();
  }

  while (st_1.size() > 0 || st_1_buf.size() > 0) {
    EXPECT_EQ(st_1.front(), st_1_buf.front());
    st_1.pop();
    st_1_buf.pop();
  }
}

TEST(test_assignment1, check_assignment_1) {
  s21::queue<int> st_1{1, 2, 3};
  s21::queue<int> st_2 = st_1;

  std::deque<int> for_st_1_buf{1, 2, 3};

  std::queue<int> st_1_buf(for_st_1_buf);
  std::queue<int> st_2_buf = st_1_buf;

  while (!st_2.empty() || !st_2_buf.empty()) {
    EXPECT_EQ(st_2.front(), st_2_buf.front());
    st_2.pop();
    st_2_buf.pop();
  }
}

TEST(test_back, check_back_1) {
  s21::queue<int> st_1{1, 2, 3};
  s21::queue<int> st_2 = st_1;

  std::deque<int> for_st_1_buf{1, 2, 3};

  std::queue<int> st_1_buf(for_st_1_buf);
  std::queue<int> st_2_buf = st_1_buf;

  while (!st_2.empty() || !st_2_buf.empty()) {
    EXPECT_EQ(st_2.front(), st_2_buf.front());
    EXPECT_EQ(st_2.back(), st_2_buf.back());
    st_2.pop();
    st_2_buf.pop();
  }
}

TEST(test_insert_many1, check_insert_many_1) {
  s21::queue<int> st_1{1, 2, 3};
  st_1.insert_many_back(1, 2, 3);

  std::deque<int> st_2_buf_que{1, 2, 3, 1, 2, 3};
  std::queue<int> st_2(st_2_buf_que);

  while (!st_1.empty()) {
    EXPECT_EQ(st_1.front(), st_2.front());
    st_1.pop();
    st_2.pop();
  }
}
// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }