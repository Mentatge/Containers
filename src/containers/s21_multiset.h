#ifndef S21_MULTISET
#define S21_MULTISET

#include <exception>
#include <initializer_list>
#include <iostream>
#include <limits>

#include "s21_vector.h"

namespace s21 {

template <typename Key>
class multiset {
 public:
  class Node;
  class list_iterator;

 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = list_iterator;
  using const_iterator = const list_iterator;
  using size_type = size_t;

 private:
  Node* node_;
  size_type size_;

 public:
  multiset() : size_(0) { node_ = new Node(); }

  multiset(size_type n) : multiset() {
    for (size_type i = 0; i < n; i++) {
      push_back(Key());  // будет работать только для инт
    }
  }

  multiset(std::initializer_list<value_type> const& items) : multiset() {
    for (auto ptr = items.begin(); ptr != items.end(); ++ptr) {
      push_back(*ptr);
    }
    sort();
  }

  multiset(const multiset& ms) {
    for (iterator it = ((multiset&)ms).begin(); it != ((multiset&)ms).end();
         it++)
      push_back(*it);
    sort();
  }

  multiset(multiset&& ms) {
    node_ = ms.node_;
    size_ = ms.size_;
    ms.size_ = 0;
    ms.node_ = nullptr;
  }

  ~multiset() {
    clear();
    delete node_;
  }

  multiset operator=(multiset&& ms) {
    if (ms.node_ != node_) {
      this->~multiset();
      node_ = ms.node_;
      size_ = ms.size_;
      ms.size_ = 0;
      ms.node_ = nullptr;
    }
  }

  // -----------------------------------------------------------------------------------------------------------------------------//
  // ----------------------MULTISET-ELEMENT-ACCESS--------------------------------------------------------------------------------//
 public:
  const_reference front() { return node_->next_->value_; }
  const_reference back() { return node_->prev_->value_; }

  //------------------------------------------------------------------------------------------------------------------------------//
  // -----------------------MULTISET-ITERATORS------------------------------------------------------------------------------------//
 public:
  iterator begin() {
    if (size_ == 0) {
      return list_iterator(node_);
    } else {
      return list_iterator(node_->next_);
    }
  }

  iterator end() { return list_iterator(node_); }

  // -----------------------------------------------------------------------------------------------------------------------------//
  // ----------------------MULTISET-CAPACITY--------------------------------------------------------------------------------------//
 public:
  bool empty() { return !(node_->next_); }
  size_type size() { return size_; }
  size_type max_size() { return (s21::multiset<Key>::size_type) - 1; }

  //-----------------------------------------------------------------------------------------------------------------------------//
  //----------------------MULTISET-MODIFIERS-------------------------------------------------------------------------------------//
 public:
  iterator insert_pos(iterator pos, const_reference value) {
    if (size_ == 0) {
      Node* element = new Node(value, node_, node_);
      node_->next_ = element;
      node_->prev_ = element;
    } else {
      Node* element = new Node(value, pos.pos_, pos.pos_->prev_);
      pos.pos_->prev_->next_ = element;
      pos.pos_->prev_ = element;
    }
    ++size_;
    sort();
    return --pos;
  }

  iterator insert(const_reference value) {
    insert_pos(end(), value);
    return end();
  }

  void clear() {
    while (size_ > 0) {
      pop_front();
    }
  }

  void erase(iterator pos) {
    if (pos.pos_ != node_) {
      if (size_ == 1) {
        node_->next_ = nullptr;
        node_->prev_ = nullptr;
      } else {
        pos.pos_->next_->prev_ = pos.pos_->prev_;
        pos.pos_->prev_->next_ = pos.pos_->next_;
      }
    }

    delete pos.pos_;
    --size_;
  }

  void push_back(const_reference value) { insert_pos(end(), value); }

  void pop_back() { erase(--end()); }

  void push_front(const_reference value) { insert_pos(begin(), value); }

  void pop_front() { erase(begin()); }

  void swap(multiset& other) noexcept {
    Node* tmp = node_;
    node_ = other.node_;
    other.node_ = tmp;
    int tmp_size = size_;
    size_ = other.size_;
    other.size_ = tmp_size;
  }

  void merge(multiset& other) {
    if (node_ != other.node_) {
      const auto it = end();
      splice(it, other);
      sort();
    }
  }

  void splice(const_iterator pos, multiset& other) {
    iterator i = other.begin();
    iterator j = other.end();
    --j;
    if (size_ == 0) {
      pos.pos_->next_ = node_;
      pos.pos_->prev_ = node_;
      node_->next_ = i.pos_;
      node_->prev_ = j.pos_;
    } else {
      i.pos_->prev_ = pos.pos_->prev_;
      j.pos_->next_ = pos.pos_;
      pos.pos_->prev_->next_ = i.pos_;
      pos.pos_->prev_ = j.pos_;
    }
    size_ += other.size_;
    other.node_->next_ = nullptr;
    other.node_->prev_ = nullptr;
    other.size_ = 0;
  }

  void reverse() {
    int j = 0;
    for (iterator i = end(); i != end() || j == 0; --i) {
      Node* tmp = i.pos_->prev_;
      i.pos_->prev_ = i.pos_->next_;
      i.pos_->next_ = tmp;
      ++j;
    }
  }

  void unique() {
    for (iterator i = begin(); i != end(); ++i) {
      while (i.pos_->next_ != node_ &&
             (i.pos_->next_->value_ == i.pos_->value_)) {
        iterator j = i;
        erase(++j);
      }
    }
  }
  void sort() {
    int len = size_;
    while (--len) {
      bool swapped = false;
      for (iterator i = begin(); i != --(end()); ++i) {
        if (i.pos_->next_->value_ < i.pos_->value_) {
          iterator j(i);
          value_type tmp = i.pos_->next_->value_;
          erase(++j);
          insert_pos(i, tmp);
          swapped = true;
          --i;
        }
      }
      if (swapped == false) break;
    }
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    s21::vector<std::pair<iterator, bool>> res_vector{};
    size_type len = sizeof...(args);
    value_type res_arr[len] = {args...};
    for (size_type i = 0; i < len; ++i) {
      res_vector.push_back(std::make_pair(res_arr[i], true));
    }
    sort();
    return res_vector;
  }

  //------------------------------------------------------------------------------------------------------------------------------//
  // ----------------------MULTISET--LOOKUP---------------------------------------------------------------------------------------//
 public:
  size_type count(const Key& key) {
    size_t tmp = 0;
    for (iterator it = (begin()); it != (end()); ++it) {
      if (it.pos_->value_ == key) {
        tmp++;
      }
    }
    return tmp;
  }

  iterator find(const Key& key) {
    iterator tmp = end();
    for (iterator it = (begin()); it != (end()); ++it) {
      if (it.pos_->value_ == key) {
        *tmp = *it;
      }
    }
    return tmp;
  }

  bool contains(const Key& key) { return count(key); }

  iterator lower_bound(const Key& key) {
    bool logic = true;
    iterator tmp1 = end();
    for (iterator it = (begin()); it != (end()); ++it) {
      if (it.pos_->value_ >= key && logic) {
        *tmp1 = *it;
        logic = false;
      }
    }
    return tmp1;
  }

  iterator upper_bound(const Key& key) {
    bool logic = true;
    iterator tmp = end();
    for (iterator it = (begin()); it != (end()); ++it) {
      if ((it.pos_->value_ > key) && logic) {
        *tmp = *it;
        logic = false;
      }
    }
    return tmp;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    iterator tmp1 = lower_bound(key);
    iterator tmp2 = upper_bound(key);
    return std::pair<iterator, iterator>(tmp1, tmp2);
  }

  //------------------------------------------------------------------------------------------------------------------------------//
  // ----------------------MULTISET--CLASSES--------------------------------------------------------------------------------------//
  class Node {
   public:  /// бьли прайват
    Node* next_;
    Node* prev_;
    value_type value_;

   public:
    Node() : next_(nullptr), prev_(nullptr) {}
    Node(value_type value, Node* next, Node* prev) : Node() {
      value_ = value;
      next_ = next;
      prev_ = prev;
    }
    Node(const Node& other)
        : value_(other.value_), next_(other.next_), prev_(other.prev_) {}
    Node(Node&& other)
        : value_(other.value_), next_(other.next_), prev_(other.prev_) {
      other.next_ = nullptr;
      other.prev_ = nullptr;
    }
    ~Node() {
      next_ = nullptr;
      prev_ = nullptr;
    }
    bool operator==(const Node& other) {
      return (next_ == other.next_ && prev_ == other.prev_ &&
              value_ == other.value_);
    }  // сравнение, конструкторы
    bool operator!=(const Node& other) {
      return !(next_ == other.next_ && prev_ == other.prev_ &&
               value_ == other.value_);
    }  // сравнение, конструкторы
  };

  class list_iterator {
   public:  /// бьли прайват
    Node* pos_;

   public:
    list_iterator() : pos_(nullptr) {}
    list_iterator(Node* ptr) : pos_(ptr) {}
    list_iterator(const list_iterator& other) : pos_(other.pos_) {}
    // мув
    // = с копированием
    // с присвоением
    list_iterator operator++() {
      pos_ = pos_->next_;
      return pos_;
    }
    list_iterator operator--() {
      pos_ = pos_->prev_;
      return pos_;
    }
    list_iterator operator++(int) {
      list_iterator tmp(*this);
      pos_ = pos_->next_;
      return tmp;
    }
    list_iterator operator--(int) {
      list_iterator tmp(*this);
      pos_ = pos_->prev_;
      return tmp;
    }
    bool operator==(const list_iterator& other) { return (pos_ == other.pos_); }
    bool operator!=(const list_iterator& other) { return (pos_ != other.pos_); }
    reference operator*() { return pos_->value_; }
  };
};

};  // namespace s21

#endif