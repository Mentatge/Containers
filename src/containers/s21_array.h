#ifndef S21_ARRAY
#define S21_ARRAY

#include <exception>
#include <initializer_list>
#include <iostream>

namespace s21 {

template <typename T, std::size_t N>
class array {
 private:
  T arr_[N];
  size_t size_;

 public:
  class arr_iterator;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = arr_iterator;
  using const_iterator = const T *;
  using size_type = size_t;
  using static_type_size = T[N];

  // -----------------------------------------------------------------------------------------------------------------------------//
  // ----------------------------ARRAY-MEMBER-FUNCTIONS---------------------------------------------------------------------------//
 public:
  array() : size_(N){};
  array(std::initializer_list<value_type> const &items) : size_(N) {
    int i = 0;
    for (auto it = items.begin(); it != items.end(); ++it) {
      arr_[i++] = *it;
    }
  };
  array(const array &a) : size_(a.size_) {
    for (size_t i = 0; i < size_; i++) {
      arr_[i] = a.arr_[i];
    }
  };
  array(array &&a) : size_(a.size_) {
    for (int i = 0; i < size_; i++) {
      arr_[i] = a.arr_[i];
    }
    a.arr_ = nullptr;
    a.size_ = 0;
  }
  ~array() = default;
  array &operator=(array &&a) {
    for (int i = 0; i < size_; i++) {
      arr_[i] = a.arr_[i];
    }
    a.arr_ = nullptr;
    a.size_ = 0;
  };
  //------------------------------------------------------------------------------------------------------------------------------//
  // --------------------------ARRAY-ITERATORS------------------------------------------------------------------------------------//

  iterator begin() { return arr_iterator(arr_); }
  iterator end() { return arr_iterator(arr_ + size_); }
  int distance(iterator begin, iterator end) { return end - begin; }
  // -----------------------------------------------------------------------------------------------------------------------------//
  // --------------------------ARRAY-ELEMENT-ACCESS-------------------------------------------------------------------------------//
 public:
  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("The number is greater than size of array");
    } else {
      return *(arr_ + pos);
    }
  }
  reference operator[](size_type pos) { return *(arr_ + pos); }
  // const_reference operator[](size_type pos) const {return *(arr_ + pos);}
  const_reference front() noexcept { return *(arr_); }
  const_reference back() noexcept { return *(arr_ + size_ - 1); }
  iterator data() { return arr_; }

  // -----------------------------------------------------------------------------------------------------------------------------//
  // --------------------------ARRAY-CAPACITY-------------------------------------------------------------------------------------//

 public:
  bool empty() { return !N; };
  size_type size() noexcept { return end() - begin(); }
  size_type max_size() noexcept { return distance(begin(), end()); }

  //-----------------------------------------------------------------------------------------------------------------------------//
  // --------------------------ARRAY-MODIFIERS-----------------------------------------------------------------------------------//
 public:
  void swap(array &other) noexcept {
    for (size_t i = 0; i < size_; i++) {
      T tmp = arr_[i];
      arr_[i] = other.arr_[i];
      other.arr_[i] = tmp;
    }
  }
  void fill(const_reference value) noexcept {
    for (size_t i = 0; i < size_; i++) arr_[i] = value;
  }

  //-----------------------------------------------------------------------------------------------------------------------------//
  // --------------------------ARRAY-CLASS-ITERATOR------------------------------------------------------------------------------//
  class arr_iterator {
   private:
    T *pos_;

   public:
    arr_iterator() : pos_(nullptr) {}
    arr_iterator(T *ptr) : pos_(ptr) {}
    arr_iterator operator++() { return ++pos_; }
    arr_iterator operator--() { return --pos_; }
    arr_iterator operator++(int) { return pos_++; }
    arr_iterator operator--(int) { return pos_--; }
    int operator-(const arr_iterator &other) { return pos_ - other.pos_; }
    reference operator*() { return *pos_; }
    bool operator==(iterator other) { return pos_ == other.pos_; }
    bool operator!=(iterator other) { return pos_ != other.pos_; }
  };
};

}  // namespace s21

#endif  // S21_ARRAY
