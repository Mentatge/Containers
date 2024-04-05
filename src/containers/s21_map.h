#ifndef S21_MAP
#define S21_MAP

#include <exception>
#include <iostream>
#include <limits>
#include <utility>

#include "s21_vector.h"

namespace s21 {

template <typename Key, typename T>
class map {
 public:
  class Node;
  class map_iterator;

 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = map_iterator;
  using const_iterator = const map_iterator;
  using size_type = size_t;

 private:
  Node *node_;
  Node *begin_;
  int size_;

  // -----------------------------------------------------------------------------------------------------------------------------//
  // -----------------------------MAP-MEMBER-FUNCTIONS----------------------------------------------------------------------------//
 public:
  map() : size_(0) {
    node_ = new Node();
    begin_ = node_;
  }

  map(std::initializer_list<value_type> const &items) : map() {
    for (auto it = items.begin(); it != items.end(); ++it) {
      insert(*it);
    }
  }

  map(const map &m) : map() {
    for (auto it = ((map &)m).begin(); it != ((map &)m).end(); ++it) {
      insert(*it);
    }
  }

  map(map &&m) {
    node_ = m.node_;
    size_ = m.size_;
    begin_ = m.begin_;
    // m.node_->~Node();
    m.node_ = nullptr;
    m.begin_ = nullptr;
    m.size_ = 0;
  }

  map &operator=(map &&m) {
    if (m.node_ != node_) {
      this->~map();
      node_ = m.node_;
      size_ = m.size_;
      begin_ = m.begin_;
      m.node_ = nullptr;
      m.begin_ = nullptr;
      m.size_ = 0;
    }
    return *(this);
  }

  ~map() {
    clear();
    // begin_ = nullptr;
    delete node_;
    begin_ = nullptr;
  }
  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-ELEMENT-ACCESS-------------------------------------------------------------------------------//
 public:
  T &at(const Key &key) {
    iterator it = search(node_, key);
    if (it == end()) {
      // std::cout<<"\n `EXCEPTION!!!!!!!!!!!!";
      throw std::out_of_range("key is not in the map");
    } else {
      return std::get<1>(*it);
    }

    // search(node, key){
    //     Node* tmp = node;
    //     if (key == node->node_->std::get<0>(key_value_)){
    //         return iterator(node);
    //     } else {
    //         if(key <node->node_->std::get<0>(key_value_)){
    //             tmp = node->node_->left_;
    //             return search(tmp, key);
    //         } else {
    //             tmp = node->node_->right_;
    //             return search(tmp, key);
    //         }
    //     }
    // }
  };

  T &operator[](const Key &key) {
    try {
      return std::get<1>(*search(node_, key));
    } catch (...) {
      insert(std::make_pair(key, T()));
      return std::get<1>(*search(node_, key));
      ;  // value по дефолту;
    }
  };

  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP--ITERATORS-----------------------------------------------------------------------------------//
 public:
  iterator begin() {
    if (size_ == 0) {
      return map_iterator(node_);
    } else {
      return map_iterator(begin_);
    }
  }

  iterator end() { return map_iterator(node_); }

  key_type get_key(map_iterator it) { return std::get<0>(*it); }

  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-CAPACITY-------------------------------------------------------------------------------------//
 public:
  bool empty() { return !(size_); };
  size_type size() { return size_; };
  size_type max_size() {
    return std::numeric_limits<std::size_t>::max() / sizeof(Node) - 1;
  };

  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-MODIFIERS------------------------------------------------------------------------------------//
 public:
  void clear() {
    while (size_ > 0) {
      erase(begin());  // долго
    }
  }

  std::pair<iterator, bool> insert(const value_type &value) {
    // iterator it = begin();
    iterator it = find_node_to_add_son(node_, std::get<0>(value));
    // std::cout<<"\n inserting after "<< std::get<1>(it.pos_->key_value_);
    Node *element = new Node(value);  // добавить проверку на наличие
    element->parent_ = it.pos_;
    // if(element->parent_ != nullptr && element->parent_->parent_ != nullptr)
    // std::cout<<"\n initially parent of parent is
    // "<<std::get<1>(element->parent_->parent_->key_value_);
    if (it.pos_ == node_) {
      // std::cout<<"here";
      element->red_ = 0;
      begin_ = element;
      node_->parent_ = element;
      node_->left_ = element;
      element->right_ = node_;
      element->parent_ = node_;  // не было
    } else {
      if (std::get<0>(value) < get_key(it)) {
        it.pos_->left_ = element;
        insert_case2(element);
      } else {
        it.pos_->right_ = element;
        // std::cout<<"\n added to the right";
        insert_case2(element);  /// переподвесить begin;
      }
      // std::cout<<std::get<1>(tmp->parent_->key_value_);
      // std::cout<<"\n"<<"begin equal to "<<get_key(begin());
      if (std::get<0>(value) < std::get<0>(begin_->key_value_))
        begin_ = element;
      if (std::get<0>(value) > std::get<0>(node_->parent_->key_value_)) {
        // std::cout<<"\n changed end";
        node_->parent_ = element;  // было right_;
        element->right_ = node_;
      }
    }
    ++size_;
    // std::cout<<"\n added "<<std::get<1>(value)<<" root is
    // "<<std::get<1>(node_->left_->key_value_); std::cout<<"\n next is
    // "<<std::get<1>((++iterator(element)).pos_->key_value_)<<" prev is
    // "<<std::get<1>((--iterator(element)).pos_->key_value_); std::cout<<"\n
    // begin is "<<std::get<1>(begin_->key_value_)<<" --end is
    // "<<std::get<1>((--end()).pos_->key_value_); std::cout<<"\n now map lools
    // like "; for (iterator it = begin(); it != end(); ++it){
    //      std::cout<<"\n "<<std::get<1>(it.pos_->key_value_);
    // }
    // if(size_ > 1) {
    //     if(element->left_ != nullptr) std::cout<<"\n left is
    //     "<<std::get<1>(element->left_->key_value_); if(element->right_ !=
    //     nullptr) std::cout<<"\n right is
    //     "<<std::get<1>(element->right_->key_value_); if(element->parent_ !=
    //     nullptr) std::cout<<"\n parent is
    //     "<<std::get<1>(element->parent_->key_value_); if(element->parent_ !=
    //     nullptr && element->parent_->parent_ != nullptr) std::cout<<"\n
    //     parent of parent is
    //     "<<std::get<1>(element->parent_->parent_->key_value_);
    // }
    // std::cout<<"\n INSERTED!!!!!!! "<<std::get<0>(*it);
    return std::make_pair(it, true);
  }

  std::pair<iterator, bool> insert(const Key &key, const T &obj) {
    iterator it = search(node_, key);
    if (it.pos_ != node_) {
      it.pos_->key_value_ = std::make_pair(key, obj);
      return std::make_pair(it, true);
    } else {
      return std::make_pair(end(), false);
    }
  }

  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj) {
    auto res = insert(key, obj);
    if (std::get<1>(res) == true) {
      // std::cout<<"\n ASSIGNED!!!!!!! ";
      return res;
    } else {
      // std::cout<<"\n INSERTED!!!!!!! ";
      return insert(std::make_pair(key, obj));
    }
  }

  void erase(iterator pos) {
    // std::cout<<"\n before delete root is
    // "<<std::get<1>(node_->left_->key_value_)<<'\n'; iterator it =
    // search(node_, std::get<0>(pos.pos_->key_value_)); std::cout<<"\n pos is
    // "<< std::get<0>(pos.pos_->key_value_);
    if (size_ == 1) {
      node_->left_ = nullptr;
      node_->parent_ = nullptr;
      begin_ = node_;
    } else {
      if (pos.pos_ != node_) {
        // std::cout<<"\n entering delete ";
        if (pos.pos_->left_ != nullptr &&
            (pos.pos_->right_ != nullptr && pos.pos_->right_ != node_)) {
          // std::cout<<"\n here";
          delete_node_with_two_childs(pos.pos_);
        } else {
          // std::cout<<"\n entering delete_one_child ";
          delete_one_child(pos.pos_);
        }
      }
    }
    --size_;
    // std::cout<<"\n after delete size is "<<size_;
    for (iterator it = begin(); it != end(); ++it) {
      // std::cout<<"\n"<<std::get<1>(it.pos_->key_value_);
    }
    // std::cout<<"\n";
    // std::cout<<"\n after delete root is
    // "<<std::get<1>(node_->left_->key_value_)<<'\n'; std::cout<<"\n after
    // delete begin is "<<std::get<1>(begin_->key_value_)<<'\n'; std::cout<<"\n
    // after delete end is "<<std::get<1>(node_->parent_->key_value_)<<'\n';
    delete pos.pos_;
  }

  void swap(map &other) noexcept {
    Node *tmp = node_;
    node_ = other.node_;
    other.node_ = tmp;
    tmp = begin_;
    begin_ = other.begin_;
    other.begin_ = tmp;
    int tmp_size = size_;
    size_ = other.size_;
    other.size_ = tmp_size;
  }

  void merge(map &other) {
    for (iterator it = ((map &)other).begin(); it != ((map &)other).end();) {
      // std::cout<<"\n \n merging "<<std::get<1>(it.pos_->key_value_)<<"\n";
      if (search(node_, std::get<0>(it.pos_->key_value_)) == node_) {
        insert(it.pos_->key_value_);
        other.erase(it);
        // it = other.begin();
        // std::cout<<"inserted and erased";
        it = ((map &)other).begin();
      } else {
        ++it;
      }
    }
    // std::cout<<"\n size is "<<size_;
  }

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> res_vector{};
    size_t len = sizeof...(args);
    if (len == 0) return res_vector;
    value_type res_arr[len] = {args...};
    for (size_t i = 0; i < len; ++i) {
      res_vector.push_back(insert(res_arr[i]));
    }
    return res_vector;
  }
  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-LOOKUP---------------------------------------------------------------------------------------//
 public:
  bool contains(const Key &key) {
    auto it = search(node_, key);
    return (it != end());
  };

 private:
  iterator search(Node *node, const Key &key) {
    Node *tmp = node;
    if (node == node_) tmp = node_->left_;
    while (tmp != nullptr && (tmp != node_)) {
      if (key == std::get<0>(tmp->key_value_)) {
        return iterator(tmp);
      } else {
        if (key < std::get<0>(tmp->key_value_)) {
          tmp = tmp->left_;
        } else {
          tmp = tmp->right_;
        }
        // if tmp == nullptr{
        //     throw std::out_of_range("key is not in the map")
        // }else{
        // return search(tmp, key);
      }
    }
    // std::cout<<"\nkey is not in the map";
    return end();
    // if (tmp == nullptr){
    //     // throw std::out_of_range("key is not in the map");
    //     std::cout<<"\nkey is not in the map";
    //     return end(); // или возвращать nullptr; или делать catch
    // } else{
    //     if (key == std::get<0>(tmp->key_value_)){
    //         return iterator(node);
    //     } else {
    //         if(key < std::get<0>(tmp->key_value_)){
    //             tmp = tmp->left_;
    //         } else {
    //             tmp = tmp->right_;
    //         }
    //     // if tmp == nullptr{
    //     //     throw std::out_of_range("key is not in the map")
    //     // }else{
    //         return search(tmp, key);
    //     }
    // }
  }

  iterator find_node_to_add_son(Node *node, const Key &key) {
    if (size_ == 0) {
      return begin();
    } else {
      Node *tmp = node->left_;
      // int i = 0;
      while (tmp != nullptr && (tmp != node_)) {
        if (key < std::get<0>(tmp->key_value_)) {
          if (tmp->left_ == nullptr)
            return iterator(tmp);
          else
            tmp = tmp->left_;
        } else if (key > std::get<0>(tmp->key_value_)) {
          if (tmp->right_ == nullptr || tmp->right_ == node_)
            return iterator(tmp);
          else
            tmp = tmp->right_;
          // std::cout<<"fsdfdsfsfdf"<<"\n";
        } else {
          throw std::invalid_argument("key is already in the map");
        }
        // ++i;
      }
      // std::cout<<std::get<1>(tmp->parent_->key_value_);
      return iterator(tmp);
    }
  }
  // if (key < node->node_->std::get<0>(key_value_)){
  //     return iterator(node);
  // } else {
  //     if(key < node->node_->std::get<0>(key_value_)){
  //         tmp = node->node_->left_;
  //     } else {
  //         tmp = node->node_->right_;
  //     }
  // // if tmp == nullptr{
  // //     throw std::out_of_range("key is not in the map")
  // // }else{
  //     return search(tmp, key);
  // }
  // }
  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-ADDITIONAL-----------------------------------------------------------------------------------//
 private:
  Node *grandparent(Node *n) {
    if ((n != nullptr) && (n->parent_ != node_))
      return n->parent_->parent_;
    else
      return nullptr;
  }

  Node *sibling(Node *n) {
    if (n == n->parent_->left_)
      return n->parent_->right_;
    else
      return n->parent_->left_;
  }

  Node *uncle(Node *n) {
    Node *g = grandparent(n);
    if (g == nullptr) return nullptr;  // No grandparent means no uncle
    if (n->parent_ == g->left_)
      return g->right_;
    else
      return g->left_;
  }

  void rotate_left(Node *n) {
    Node *pivot = n->right_;

    pivot->parent_ = n->parent_;

    if (n->parent_ == node_)
      node_->left_ =
          pivot; /* при этом, возможно, pivot становится корнем дерева */
    if (n->parent_ != node_) {
      if (n->parent_->left_ == n)
        n->parent_->left_ = pivot;
      else
        n->parent_->right_ = pivot;
    }

    n->right_ = pivot->left_;
    if (pivot->left_ != nullptr) pivot->left_->parent_ = n;

    n->parent_ = pivot;
    pivot->left_ = n;
  }

  void rotate_right(Node *n) {
    Node *pivot = n->left_;

    pivot->parent_ = n->parent_;
    if (n->parent_ == node_)
      node_->left_ =
          pivot; /* при этом, возможно, pivot становится корнем дерева */
    if (n->parent_ != node_) {
      if (n->parent_->left_ == n)
        n->parent_->left_ = pivot;
      else
        n->parent_->right_ = pivot;
    }

    n->left_ = pivot->right_;
    if (pivot->right_ != nullptr) pivot->right_->parent_ = n;

    n->parent_ = pivot;
    pivot->right_ = n;
  }

  void replace_node(Node *n, Node *child) {
    if (child != nullptr) {
      child->parent_ = n->parent_;
    }
    if (n == n->parent_->left_) {
      n->parent_->left_ = child;
    } else {
      n->parent_->right_ = child;
    }
  }

  void delete_one_child(Node *n) {
    if (n == begin_) begin_ = (++begin()).pos_;
    if (n == node_->parent_) {
      node_->parent_ = (--(--end())).pos_;
      (--(--end())).pos_->right_ = node_;
    }
    // std::cout<<"\n deleting one child " << std::get<1>(n->key_value_);
    Node *child =
        (n->right_ == nullptr || n->right_ == node_) ? n->left_ : n->right_;
    // if (child != nullptr) std::cout<<"\n child is
    // "<<std::get<1>(child->key_value_);
    if (child != nullptr || n->red_ != 0) {
      // std::cout<<"\n child is "<<std::get<1>(child->key_value_);   // для
      // красных узлов либо узлов с одним нуллом
      replace_node(n, child);
      // std::cout<<"\n ???????????";
      if (n->red_ != 0) {
        // std::cout<<"\n replaced and changing begin and end";
        // if(n == begin_) begin_ = (++begin()).pos_;
        // if(n == node_->parent_) {
        //     node_->parent_ = (--(--end())).pos_;
        //     (--(--end())).pos_->right_ = node_;
        // }
        // delete n;
      }
    }
    if (n->red_ == 0) {
      // std::cout<<"\n still deleting one child";
      if (child != nullptr && (child->red_ == 1)) {
        // std::cout<<"\n yet still deleting one child";
        child->red_ = 0;
      } else {
        // std::cout<<"\n yet stilsfsdf";
        if (child != nullptr) {  // случай для черного узла с одним сыном узлом
          // std::cout<<"\n deleting child";
          delete_case1(child);
          // if(n == begin_) begin_ = (++begin()).pos_;
          // if(n == node_->parent_) {
          //     node_->parent_ = (--(--end())).pos_;
          //     (--(--end())).pos_->right_ = node_;
          // }
          // delete child;
        } else {  // случай для черного узла с сыновьями нуллптрами
          // std::cout<<"\n deleting n with color "<<n->red_;
          delete_case1(n);
          replace_node(n, child);
          // if(n == begin_) begin_ = (++begin()).pos_;
          // if(n == node_->parent_) {
          //     node_->parent_ = (--(--end())).pos_;
          //     (--(--end())).pos_->right_ = node_;
          // }
          // delete n;
          // replace_node(n, child);
          // std::cout<<"\n final line";
          // std::cout<<"\n size is";
        }
      }
    }
    // delete // нужно добавить удаление
    //  if(pos.pos_ == begin_) begin_ = (++begin()).pos_;
    //  if(pos.pos_ == node_->parent_) node_->parent_ = (--(--end())).pos_;
    //  if(n == begin_) begin_ = (++begin()).pos_;
    //  if(n == node_->parent_) node_->parent_ = (--(--end())).pos_;
    //  delete n;
  }

  void insert_case1(Node *n) {
    if (n->parent_ == node_)
      n->red_ = 0;
    else
      insert_case2(n);
  }

  void insert_case2(Node *n) {
    // std::cout<<"\n adding "<< std::get<1>(n->key_value_)<<" parent is
    // "<<std::get<1>(n->parent_->key_value_)<<" color of parent is
    // "<<n->parent_->red_;
    if (n->parent_->red_ == 0) {
      // std::cout<<" !!!!!!!!!!!!";
      return; /* Tree is still valid */
    } else
      insert_case3(n);
  }

  void insert_case3(Node *n) {
    // std::cout<<"!!!!!!!!!!!!";
    // std::cout<<"\n inserting 3";
    Node *u = uncle(n), *g;
    if ((u != nullptr) && (u->red_ == 1)) {
      // && (n->parent->color == RED) Второе условие проверяется в insert_case2,
      // то есть родитель уже является красным.
      n->parent_->red_ = 0;
      u->red_ = 0;
      g = grandparent(n);
      g->red_ = 1;
      insert_case1(g);
    } else {
      insert_case4(n);
    }
  }

  void insert_case4(Node *n) {
    // std::cout<<"\n inserting 4";
    Node *g = grandparent(n);
    if ((n == n->parent_->right_) && (n->parent_ == g->left_)) {
      rotate_left(n->parent_);
      n = n->left_;
    } else if ((n == n->parent_->left_) && (n->parent_ == g->right_)) {
      rotate_right(n->parent_);
      n = n->right_;
    }
    insert_case5(n);
  }

  void insert_case5(Node *n) {
    // std::cout<<"\n inserting 5";
    Node *g = grandparent(n);
    n->parent_->red_ = 0;
    g->red_ = 1;
    if ((n == n->parent_->left_) && (n->parent_ == g->left_)) {
      rotate_right(g);
    } else {
      rotate_left(g);
    }
  }

  void delete_case1(Node *n) {
    // std::cout<<"\n delete case1";
    if (n->parent_ != node_) delete_case2(n);
  }

  void delete_case2(Node *n) {
    // std::cout<<"\n delete case2";
    Node *s = sibling(n);
    // std::cout<<"\n still deleting case2";
    // std::cout<<"\n sibling is "<<std::get<1>(s->key_value_);
    // std::cout<<"\n root is "<<std::get<1>(node_->left_->key_value_);
    if (s->red_ == 1) {
      // std::cout<<"\n yet still deleting case2";
      n->parent_->red_ = 1;
      s->red_ = 0;
      if (n == n->parent_->left_)
        rotate_left(n->parent_);
      else
        rotate_right(n->parent_);
    }
    delete_case3(n);
  }

  void delete_case3(Node *n) {
    Node *s = sibling(n);
    // std::cout<<"\n sibling is "<<std::get<0>(s->key_value_);
    // std::cout<<"\n delete case3";
    if ((n->parent_->red_ == 0) && (s->red_ == 0) &&
        (s->left_ == nullptr || s->left_->red_ == 0) &&
        (s->right_ == nullptr || s->right_->red_ == 0)) {
      s->red_ = 1;
      delete_case1(n->parent_);
    } else
      // std::cout<<"\n delete case3";
      // std::cout<<"\n delete case3.1";
      delete_case4(n);
  }

  void delete_case4(Node *n) {
    // std::cout<<"\n delete case4";
    Node *s = sibling(n);
    if ((n->parent_->red_ == 1) && (s->red_ == 0) &&
        (s->left_ == nullptr || s->left_->red_ == 0) &&
        (s->right_ == nullptr || s->right_->red_ == 0)) {
      s->red_ = 1;
      n->parent_->red_ = 0;
    } else
      delete_case5(n);
  }

  void delete_case5(Node *n) {
    // std::cout<<"\n delete case5";
    Node *s = sibling(n);
    if (s->red_ == 0) {
      if ((n == n->parent_->left_) &&
          (s->right_ == nullptr || s->right_->red_ == 0) &&
          (s->left_ != nullptr &&
           s->left_->red_ == 1)  // было s->left_->red_ == 1
      ) {
        /* this last test is trivial too due to cases 2-4. */
        s->red_ = 1;
        s->left_->red_ = 0;
        rotate_right(s);
      } else if ((n == n->parent_->right_) &&
                 (s->left_ == nullptr || s->left_->red_ == 0) &&
                 (s->right_ != nullptr && s->right_->red_ == 1)) {
        /* this last test is trivial too due to cases 2-4. */
        s->red_ = 1;
        s->right_->red_ = 0;
        rotate_left(s);
      }
    }
    delete_case6(n);
  }

  void delete_case6(Node *n) {
    // std::cout<<"\n delete case6";
    Node *s = sibling(n);
    s->red_ = n->parent_->red_;
    n->parent_->red_ = 0;

    if (n == n->parent_->left_) {
      if (s->right_ != nullptr) s->right_->red_ = 0;
      rotate_left(n->parent_);
    } else {
      if (s->left_ != nullptr) s->left_->red_ = 0;
      rotate_right(n->parent_);
    }
  }

  void delete_node_with_two_childs(Node *n) {
    iterator it = find_the_biggest_in_left(n);
    // std::cout<<"\n the biggest in the left is
    // "<<std::get<0>(it.pos_->key_value_); if (n == n->parent_->left_){
    //     n->parent_->left_ = it.pos_;
    // } else{
    //     n->parent_->right_ = it.pos_;
    // }
    // it.pos_->parent_ = n->parent_;
    // it.pos_->left_ = n->left_;
    // it.pos_->right_ = n->right_;
    n->key_value_ = it.pos_->key_value_;
    // std::cout<<"\n entered deliting with two childs";
    delete_one_child(it.pos_);
    // if(it.pos_ == begin_) begin_ = (++begin()).pos_;
    // if(it.pos_ == node_->parent_) node_->parent_ = (--(--end())).pos_;
  }

  iterator find_the_biggest_in_left(Node *n) {
    Node *tmp = n->left_;
    while (tmp->right_ != nullptr && tmp->right_ != node_) {
      tmp = tmp->right_;
    }
    return iterator(tmp);
  }
  //----------------------------------------------------------------------------------------------------------------------------//
  //---------------------------MAP-CLASSES--------------------------------------------------------------------------------------//
 public:
  class Node {
   public:
    Node *left_;
    Node *right_;
    Node *parent_;
    bool red_;
    value_type key_value_;

   public:
    Node() : left_(nullptr), right_(nullptr), parent_(nullptr), red_(1) {}
    Node(value_type key_value) : Node() {
      key_value_ = key_value;
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      red_ = 1;
    }
    Node(const Node &other)
        : left_(other.left_),
          right_(other.right),
          parent_(other.parent_),
          red_(other.red_),
          key_value_(other.key_value_) {}
    Node(Node &&other)
        : left_(other.left_),
          right_(other.right),
          parent_(other.parent_),
          red_(other.red_),
          key_value_(other.key_value_) {
      other.left_ = nullptr;
      other.right_ = nullptr;
      other.parent_ = nullptr;
    }
    ~Node() {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
    }
    bool operator==(const Node &other) {
      return (left_ == other.left_ && right_ == other.prev_ &&
              parent_ == other.parent_ && red_ == other.red_ &&
              key_value_ == other.key_value_);
    }
    bool operator!=(const Node &other) {
      return !(left_ == other.left_ && right_ == other.prev_ &&
               parent_ == other.parent_ && red_ == other.red_ &&
               key_value_ == other.key_value_);
    }
  };

  class map_iterator {
   public:
    Node *pos_;

   public:
    map_iterator() : pos_(nullptr) {}
    map_iterator(Node *pos) : pos_(pos) {}
    map_iterator(const map_iterator &other) : pos_(other.pos_) {}

    map_iterator &operator=(const map_iterator &m) {
      if (&m != this) {
        pos_ = m.pos_;
      }
      return *this;
    }

    map_iterator operator++() {
      if (pos_->right_ == nullptr || pos_->right_ == pos_->parent_) {
        if (pos_->parent_->right_ == pos_) {
          Node *tmp = pos_->parent_;
          while (std::get<0>(tmp->key_value_) < std::get<0>(pos_->key_value_)) {
            tmp = tmp->parent_;
          }
          pos_ = tmp;
        } else {
          pos_ = pos_->parent_;
        }
      } else if (pos_->right_->left_ != nullptr &&
                 (std::get<0>(pos_->right_->left_->key_value_) <
                  std::get<0>(pos_->key_value_))) {
        pos_ = pos_->right_;
      } else {
        Node *tmp = pos_->right_;
        while (tmp->left_ != nullptr) {
          tmp = tmp->left_;
        }
        pos_ = tmp;
      }
      return pos_;
    }

    map_iterator operator--() {
      // if(pos_ != begin()){
      if (pos_->left_ == nullptr) {
        if (pos_->parent_->left_ == pos_) {
          Node *tmp = pos_->parent_;
          while (std::get<0>(tmp->key_value_) > std::get<0>(pos_->key_value_)) {
            tmp = tmp->parent_;
          }
          pos_ = tmp;
        } else {
          pos_ = pos_->parent_;
        }
      } else {
        Node *tmp = pos_->left_;
        while (tmp->right_ != nullptr && tmp->right_ != pos_) {
          tmp = tmp->right_;
        }
        pos_ = tmp;
      }
      // }
      return pos_;
    }

    map_iterator operator++(int) {
      map_iterator tmp(*this);
      if (pos_->right_ == nullptr) {
        if (pos_->parent_->right_ == pos_) {
          pos_ = pos_->parent_->parent_;
        } else {
          pos_ = pos_->parent_;
        }
      } else {
        pos_ = pos_->right_;
      }
      return tmp;
    }

    map_iterator operator--(int) {
      map_iterator tmp(*this);
      if (pos_ != begin()) {
        if (pos_->left_ == nullptr) {
          if (pos_->parent_->left_ == pos_) {
            pos_ = pos_->parent_->parent_;
          } else {
            pos_ = pos_->parent_;
          }
        } else {
          pos_ = pos_->left_;
        }
      }
      return tmp;
    }

    bool operator==(const map_iterator &other) { return (pos_ == other.pos_); }

    bool operator!=(const map_iterator &other) { return (pos_ != other.pos_); }

    reference operator*() { return pos_->key_value_; }
    // begin возвращает указатель на наименьший элемент(самый левый сын)
    // end фейковая вершина декремент которой(left) дает самого правого сына,
    // increment(parent) дает самого левого?
    //  end это парент у корня?
    // обычный инкремент должен идти в правого сына и найти самого левого
    // подсынаб если его нет то уйти в родителя и так далее
  };
};
}  // namespace s21

#endif