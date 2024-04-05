#ifndef S21_SET
#define S21_SET

#include <exception>
#include <iostream>
#include <limits>
#include <vector>

namespace s21 {
template <typename Key>
class set {
 private:
  class Node;
  class set_iterator;

 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = set_iterator;
  using const_iterator = const set_iterator;
  using size_type = size_t;

 private:
  Node *begin_;
  Node *end_;
  int size_;

  Node *sibling(Node *n) {
    if (n == n->parent->left)
      return n->parent->right;
    else
      return n->parent->left;
  }

  void replace_node(Node *n, Node *child) {
    if (child != nullptr) {
      child->parent = n->parent;
    }
    if (n == n->parent->left) {
      n->parent->left = child;
    } else {
      n->parent->right = child;
    }
  }

  void delete_case1(Node *n) {
    if (n->parent != end_) delete_case2(n);
  }

  void delete_case2(Node *n) {
    Node *s = sibling(n);
    if (s->red == 1) {
      n->parent->red = 1;
      s->red = 0;
      if (n == n->parent->left)
        rotate_left_(n->parent);
      else
        rotate_right_(n->parent);
    }
    delete_case3(n);
  }

  void delete_case3(Node *n) {
    Node *s = sibling(n);
    if ((n->parent->red == 0) && (s->red == 0) &&
        (s->left == nullptr || s->left->red == 0) &&
        (s->right == nullptr || s->right->red == 0)) {
      s->red = 1;
      delete_case1(n->parent);
    } else
      // std::cout<<"\n delete case3";
      // std::cout<<"\n delete case3.1";
      delete_case4(n);
  }

  void delete_case4(Node *n) {
    Node *s = sibling(n);
    if ((n->parent->red == 1) && (s->red == 0) &&
        (s->left == nullptr || s->left->red == 0) &&
        (s->right == nullptr || s->right->red == 0)) {
      s->red = 1;
      n->parent->red = 0;
    } else
      delete_case5(n);
  }

  void delete_case5(Node *n) {
    // std::cout<<"\n delete case5";
    Node *s = sibling(n);
    if (s->red == 0) {
      if ((n == n->parent->left) &&
          (s->right == nullptr || s->right->red == 0) &&
          (s->left != nullptr && s->left->red == 1)  // было s->left_->red_ == 1
      ) {
        /* this last test is trivial too due to cases 2-4. */
        s->red = 1;
        s->left->red = 0;
        rotate_right_(s);
      } else if ((n == n->parent->right) &&
                 (s->left == nullptr || s->left->red == 0) &&
                 (s->right != nullptr && s->right->red == 1)) {
        /* this last test is trivial too due to cases 2-4. */
        s->red = 1;
        s->right->red = 0;
        rotate_left_(s);
      }
    }
    delete_case6(n);
  }

  void delete_case6(Node *n) {
    Node *s = sibling(n);
    s->red = n->parent->red;
    n->parent->red = 0;

    if (n == n->parent->left) {
      if (s->right != nullptr) s->right->red = 0;
      rotate_left_(n->parent);
    } else {
      if (s->left != nullptr) s->left->red = 0;
      rotate_right_(n->parent);
    }
  }

  void setnullptr_() {
    begin_ = nullptr;
    end_ = nullptr;
    size_ = 0;  // закомитил хз зачем
  }

  Node *grandparent_(Node *n) {
    if ((n != nullptr) && (n->parent != nullptr))
      return n->parent->parent;
    else
      return nullptr;
  }

  Node *uncle_(Node *n) {
    Node *g = grandparent_(n);
    if (g == nullptr) return nullptr;  // No grandparent means no uncle
    if (n->parent == g->left)
      return g->right;
    else
      return g->left;
  }

  void rotate_left_(Node *n) {
    Node *pivot = n->right;
    pivot->parent = n->parent;
    if (n->parent == end_) {
      end_->left = pivot;
    } /* при этом, возможно, pivot становится корнем дерева */
    if (n->parent != end_) {
      if (n->parent->left == n)
        n->parent->left = pivot;
      else
        n->parent->right = pivot;
    }
    n->right = pivot->left;
    if (pivot->left != nullptr) pivot->left->parent = n;
    n->parent = pivot;
    pivot->left = n;
  }

  void rotate_right_(Node *n) {
    Node *pivot = n->left;
    pivot->parent = n->parent;
    if (n->parent == end_) {
      end_->left = pivot;
    } /* при этом, возможно, pivot становится корнем дерева */
    if (n->parent != end_) {
      if (n->parent->left == n)
        n->parent->left = pivot;
      else
        n->parent->right = pivot;
    }
    n->left = pivot->right;
    if (pivot->right != nullptr) pivot->right->parent = n;
    n->parent = pivot;
    pivot->right = n;
  }

  void insert_case1_(Node *n) {
    if (n->parent == end_)
      n->red = false;
    else
      insert_case2_(n);
  }

  void insert_case2_(Node *n) {
    // std::cout << n->parent->red << " colored \n";
    // std::cout << n->parent->key << " key parent";
    if (n->parent->red == false) {
      // std::cout << "insert case 2 ";
      return; /* Tree is still valid */
    } else
      insert_case3_(n);
  }

  void insert_case3_(Node *n) {
    Node *u = uncle_(n), *g;
    if ((u != nullptr) && (u->red == true)) {
      // && (n->parent->red == RED) Второе условие проверяется в insert_case2,
      // то есть родитель уже является красным.
      n->parent->red = false;
      u->red = false;
      g = grandparent_(n);
      g->red = true;
      insert_case1_(g);
    } else {
      insert_case4_(n);
    }
  }

  void insert_case4_(Node *n) {
    Node *g = grandparent_(n);

    if ((n == n->parent->right) && (n->parent == g->left)) {
      rotate_left_(n->parent);
      n = n->left;
    } else if ((n == n->parent->left) && (n->parent == g->right)) {
      rotate_right_(n->parent);
      n = n->right;
    }
    insert_case5_(n);
  }

  void insert_case5_(Node *n) {
    Node *g = grandparent_(n);
    n->parent->red = false;
    g->red = true;
    if ((n == n->parent->left) && (n->parent == g->left)) {
      rotate_right_(g);
    } else {
      rotate_left_(g);
    }
  }

  // -------------------------Member functions -------------------//
 public:
  set() : size_(0) {
    end_ = new Node();
    begin_ = end_;
  };

  set(std::initializer_list<value_type> const &items) : set() {
    for (auto it = items.begin(); it != items.end(); ++it) {
      insert(*it);
    }
  };

  set(const set &s) : set() {
    for (auto it = ((set &)s).begin(); it != ((set &)s).end(); ++it) {
      insert(*it);
    }
  };

  set(set &&s) {
    end_ = s.end_;
    size_ = s.size_;
    begin_ = s.begin_;
    s.end_ = nullptr;
    s.begin_ = nullptr;
    s.size_ = 0;
    // return *this;  // ????
  };  // проверить

  ~set() {
    clear();
    delete end_;
    // begin_ = nullptr;      // end_
  };

  set &operator=(set &&s) {
    if (s.end_ != end_) {
      this->~set();
      begin_ = s.begin_;
      end_ = s.end_;
      size_ = s.size_;
      begin_ = nullptr;
      end_ = nullptr;
      size_ = 0;
    }
    return *(this);
  };

  //-------------------------Set Capacity-----------------------//
 public:
  bool empty() {  // maybe size = 0??//
    return begin() == end();
  }

  size_type size() const { return size_; }

  size_type max_size() {
    return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
  }

  // --------------- Iterator ---------------------//
  iterator begin() {  // no execept???
    if (size_ == 0) {
      return set_iterator(end_);
    } else {
      return set_iterator(begin_);  // ???
    }                               // need to check???
  }

  iterator end() {  // no execpt???//
    return set_iterator(end_);
  }

  // ---------------------- Set Modifiers -----------------//

  void clear() {       // xz
    while (size_ > 0)  // подумать как чистить
    {
      erase(begin());
    }
  }

  iterator find_parent_for_insert(const value_type &value, Node *node) {
    if (size_ == 0) {
      return begin();
    } else {
      Node *tmp = node->left;
      // int i = 0;
      while (tmp != nullptr && (tmp != end_)) {
        if (value < tmp->key) {
          if (tmp->left == nullptr)
            return iterator(tmp);
          else
            tmp = tmp->left;
        } else if (value > tmp->key) {
          if (tmp->right == nullptr || tmp->right == end_)
            return iterator(tmp);
          else
            tmp = tmp->right;
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

  std::pair<iterator, bool> insert(const value_type &value) {
    iterator it = find_parent_for_insert(value, end_);
    if (it.pos_ == end_ && size_ != 0) {  // это добавил работает
      // std::cout << "fuck";
      return std::make_pair(it, false);
    }
    Node *meaning = new Node(value);
    meaning->parent = it.pos_;
    // std::cout << "fucck";
    // проверка на пруф по поиску значений в этой хуете
    if (size_ == 0) {
      meaning->red = false;
      begin_ = meaning;
      end_->parent = meaning;
      end_->left = meaning;
      meaning->right = end_;
      meaning->parent = end_;  // для удаление перепривязка
    } else {
      // std::cout << it.pos_->key << " pos it \n";
      if (value > it.pos_->key) {
        it.pos_->right = meaning;
        insert_case2_(meaning);
      } else {
        it.pos_->left = meaning;
        insert_case2_(meaning);
      }
      if (value < begin_->key) {
        begin_ = meaning;
      }
      if (value > end_->parent->key) {
        end_->parent = meaning;
        meaning->right = end_;
      }
    }
    ++size_;
    return std::make_pair(it, true);
  }

  void erase(iterator pos) {
    if (size_ == 1) {
      end_->left = nullptr;
      end_->parent = nullptr;
      begin_ = end_;
    } else {
      if (pos.pos_ != end_) {
        if (pos.pos_->left != nullptr &&
            (pos.pos_->right != nullptr && pos.pos_->right != end_)) {
          delete_node_with_two_childs(pos.pos_);
        } else {
          delete_one_child(pos.pos_);
        }
      }
    }
    --size_;
    delete pos.pos_;
  }

  void delete_node_with_two_childs(Node *node) {
    iterator it = find_the_biggest_in_left(node);
    node->key = it.pos_->key;
    delete_one_child(it.pos_);
  }

  iterator find_the_biggest_in_left(Node *n) {
    Node *tmp = n->left;
    while (tmp->right != nullptr && tmp->right != end_) {
      tmp = tmp->right;
    }
    return iterator(tmp);
  }

  void delete_one_child(Node *n) {
    if (n == begin_) begin_ = (++begin()).pos_;
    if (n == end_->parent) {
      end_->parent = (--(--end())).pos_;
      (--(--end())).pos_->right = end_;
    }
    Node *child =
        (n->right == nullptr || n->right == end_) ? n->left : n->right;
    if (child != nullptr || n->red != 0) {
      replace_node(n, child);
    }
    if (n->red == 0) {
      if (child != nullptr && (child->red == 1)) {
        child->red = 0;
      } else {
        if (child != nullptr) {  // случай для черного узла с одним сыном узлом
          delete_case1(child);
        } else {  // случай для черного узла с сыновьями нуллптрами
          delete_case1(n);
          replace_node(n, child);
        }
      }
    }
  }

  void swap(set &other) {
    // std::swap(other., *this);
    Node *tmp = end_;
    end_ = other.end_;
    other.end_ = tmp;
    tmp = begin_;
    begin_ = other.begin_;
    other.begin_ = tmp;
    int tmp_size_ = size_;
    size_ = other.size_;
    other.size_ = tmp_size_;
  }

  // template <class Key>
  template <class... Keys>
  std::vector<std::pair<typename s21::set<Key>::iterator, bool>> insert_many(
      Keys &&...args) {
    std::vector<std::pair<iterator, bool>> ret_vect;
    for (const value_type key : {value_type(args)...}) {
      ret_vect.push_back(insert(key));
    }
    return ret_vect;
  }

  void merge(set &other) {
    for (iterator it = ((set &)other).begin(); it != ((set &)other).end();) {
      if (searchkey(end_, it.pos_->key) != end_) {
        // std::cout << it.pos_->key;
        insert(it.pos_->key);
        other.erase(it);
        it = ((set &)other).begin();
      } else {
        ++it;
      }
    }
    // базовая хуйня вопросов много особенно как он меняет значения сам ли или
    // заводить вайл и нужно ли чистить другое
  }

  // ----------------------------------  Set Lookup -------------------------//
  iterator find(const Key &key) {
    Node tmp = end_->left;
    if (key > tmp.key) {
      while (tmp.left != nullptr || tmp.right != nullptr) {
        if (key == tmp.key) {
          break;
        }
        if (tmp.key < key) {
          tmp = tmp->right;
        }
        if (tmp.key > key) {
          tmp = tmp->right;
        }
      }
    }
    if (tmp.key == key) {
      return set_iterator(tmp);  // проверить
    } else {
      return end();
    }
  };

  bool contains(const Key &key) {  // проверить нада
    iterator it = searchkey(end_, key);
    return (it.pos_ != end_);
  }

 private:
  iterator searchkey(Node *node,
                     const Key &key) {  // проверить плиз меняю на итератор
    Node *tmp = node;
    if (tmp == end_) {
      tmp = tmp->left;
    } else if (tmp->key > key) {
      tmp = tmp->left;
    } else if (tmp->key < key) {
      tmp = tmp->right;
    }
    while (tmp != nullptr && (tmp != end_)) {
      if (tmp->key == key) {
        return iterator(tmp);
      } else if (tmp->key > key) {
        tmp = tmp->left;
      } else {
        tmp = tmp->right;
      }
    }
    return end();  // было тмп
  }

  class Node {
   public:
    Node *left;
    Node *right;
    Node *parent;
    bool red;
    key_type key;

   public:
    Node() {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      red = true;
    };
    Node(value_type value) {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      red = true;
      key = value;
    };
    Node(Node &n) {
      this->key = n.key;
      this->left = n.left;
      this->parent = n.parent;
      this->red = n.red;
      this->right = n.right;
    };
    Node(Node &&n) {
      if (*this != n) {
        this->clear();
        this->left = n.left;
        this->right = n.right;
        this->parent = n.parent;
        this->red = n.red;
        this->key = n.key;
        n.begin_ = nullptr;
        n.end_ = nullptr;
        n.size_ = 0;
        // return *this;
      }
    };

    ~Node() {
      parent = nullptr;
      left = nullptr;
      right = nullptr;
    }

    bool operator==(const Node &n) {
      return (this->key = n.key, this->left = n.left, this->parent = n.parent,
              this->red = n.red, this->right = n.right);
    }

    bool operator!=(const Node &n) {
      return !(this->key != n.key, this->left != n.left,
               this->parent != n.parent, this->red != n.red,
               this->right != n.right);
    }
  };

  class set_iterator {
   public:
    Node *pos_;

   public:
    set_iterator() : pos_(nullptr) {}

    set_iterator(Node *pos) : pos_(pos) {}

    set_iterator(const set_iterator &other) : pos_(other.pos_) {}

    set_iterator &operator=(const set_iterator &other) {
      if (this != &other) {
        pos_ = other.pos_;
      }

      return *this;
    }

    set_iterator operator++() {  // не работает
      if (pos_->right == nullptr || pos_->right == pos_->parent) {
        if (pos_->parent->right == pos_) {  // тут падаем в сегу
          Node *tmp = pos_->parent;
          while (tmp->key < pos_->key) {
            tmp = tmp->parent;
          }
          pos_ = tmp;
        } else {
          pos_ = pos_->parent;
        }
      } else if (pos_->right->left != nullptr &&
                 (pos_->right->left->key < pos_->key)) {
        pos_ = pos_->right;
      } else {
        Node *tmp = pos_->right;
        while (tmp->left != nullptr) {
          tmp = tmp->left;
        }
        pos_ = tmp;
      }
      return pos_;
    }

    bool operator==(const set_iterator &other) { return (pos_ == other.pos_); }
    bool operator!=(const set_iterator &other) { return !(pos_ == other.pos_); }

    reference operator*() { return pos_->key; }

    set_iterator operator--() {
      Node *tmp;
      if (pos_->left == nullptr) {
        if (pos_->parent->left == pos_) {
          pos_ = pos_->parent->parent;
        } else {
          pos_ = pos_->parent;
        }
      } else {
        tmp = pos_->left;
        while (tmp->right != nullptr) {
          tmp = tmp->right;
        }
        pos_ = tmp;
      }
      return pos_;
    }
  };
};
}  // namespace s21

#endif
