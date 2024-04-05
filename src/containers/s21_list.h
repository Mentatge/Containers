#ifndef S21_LIST
#define S21_LIST

#include <exception>
#include <iostream>
#include <limits>

#include "list"

namespace s21 {

template <typename T>
class list {
 public:
  class Node;
  class list_iterator;

 public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using const_reference = const T&;
  using iterator = list_iterator;
  using const_iterator = const list_iterator;
  using size_type = size_t;

 private:
  Node* node_;
  size_type size_;

  // -----------------------------------------------------------------------------------------------------------------------------//
  // ----------------------------LIST-MEMBER-FUNCTIONS----------------------------------------------------------------------------//
 public:
  list() : size_(0) { node_ = new Node(); }

  list(size_type n) : list() {
    for (size_type i = 0; i < n; i++) {
      push_back(T());  // будет работать только для инт
    }
  }

  list(std::initializer_list<value_type> const& items) : list() {
    for (auto ptr = items.begin(); ptr != items.end(); ++ptr) {
      push_back(*ptr);  //
    }
  }

  list(const list& l) : list() {
    for (iterator it = ((list&)l).begin(); it != ((list&)l).end(); ++it) {
      push_back(*it);
    }
  }

  list(list&& l) {
    node_ = l.node_;
    size_ = l.size_;
    l.size_ = 0;
    l.node_ = nullptr;  ///проверить??????????????
  }

  ~list() {
    clear();
    delete node_;  ///проверить??????????????
  }

  list operator=(list&& l) {
    if (l.node_ != node_) {
      this->~list();  //
      node_ = l.node_;
      size_ = l.size_;
      l.size_ = 0;
      l.node_ = nullptr;  ///проверить??????????????
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------------------------------------------//
  // --------------------------LIST-ELEMENT-ACCESS--------------------------------------------------------------------------------//
 public:
  const_reference front() { return node_->next_->value_; }
  const_reference back() {
    return node_->prev_->value_;
  }  // проверки на наличие элемента

  //------------------------------------------------------------------------------------------------------------------------------//
  // --------------------------LIST--ITERATORS------------------------------------------------------------------------------------//
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
  // --------------------------LIST-CAPACITY-------------------------------------------------------------------------------------//
 public:
  bool empty() { return !(node_->next_); }
  size_type size() { return size_; }
  // size_type max_size() {return std::numeric_limits<std::size_t>::max()/
  // sizeof(Node);}
  size_type max_size() { return (s21::list<int>::size_type) - 1; }

  //-----------------------------------------------------------------------------------------------------------------------------//
  //--------------------------LIST-MODIFIERS-----------------------------------------------------------------------------------//
 public:
  void clear() {
    while (size_ > 0) {
      pop_front();
    }
  }

  iterator insert(iterator pos, const_reference value) {
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
    return --pos;
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

      // delete pos.pos_; ////?????????????
      // pos.pos_ = nullptr;
      delete pos.pos_;
      --size_;
    }
  }

  void push_back(const_reference value) { insert(end(), value); }

  void pop_back() { erase(--end()); }

  void push_front(const_reference value) { insert(begin(), value); }

  void pop_front() { erase(begin()); }

  void swap(list& other) noexcept {
    Node* tmp = node_;
    node_ = other.node_;
    other.node_ = tmp;
    int tmp_size = size_;
    size_ = other.size_;
    other.size_ = tmp_size;
  }

  void merge(list& other) {
    if (node_ != other.node_) {
      const auto it = end();
      splice(it, other);
      sort();
    }
  }

  void splice(const_iterator pos, list& other) {
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
          insert(i, tmp);
          swapped = true;
          --i;
        }
      }
      if (swapped == false) break;
    }
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    size_type len = sizeof...(args);
    value_type arr[len] = {args...};
    for (size_type i = 0; i < len; ++i) {
      insert(pos, arr[i]);
    }
    iterator it = pos;
    return --it;
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    size_type len = sizeof...(args);
    value_type arr[len] = {args...};
    for (size_type i = 0; i < len; ++i) {
      push_back(arr[i]);
    }
  }

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    int len = sizeof...(args);
    value_type arr[len] = {args...};
    for (int i = len - 1; i >= 0; --i) {
      push_front(arr[i]);
    }
  }

  //------------------------------------------------------------------------------------------------------------------------------//
  // --------------------------LIST--CLASSES--------------------------------------------------------------------------------------//
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
}  // namespace s21

#endif  // S21_LIST
