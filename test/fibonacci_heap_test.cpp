//
// Created by runfme on 16.11.18.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testFibonacciHeap

#include "../src/FibonacciHeap/FibonacciHeap.hpp"
#include <gtest/gtest.h>

TEST(FibonacciHeapBasics, Constructor) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  EXPECT_EQ(heap.empty(), true);
}

TEST(FibonacciHeapBasics, Empty) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  EXPECT_EQ(heap.empty(), true);

  heap.insert(1);
  EXPECT_EQ(heap.empty(), false);
}

TEST(FibonacciHeapBasics, Insert) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  auto pointer = heap.insert(1);
  EXPECT_EQ(pointer->value(), 1);
}

TEST(FibonacciHeapBasics, GetMin) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  auto pointer = heap.insert(1);
  heap.insert(2);
  heap.insert(5);
  heap.insert(0);
  heap.insert(3);
  EXPECT_EQ(heap.min(), 0);
}

TEST(FibonacciHeapBasics, ExtractMin) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  heap.insert(2);
  heap.insert(3);
  heap.insert(5);
  heap.insert(10);
  heap.insert(3);
  heap.insert(11);
  EXPECT_EQ(heap.extract_min(), 2);
  EXPECT_EQ(heap.extract_min(), 3);
  EXPECT_EQ(heap.extract_min(), 3);
  heap.insert(1);
  EXPECT_EQ(heap.extract_min(), 1);
  EXPECT_EQ(heap.extract_min(), 5);
  EXPECT_EQ(heap.extract_min(), 10);
  EXPECT_EQ(heap.extract_min(), 11);

  EXPECT_EQ(heap.size(), 0);
}

TEST(FibonacciHeapBasics, Size) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
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

TEST(FibonacciHeapBasics, Delete) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  heap.remove(pointer_to_1);
  EXPECT_EQ(pointer_to_1->is_valid(), false);
  EXPECT_EQ(heap.size(), 0);

  heap.insert(3);
  auto pointer_to_0 = heap.insert(0);
  heap.insert(1);
  heap.remove(pointer_to_0);
  EXPECT_EQ(pointer_to_0->is_valid(), false);
  EXPECT_EQ(heap.min(), 1);
}

TEST(FibonacciHeapAdvanced, Change) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  heap.insert(1);
  heap.insert(3);
  auto pointer_to_2 = heap.insert(2);
  heap.insert(1);

  heap.decrease_key(pointer_to_2, 0);
  EXPECT_EQ(pointer_to_2->value(), 0);
  EXPECT_EQ(heap.min(), 0);

  heap.decrease_key(pointer_to_2, -1);
  EXPECT_EQ(pointer_to_2->value(), -1);
  EXPECT_EQ(heap.min(), -1);
}

TEST(FibonacciHeapAdvanced, MergeEmptyToNonEmpty) {
  FibonacciHeap<int> heap1 = FibonacciHeap<int>();
  FibonacciHeap<int> heap2 = FibonacciHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 1);
  EXPECT_EQ(heap1.size(), 2);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(FibonacciHeapAdvanced, MergeNonEmptyToEmpty) {
  FibonacciHeap<int> heap1 = FibonacciHeap<int>();
  FibonacciHeap<int> heap2 = FibonacciHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.merge(heap1);
  EXPECT_EQ(heap2.min(), 1);
  EXPECT_EQ(heap2.size(), 2);
  EXPECT_EQ(heap1.size(), 0);
}

TEST(FibonacciHeapAdvanced, MergeNonEmptyToNonEmpty) {
  FibonacciHeap<int> heap1 = FibonacciHeap<int>();
  FibonacciHeap<int> heap2 = FibonacciHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.insert(0);
  heap2.insert(4);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 0);
  EXPECT_EQ(heap1.size(), 4);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(FibonacciHeapExceptions, RequestsToEmptyHeap) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();

  EXPECT_THROW(heap.min(), std::runtime_error);
  EXPECT_THROW(heap.extract_min(), std::runtime_error);
}

TEST(FibonacciHeapExceptions, RequestsToInvalidatedPointer) {
  FibonacciHeap<int> heap = FibonacciHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  heap.remove(pointer_to_1);

  EXPECT_THROW(heap.remove(pointer_to_1), std::runtime_error);
  EXPECT_THROW(heap.decrease_key(pointer_to_1, 10), std::runtime_error);
}