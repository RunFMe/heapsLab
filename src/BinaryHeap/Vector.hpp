//
// Created by runfme on 13.11.18.
//
#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
template <class T>
class Vector {
 public:
  Vector() {
    storage_size = 1;
    size_ = 0;
    storage = new T[storage_size];
  }

  void push_back(T value) {
    storage[size_] = value;
    ++size_;
    adjust_storage();
  }

  void pop_back() {
    --size_;
    adjust_storage();
  }

  unsigned long size() {
    return size_;
  }

  bool empty() {
    return size_ == 0;
  }

  T& operator[](unsigned long index) {
    if (index >= size_) {
      throw std::out_of_range ("Index is out of range");
    }
    return storage[index];
  }

  unsigned long get_storage_size() {
    return storage_size;
  }

  ~Vector() {
    delete[] storage;
  }

 private:
  T* storage;
  unsigned long storage_size;
  unsigned long size_;

  void set_new_storage(unsigned long new_size) {
    T* new_storage = new T[storage_size * 2];
    for (int i = 0; i < size_; ++i) {
      new_storage[i] = storage[i];
    }
    delete[] storage;
    storage = new_storage;
    storage_size = new_size;
  }

  void adjust_storage() {
    if (size_ >= storage_size) {
      set_new_storage(storage_size * 2);
    } else if (size_ != 0 && size_ * 4 <= storage_size) {
      set_new_storage(storage_size / 2);
    }
  }
};

#endif