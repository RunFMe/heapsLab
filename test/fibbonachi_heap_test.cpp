//
// Created by runfme on 16.11.18.
//


#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testFibbonachiHeap

#include "../src/FibbonachiHeap/FibbonachiHeap.hpp"
#include <gtest/gtest.h>

TEST(FibbonachiHeapBasics, Constructor) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
  EXPECT_EQ(heap.empty(), true);
}

TEST(FibbonachiHeapBasics, Empty) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
  EXPECT_EQ(heap.empty(), true);

  heap.insert(1);
  EXPECT_EQ(heap.empty(), false);
}

TEST(FibbonachiHeapBasics, Insert) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
  auto pointer = heap.insert(1);
  EXPECT_EQ(pointer->value(), 1);
}

TEST(FibbonachiHeapBasics, GetMin) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
  auto pointer = heap.insert(1);
  heap.insert(2);
  heap.insert(5);
  heap.insert(0);
  heap.insert(3);
  EXPECT_EQ(heap.min(), 0);
}

TEST(FibbonachiHeapBasics, ExtractMin) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
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

TEST(FibbonachiHeapBasics, Size) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
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

TEST(FibbonachiHeapBasics, Delete) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
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

TEST(FibbonachiHeapAdvanced, Change) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
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

TEST(FibbonachiHeapAdvanced, MergeEmptyToNonEmpty) {
  FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
  FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 1);
  EXPECT_EQ(heap1.size(), 2);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(FibbonachiHeapAdvanced, MergeNonEmptyToEmpty) {
  FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
  FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.merge(heap1);
  EXPECT_EQ(heap2.min(), 1);
  EXPECT_EQ(heap2.size(), 2);
  EXPECT_EQ(heap1.size(), 0);
}

TEST(FibbonachiHeapAdvanced, MergeNonEmptyToNonEmpty) {
  FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
  FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.insert(0);
  heap2.insert(4);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 0);
  EXPECT_EQ(heap1.size(), 4);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(FibbonachiHeapExceptions, RequestsToEmptyHeap) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();

  EXPECT_THROW(heap.min(), std::runtime_error);
  EXPECT_THROW(heap.extract_min(), std::runtime_error);
}

TEST(FibbonachiHeapExceptions, RequestsToInvalidatedPointer) {
  FibbonachiHeap<int> heap = FibbonachiHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  heap.remove(pointer_to_1);

  EXPECT_THROW(heap.remove(pointer_to_1), std::runtime_error);
  EXPECT_THROW(heap.decrease_key(pointer_to_1, 10), std::runtime_error);
}
