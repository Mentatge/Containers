#ifndef S21_sTACK
#define S21_sTACK

#include <exception>
#include <iostream>

#include "s21_list.h"

namespace s21 {

template <typename T, class Container = s21::list<T>>
class stack {
 public:
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  // -------sTACK Functions -------------------------//
 public:
  stack() : container_(){};

  stack(std::initializer_list<value_type> const &items) : container_(items){};

  stack(const stack &s) : container_(s.container_){};

  stack(stack &&s)
      : container_(std::move(s.container_)){};  // check как чекнуть то ошибка
                                                // вылезает нада проверять

  ~stack(){
      // std::cout << "destraction" << std::endl;
  };

  stack operator=(stack &&s) {
    container_ = s.container_;
    return *this;
  }
  //-------------Element access--------//
  const_reference top() { return container_.back(); }

  //------------Capacity --------------//
  bool empty() { return container_.empty(); }

  size_type size() { return container_.size(); }
  // ----------- Modifiers ------------//
  void push(const_reference value) { container_.push_back(value); }

  void pop() { container_.pop_back(); }

  void swap(stack &other) { container_.swap(other.container_); }

  template <typename... Args>
  void insert_many_front(Args &&...args) {
    int len = sizeof...(args);
    value_type arr[len] = {args...};
    for (int i = len - 1; i >= 0; --i) {
      container_.push_front(arr[i]);
    }
  }

 private:
  Container container_;
};
}  // namespace s21
#endif  // S21_sTACK
