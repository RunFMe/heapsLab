//
// Created by runfme on 16.11.18.
//


#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinaryHeap

#include "../src/BinaryHeap/BinaryHeap.hpp"
#include <gtest/gtest.h>

TEST(BinaryHeapBasics, Constructor) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  EXPECT_EQ(heap.empty(), true);
}

TEST(BinaryHeapBasics, Insert) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  auto pointer = heap.insert(1);
  EXPECT_EQ(pointer->value(), 1);
}

TEST(BinaryHeapBasics, Empty) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  EXPECT_EQ(heap.empty(), true);

  heap.insert(1);
  EXPECT_EQ(heap.empty(), false);
}

TEST(BinaryHeapBasics, GetMin) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  heap.insert(2);
  heap.insert(3);
  heap.insert(1);
  heap.insert(4);
  EXPECT_EQ(heap.min(), 1);
}

TEST(BinaryHeapBasics, ExtractMin) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  heap.insert(2);
  heap.insert(3);
  EXPECT_EQ(heap.extract_min(), 2);
  heap.insert(1);
  EXPECT_EQ(heap.extract_min(), 1);
}

TEST(BinaryHeapBasics, Size) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  EXPECT_EQ(heap.size(), 0);
  heap.insert(1);
  EXPECT_EQ(heap.size(), 1);
  heap.insert(2);
  heap.insert(6);
  heap.insert(1);
  EXPECT_EQ(heap.size(), 4);
  heap.extract_min();
  heap.extract_min();
  EXPECT_EQ(heap.size(), 2);
}

TEST(BinaryHeapAdvanced, Delete) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  EXPECT_EQ(heap.extract(pointer_to_1), 1);
  EXPECT_EQ(pointer_to_1->is_valid(), false);

  heap.insert(3);
  auto pointer_to_2 = heap.insert(2);
  heap.insert(1);
  EXPECT_EQ(heap.extract(pointer_to_2), 2);
  EXPECT_EQ(pointer_to_2->is_valid(), false);
}

TEST(BinaryHeapAdvanced, Change) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  heap.insert(1);
  heap.insert(3);
  auto pointer_to_2 = heap.insert(2);
  heap.insert(1);

  heap.change(pointer_to_2, 0);
  EXPECT_EQ(pointer_to_2->value(), 0);
  EXPECT_EQ(heap.min(), 0);
}

TEST(BinaryHeapAdvanced, IteratorConstructor) {
  int a[] = {4,2,5,1,6,3};
  BinaryHeap<int> heap = BinaryHeap<int>(a, a+6);
  EXPECT_EQ(heap.extract_min(), 1);
  EXPECT_EQ(heap.extract_min(), 2);
  EXPECT_EQ(heap.extract_min(), 3);
  EXPECT_EQ(heap.extract_min(), 4);
  EXPECT_EQ(heap.extract_min(), 5);
  EXPECT_EQ(heap.extract_min(), 6);
}

TEST(BinaryHeapAdvanced, EmptyIteratorConstructor) {
  int a[] = {4};
  BinaryHeap<int> heap = BinaryHeap<int>(a, a+0);
  EXPECT_EQ(heap.empty(), true);
}

TEST(BinaryHeapAdvanced, Optimize) {
  BinaryHeap<int> heap = BinaryHeap<int>();

  heap.optimize(1, 1);
  EXPECT_EQ(heap.get_tree_degree(), 4);

  heap.insert(2);
  heap.insert(3);
  heap.insert(3);
  heap.insert(63);
  heap.insert(-1);
  heap.insert(0);
  heap.insert(7);
  EXPECT_EQ(heap.extract_min(), -1);
  heap.insert(1);
  EXPECT_EQ(heap.extract_min(), 0);
}

TEST(BinaryHeapExceptions, RequestsToEmptyHeap) {
  BinaryHeap<int> heap = BinaryHeap<int>();

  EXPECT_THROW(heap.min(), std::runtime_error);
  EXPECT_THROW(heap.extract_min(), std::runtime_error);
}

TEST(BinaryHeapExceptions, RequestsToInvalidatedPointer) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  heap.extract(pointer_to_1);

  EXPECT_THROW(heap.extract(pointer_to_1), std::runtime_error);
  EXPECT_THROW(heap.change(pointer_to_1, 10), std::runtime_error);
}

TEST(BinaryHeapExceptions, RequestsToPointerFromAnotherHeap) {
  BinaryHeap<int> heap1 = BinaryHeap<int>();
  BinaryHeap<int> heap2 = BinaryHeap<int>();
  auto pointer_to_1 = heap1.insert(1);

  EXPECT_THROW(heap2.change(pointer_to_1, 10), std::runtime_error);
}


TEST(BinaryHeapExceptions, Stress) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  int size = 0;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 5000; ++j) {
      auto a = heap.insert(j);
      size++;
      EXPECT_EQ(heap.size(), size);
      heap.change(a, 2500);
      EXPECT_EQ(a->value(), 2500);
      heap.change(a, j);
      EXPECT_EQ(a->value(), j);
      heap.change(a, rand());
    }
    for (int k = 0; k < 2000; ++k) {
      heap.extract_min();

      size--;
      EXPECT_EQ(heap.size(), size);
    }
  }
}

TEST(BinaryHeapExceptions, OptimizeExceptions) {
  BinaryHeap<int> heap = BinaryHeap<int>();
  heap.insert(1);

  EXPECT_THROW(heap.optimize(1, 1), std::runtime_error);

  heap.extract_min();
  EXPECT_THROW(heap.optimize(10, 100), std::invalid_argument);
}