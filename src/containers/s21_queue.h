#ifndef S21_QUEUE
#define S21_QUEUE

#include <iostream>

#include "s21_list.h"

namespace s21 {
template <typename T, class Container = s21::list<T>>
class queue {
 public:
  // ------------------------Member type----------------- //
  using value_type = typename Container::value_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using size_type = typename Container::size_type;
  // --------------------Functions-------------------------//
  queue() : container_(){};
  queue(std::initializer_list<value_type> const &items) : container_(items){};
  queue(const queue &q) : container_(q.container_){};
  queue(queue &&q) : container_(std::move(q.container_)){};
  ~queue(){};
  queue operator=(queue &&q) {
    container_ = q.container_;
    return *this;
  };
  //----------------------Element access------------------//

  const_reference front() { return container_.front(); }
  const_reference back() { return container_.back(); }
  //---------------------------Capacity--------------------//
  bool empty() { return container_.empty(); }
  size_type size() { return container_.size(); }
  //------------------Modifiers---------------------------//
  void push(const_reference value) {
    // const_reference val = value;
    container_.push_back(value);
  }

  void pop() { container_.pop_front(); }

  void swap(queue &other) { container_.swap(other.container_); }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    size_type len = sizeof...(args);
    value_type arr[len] = {args...};
    for (size_type i = 0; i < len; ++i) {
      container_.push_back(arr[i]);
    }
  }

 private:
  Container container_;
};
}  // namespace s21
#endif