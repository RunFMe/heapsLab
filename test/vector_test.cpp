//
// Created by runfme on 16.11.18.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE testVector

#include "../src/BinaryHeap/Vector.hpp"
#include <gtest/gtest.h>

TEST(VectorBasics, Constuctor) {
  Vector<int> array;
  EXPECT_EQ(array.empty(), true);
}

TEST(VectorBasics, PushBack) {
  Vector<int> array;
  array.push_back(1);
  EXPECT_EQ(array[0], 1);
  array.push_back(2);
  EXPECT_EQ(array[1], 2);
}

TEST(VectorBasics, ElementAcess) {
  Vector<int> array;
  array.push_back(1);
  EXPECT_EQ(array[0], 1);
  array[0] = 2;
  EXPECT_EQ(array[0], 2);
}

TEST(VectorBasics, Size) {
  Vector<int> array;
  EXPECT_EQ(array.size(), 0);
  array.push_back(1);
  EXPECT_EQ(array.size(), 1);
  array.push_back(2);
  array.push_back(3);
  array.push_back(4);
  EXPECT_EQ(array.size(), 4);
}

TEST(VectorBasics, PopBack) {
  Vector<int> array;
  array.push_back(1);
  array.push_back(2);
  array.push_back(3);
  array.push_back(4);
  array.pop_back();
  EXPECT_EQ(array.size(), 3);
}

TEST(VectorBasics, StorageResize) {
  Vector<int> array;
  EXPECT_EQ(array.get_storage_size(), 1);
  array.push_back(1);
  EXPECT_EQ(array.get_storage_size(), 2);
  array.push_back(2);
  array.push_back(3);
  array.push_back(4);
  EXPECT_EQ(array.get_storage_size(), 8);
  array.pop_back();
  array.pop_back();
  EXPECT_EQ(array.get_storage_size(), 4);
}

TEST(VectorBasics, OutOfRangeAccess) {
  Vector<int> array;
  array.push_back(1);
  EXPECT_THROW(array[10], std::out_of_range);
}