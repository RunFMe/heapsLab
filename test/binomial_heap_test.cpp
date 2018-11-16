//
// Created by runfme on 16.11.18.
//


#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testBinomialHeap

#include "../src/BinomialHeap/BinomialHeap.hpp"
#include <gtest/gtest.h>

TEST(BinomialHeapBasics, Constructor) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  EXPECT_EQ(heap.empty(), true);
}

TEST(BinomialHeapBasics, Empty) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  EXPECT_EQ(heap.empty(), true);

  heap.insert(1);
  EXPECT_EQ(heap.empty(), false);
}

TEST(BinomialHeapBasics, Insert) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  auto pointer = heap.insert(1);
  EXPECT_EQ(pointer->value(), 1);
}

TEST(BinomialHeapBasics, GetMin) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  auto pointer = heap.insert(1);
  heap.insert(2);
  heap.insert(5);
  heap.insert(0);
  heap.insert(3);
  EXPECT_EQ(heap.min(), 0);
}

TEST(BinomialHeapBasics, ExtractMin) {
  BinomialHeap<int> heap = BinomialHeap<int>();
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
}

TEST(BinomialHeapBasics, Size) {
  BinomialHeap<int> heap = BinomialHeap<int>();
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

TEST(BinomialHeapBasics, Delete) {
  BinomialHeap<int> heap = BinomialHeap<int>();
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

TEST(BinomialHeapAdvanced, Change) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  heap.insert(1);
  heap.insert(3);
  auto pointer_to_2 = heap.insert(2);
  heap.insert(1);

  heap.change(pointer_to_2, 0);
  EXPECT_EQ(pointer_to_2->value(), 0);
  EXPECT_EQ(heap.min(), 0);

  heap.change(pointer_to_2, 100);
  EXPECT_EQ(pointer_to_2->value(), 100);
  EXPECT_EQ(heap.min(), 1);
}

TEST(BinomialHeapAdvanced, MergeEmptyToNonEmpty) {
  BinomialHeap<int> heap1 = BinomialHeap<int>();
  BinomialHeap<int> heap2 = BinomialHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 1);
  EXPECT_EQ(heap1.size(), 2);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(BinomialHeapAdvanced, MergeNonEmptyToEmpty) {
  BinomialHeap<int> heap1 = BinomialHeap<int>();
  BinomialHeap<int> heap2 = BinomialHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.merge(heap1);
  EXPECT_EQ(heap2.min(), 1);
  EXPECT_EQ(heap2.size(), 2);
  EXPECT_EQ(heap1.size(), 0);
}

TEST(BinomialHeapAdvanced, MergeNonEmptyToNonEmpty) {
  BinomialHeap<int> heap1 = BinomialHeap<int>();
  BinomialHeap<int> heap2 = BinomialHeap<int>();
  heap1.insert(1);
  heap1.insert(3);

  heap2.insert(0);
  heap2.insert(4);

  heap1.merge(heap2);
  EXPECT_EQ(heap1.min(), 0);
  EXPECT_EQ(heap1.size(), 4);
  EXPECT_EQ(heap2.size(), 0);
}

TEST(BinomialHeapExceptions, RequestsToEmptyHeap) {
  BinomialHeap<int> heap = BinomialHeap<int>();

  EXPECT_THROW(heap.min(), std::runtime_error);
  EXPECT_THROW(heap.extract_min(), std::runtime_error);
}

TEST(BinomialHeapExceptions, RequestsToInvalidatedPointer) {
  BinomialHeap<int> heap = BinomialHeap<int>();
  auto pointer_to_1 = heap.insert(1);
  heap.remove(pointer_to_1);

  EXPECT_THROW(heap.remove(pointer_to_1), std::runtime_error);
  EXPECT_THROW(heap.change(pointer_to_1, 10), std::runtime_error);
}

TEST(BinomialHeapExceptions, Stress) {
  BinomialHeap<int> heap = BinomialHeap<int>();
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
