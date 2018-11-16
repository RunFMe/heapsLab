//
// Created by not sashka on 19.10.18.
//

#ifndef BINARY_HEAP_BINARYHEAP_H
#define BINARY_HEAP_BINARYHEAP_H

#include <cassert>
#include <cstring>
#include <memory>
#include <exception>
#include <bits/shared_ptr.h>
#include "Vector.hpp"

template <typename T>
class BinaryHeap {
  class Pointer{
    friend class BinaryHeap<T>;
   public:
    Pointer(unsigned long index, BinaryHeap* parent_heap);

    T value();

    bool is_valid();

    BinaryHeap* get_parent_heap();

   private:
    BinaryHeap* parent_heap;
    unsigned long index;
    bool valid;
  };
 public:

  bool empty();

  size_t size();

  std::shared_ptr<Pointer> insert(T value);

  T min();

  T extract_min();

  T extract(std::shared_ptr<Pointer> pointer);

  void change(std::shared_ptr<Pointer> pointer, T new_value);

  BinaryHeap();

  template <class Iterator>
  BinaryHeap(Iterator begin, Iterator end);

  void optimize(size_t insert_count, size_t extract_count) {
    
  }

 private:
  struct Element {
    T value;
    std::shared_ptr<typename BinaryHeap<T>::Pointer> pointer;
  };

  unsigned long tree_degree = 2;
  Vector<Element> storage;

  void swap_elements(unsigned long first_index, unsigned long second_index);

  unsigned long parent(unsigned long index);

  unsigned long nth_child(unsigned long index, unsigned long n);

  void sift_up(unsigned long index);

  void sift_down(unsigned long index);

  std::shared_ptr<Pointer> push_back_element(T value);
};

template<typename T>
BinaryHeap<T>::BinaryHeap() = default;

template<typename T>
template<class Iterator>
BinaryHeap<T>::BinaryHeap(Iterator begin, Iterator end) {
  for (Iterator iter = begin; iter != end ; ++iter) {
    push_back_element(*iter);
  }

  //call sift_down for all non leaves
  for (unsigned long i = size()/2; i > 0; --i) {
    sift_down(i-1);
  }
}

template<typename T>
size_t BinaryHeap<T>::size() {
  return storage.size();
}

template<typename T>
bool BinaryHeap<T>::empty() {
  return storage.empty();
}

template<typename T>
std::shared_ptr<typename BinaryHeap<T>::Pointer> BinaryHeap<T>::insert(T value) {
  auto pointer = push_back_element(value);
  sift_up(pointer->index);
  return pointer;
}

template<typename T>
T BinaryHeap<T>::min() {
  if (empty()) {
    throw std::runtime_error ("No minimal element. Heap is empty.");
  }
  return storage[0].value;
}

template<typename T>
T BinaryHeap<T>::extract(std::shared_ptr<BinaryHeap::Pointer> pointer) {
  if (!pointer->is_valid()) {
    throw std::runtime_error ("Invalidated pointer to an element");
  }
  if (pointer->get_parent_heap() != this) {
    throw std::runtime_error ("Pointer from another heap");
  }

  unsigned long extract_index = pointer->index;
  T value = pointer->value();

  //try to swap extracted with last element and sift it down
  if (extract_index == size() - 1) {
    storage.pop_back();
  } else {
    swap_elements(extract_index, size()-1);
    storage.pop_back();
    sift_down(extract_index);
  }

  pointer->valid = false;
  return value;
}

template<typename T>
T BinaryHeap<T>::extract_min() {
  if (empty()) {
    throw std::runtime_error ("No minimal element. Heap is empty.");
  }
  return extract(storage[0].pointer);
}

template<typename T>
void BinaryHeap<T>::change(std::shared_ptr<BinaryHeap::Pointer> pointer, T new_value) {
  if (!pointer->is_valid()) {
    throw std::runtime_error ("Invalidated pointer to an element");
  }
  if (pointer->get_parent_heap() != this) {
    throw std::runtime_error ("Pointer from another heap");
  }

  T previous_value = pointer->value();
  storage[pointer->index].value = new_value;

  if (new_value > previous_value) {
    sift_down(pointer->index);
  } else {
    sift_up(pointer->index);
  }
}

template<typename T>
void BinaryHeap<T>::swap_elements(unsigned long first_index, unsigned long second_index) {
  if (first_index >= size() || second_index>= size()) {
    throw std::out_of_range ("At least one of indexes is out of range");
  }
  std::swap(storage[first_index].value, storage[second_index].value);
  std::swap(storage[first_index].pointer->index, storage[second_index].pointer->index);
  std::swap(storage[first_index].pointer, storage[second_index].pointer);
}

template<typename T>
std::shared_ptr<typename BinaryHeap<T>::Pointer> BinaryHeap<T>::push_back_element(T value) {
  unsigned long new_index = storage.size();
  std::shared_ptr<Pointer> pointer(new Pointer(new_index, this));
  storage.push_back({value, pointer});

  return pointer;
}

template<typename T>
void BinaryHeap<T>::sift_down(unsigned long index) {
  if (index >= size()) {
    throw std::out_of_range ("Index is out of range");
  }
  unsigned long min_index = index;

  for (unsigned long i = 0; i < tree_degree; ++i) {
    unsigned long child_index = nth_child(index, i);
    if (child_index < size() && storage[child_index].value < storage[min_index].value) {
      min_index = child_index;
    }
  }

  if (min_index != index) {
    swap_elements(min_index, index);
    sift_down(min_index);
  }
}

template<typename T>
void BinaryHeap<T>::sift_up(unsigned long index) {
  if (index >= size()) {
    throw std::out_of_range ("Index is out of range");
  }
  unsigned long current = index;

  while (current != 0) {
    unsigned long current_parent = parent(current);
    if (storage[current].value < storage[current_parent].value) {
      swap_elements(current, current_parent);
    }
    current = current_parent;
  }
}

template<typename T>
unsigned long BinaryHeap<T>::nth_child(unsigned long index, unsigned long n) {
  if (index >= size()) {
    throw std::out_of_range ("Index is out of range");
  }
  return index * tree_degree + n + 1;
}

template<typename T>
unsigned long BinaryHeap<T>::parent(unsigned long index) {
  if (index == 0) {
    throw std::runtime_error ("Root node has no parent");
  }
  if (index >= size()) {
    throw std::out_of_range ("Index is out of range");
  }
  return (index - 1) / tree_degree;
}

template<typename T>
bool BinaryHeap<T>::Pointer::is_valid() {
  return valid;
}

template<typename T>
T BinaryHeap<T>::Pointer::value() {
  return parent_heap->storage[index].value;
}

template<typename T>
BinaryHeap<T>::Pointer::Pointer(unsigned long index, BinaryHeap* const parent_heap) {
  this->index = index;
  this->parent_heap = parent_heap;
  this->valid = true;
}

template<typename T>
BinaryHeap<T>* BinaryHeap<T>::Pointer::get_parent_heap() {
  return parent_heap;
}

#endif //BINARY_HEAP_BINARYHEAP_H
