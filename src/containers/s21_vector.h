#ifndef S21_VECTOR
#define S21_VECTOR

#include <exception>
#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {

template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

 private:
  size_t size_, capacity_;
  T* arr;

  // -----------------------К-О-Н-С-Т-Р-У-К-Т-О-Р-Ы------------------------------------------------------------------------------//
 public:
  vector() : size_(0U), capacity_(0U), arr(nullptr) {}  // дефолтный конструктор
  vector(size_type n)
      : size_(n),
        capacity_(n),
        arr(n ? new T[n] : nullptr) {}  // параметризированный конструктор
  vector(
      std::initializer_list<T> const& items) {  // инициализируюший конструктор
    arr = new T[items.size()];
    int i = 0;
    for (auto it = items.begin(); it != items.end(); it++) {
      arr[i] = *it;
      i++;
    }
    size_ = items.size();
    capacity_ = items.size();
  }
  vector(const vector& v) : size_(v.size_), capacity_(v.capacity_) {
    arr = new T[size_];
    for (size_t i = 0; i < size_; ++i) {
      arr[i] = v.arr[i];
    }
  }  // копирующий конструктор
  vector(vector&& v)
      : size_(v.size_),
        capacity_(v.capacity_),
        arr(v.arr)  // move сonstructor
  {
    v.arr = nullptr;
    v.size_ = 0;
  }

  ~vector() { delete[] arr; }  // деструктор

  vector operator=(vector&& v) {
    this->~vector();  /// переставить?
    if (&v != this) {
      size_ = v.size_;
      capacity_ = v.capacity_;
      arr = v.arr;  /// работает без перегрузки?
      v.size_ = 0;
      v.capacity_ = 0;
      v.arr = nullptr;
    }
    return *this;
  }  // присвоение с move
  // -----------------------------------------------------------------------------------------------------------------------------//

  // --------------------------VECTOR-ELEMENT-ACCESS------------------------------------------------------------------------------//
  T& at(size_t i);  // доступ к элементу массива с проверкой границ
  T& operator[](size_t pos) { return arr[pos]; }  // перегрузка []
  const T& operator[](size_t pos) const { return &arr[pos]; }  // перегрузка []
  const T& front() const { return arr[0]; }  // доступ к первому элементу
  T& front() { return arr[0]; }  // доступ к первому элементу
  const T& back() const {
    return arr[size_ - 1];
  }  // доступ к последнему элементу
  T& back() { return arr[size_ - 1]; }  // доступ к последнему элементу
  const T* data() const { return arr; }  // доступ к массиву
  T* data() { return arr; }              // доступ к массиву
  // -----------------------------------------------------------------------------------------------------------------------------//

  // --------------------------VECTOR-CAPACITY------------------------------------------------------------------------------------//
  bool empty();
  size_t size() const noexcept { return size_; }  // distance begin end
  // size_t max_size()const noexcept{return
  // std::numeric_limits<std::size_t>::max()/sizeof(value_type);}
  size_t max_size() const noexcept { return (size_type)-1; }
  void reserve(size_t size);
  size_t capacity() const noexcept { return capacity_; }
  void shrink_to_fit();
  // -----------------------------------------------------------------------------------------------------------------------------//

  // ---------------------------I-T-E-R-A-T-O-R-S---------------------------------------------------------------------------------//
  iterator begin() noexcept { return arr; }
  iterator end() { return arr + size_; }
  // -----------------------------------------------------------------------------------------------------------------------------//

  // ---------------------------V-E-C-T-O-R---M-O-D-I-F-I-E-R-S-------------------------------------------------------------------//
  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  void erase(const_iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void swap(vector& other) noexcept;

  template <typename... Args>
  void insert_many_back(Args&&... args);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);
  // -----------------------------------------------------------------------------------------------------------------------------//
};

template <typename T>
bool vector<T>::empty() {
  if (begin() == end()) {  // begin() == end()
    return true;
  } else {
    return false;
  }
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if (capacity_ > size_) {
    capacity_ = size_;
  }
}

template <typename T>
T& vector<T>::at(size_t i) {
  if (i >= size_) {
    throw std::out_of_range("element is out of the boundaries of the array");
  }
  return arr[i];
}

template <typename T>
void vector<T>::push_back(const_reference x) {
  if (size_ == capacity_) {
    reserve(size_ * 2);
  }
  ++size_;
  // std::cout<<"\n size is "<<size_;
  // std::cout<<"\n capacity is "<<capacity_;
  arr[size_ - 1] = x;
  // std::cout<<"\n added"<<x;
}

// size_t size();

// void push_back(T x);

template <typename T>
void vector<T>::pop_back() {
  // arr[size_ - 1]->~T();
  size_--;
}

template <typename T>
void vector<T>::clear() noexcept {
  // for (int i = 0; i < size_; ++i){
  //     pop_back();
  // }
  size_ = 0;
}

template <typename T>
void vector<T>::erase(const_iterator pos) {
  for (size_t i = pos - begin(); i < size_ - 1; ++i) {
    T tmp = arr[i];
    arr[i] = arr[i + 1];
    arr[i + 1] =
        tmp;  // какой тип данных сможет сломать  swap и удаление последнего
  }
  pop_back();
}

template <typename T>
void vector<T>::swap(vector& other) noexcept {
  if (other.capacity_ > capacity_) {
    reserve(other.capacity_);
  } else if (other.capacity_ < capacity_) {
    other.reserve(capacity_);
  }
  T* tmp = arr;
  arr = other.arr;
  other.arr = tmp;
  size_t tmp_size = size_;
  size_ = other.size_;
  other.size_ = tmp_size;
  // for(i = 0; i < other.size_; ++i){
  //     T tmp = arr[i];
  //     arr[i] = other.arr[i];
  //     other.arr[i] = T tmp;
  // }
}

template <typename T>
T* vector<T>::insert(iterator pos, const_reference value) {
  // std::cout<<"\n ARR IS "<<*arr;
  // std::cout<<"\n SIZE IS "<<size_;
  // std::cout<<"\n CAPCITY IS "<<capacity_;
  // std::cout<<"\n SIZE IS "<<size_;
  int counter = 0;
  for (iterator it = begin(); it < pos; ++it) {
    ++counter;
  }
  if (size_ == capacity_) {
    reserve(size_ * 2);
  }
  // std::cout<<"\n SIZE IS "<<size_;
  //  std::cout<<"\n CAPCITY IS "<<capacity_;
  for (int i = size_; i > counter; --i) {
    // std::cout<<"\n i - arr "<< i - begin();
    //  std::cout<<"\n itterator is "<< arr[i];
    //  std::cout<<"\n before arr is ";
    //  for(size_t i = 0; i < size_ +1 ; ++i){
    //      std::cout<< arr[i]<<" ";
    //  }
    arr[i] = arr[i - 1];
    // arr[i] = arr[i - 1];
    // std::cout<<"\n after arr is ";
    // for(size_t i = 0; i < size_ +1 ; ++i){
    //     std::cout<< arr[i]<<" ";
    // }
    // std::cout<<"\n";
  }
  arr[counter] = value;
  ++size_;
  return pos;
}

template <typename T>
void vector<T>::reserve(size_t size) {
  if (size > capacity_) {
    T* buff = new T[size];
    for (size_t i = 0; i < size_; ++i) {
      buff[i] = arr[i];
    }
    delete[] arr;  // нужна ли?
    arr = buff;
    capacity_ = size;
    // std::cout<<"\n CAPCITY IS "<<capacity_;
  }
}

template <typename T>
template <typename... Args>
T* vector<T>::insert_many(const T* pos, Args&&... args) {
  int counter = 0;
  for (iterator it = begin(); it < pos; ++it) {
    ++counter;
  }
  int len = sizeof...(args);
  value_type array[len] = {args...};
  iterator it = arr + counter;
  // std::cout<<"\n it is "<<*it;
  for (int i = len - 1; i >= 0; --i) {
    it = arr + counter;
    // std::cout<<"\n it is "<<*it;
    insert(it, array[i]);
    // std::cout<<"\n i is "<< arr[i];
  }
  return it;
}

template <typename T>
template <typename... Args>
void vector<T>::insert_many_back(Args&&... args) {
  size_type len = sizeof...(args);
  value_type arr[len] = {args...};
  for (size_type i = 0; i < len; ++i) {
    push_back(arr[i]);
    //     std::cout<<"\n CAPCITY IS "<<capacity_;
    // std::cout<<"\n size is "<<size_;
  }
}
/// reserve без аллокатора
// void reserve(size_t new_capacity){ /// выделение памяти
//     if (new_capacity <= capacity_){
//         return;
//     }

//     T* newarr = reinterpret_cast<T*>(new char[new_capacity * sizeof(T)])
//     size_t index = 0;
//     try
//     {
//         for(; index < size_; ++index){
//             new(newarr + index) T(arr_[index])
//         }
//     }
//     catch(...)
//     {
//         for(size_t i = 0; i < index; ++i){
//             (newarr + i)->~T();
//         }
//         delete [] reinterpret_cast<char*> (newarr);
//         throw;
//     }
//     arr_ = newarr;
//     capacity_ = new_capacity

// }

// push back с аллокатором
// void push_back(const T& x){
//     if (size_ == capacity_){
//         reserve(capacity_ > 0 ? capacity_ * 2 : 1);
//     }
//     if (size_ < capacity_){
//         traits::construct(alloc, arr + sz++, x);
//         return;
//     } else{
//         T* new_arr = traits::allocate(alloc, capacity_ * 2);
//         for (int i = 0; i < capacity_; ++i){
//             traits::construct(alloc, new_arr + i, arr[i])
//         }
//         traits:construct(alloc, new_arr + capacity_, x);в
//         for (int i = 0; i < capacity_; ++i){
//              traits::destroy(alloc,arr + i)
//         }
//         traits:deallocate(arr, capacity_);
//         arr = new_arr;
//         capacity_*=2;//(cap<<=1)
//     }
// }

/// конструктор копирования///
// traits::select_on_container_copy_constraction(alloc)

// // перегрузка []
// T& operator[](size_t i){
//     return arr[i]
// }
// const T& operator[](size_t i)const{
//     return arr[i]
// }
}  // namespace s21

#endif

// https://en.cppreference.com/w/cpp/container/vector