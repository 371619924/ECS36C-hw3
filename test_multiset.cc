#include <gtest/gtest.h>

#include "multiset.h"

TEST(Multiset, Empty)
{
  Multiset<int> mset;

  /* Should be fully empty */
  EXPECT_EQ(mset.Empty(), true);
  EXPECT_EQ(mset.Size(), 0);
  EXPECT_THROW(mset.Count(42), std::exception);
}

TEST(Multiset, OneKey)
{
  Multiset<int> mset;

  /* Test some insertion */
  mset.Insert(23);
  mset.Insert(42);
  mset.Insert(42);
  EXPECT_EQ(mset.Empty(), false);
  EXPECT_EQ(mset.Size(), 3);
  EXPECT_EQ(mset.Min(), 23);
  EXPECT_EQ(mset.Max(), 42);
  EXPECT_EQ(mset.Count(42), 2);
}

TEST(Multiset, MultipleDuplicates)
{
  Multiset<int> mset;
  for (int i = 0; i < 10; ++i)
  {
    mset.Insert(5);
  }
  EXPECT_EQ(mset.Size(), 10);
  EXPECT_EQ(mset.Count(5), 10);
}

TEST(Multiset, RemoveDuplicate)
{
  Multiset<int> mset;
  mset.Insert(10);
  mset.Insert(10);
  EXPECT_EQ(mset.Count(10), 2);

  mset.Remove(10);
  EXPECT_EQ(mset.Count(10), 1);

  mset.Remove(10);

  EXPECT_THROW(mset.Count(10), std::exception);
  EXPECT_EQ(mset.Size(), 0);
}

TEST(Multiset, RemoveNonexistent)
{
  Multiset<int> mset;
  mset.Insert(20);
  EXPECT_THROW(mset.Remove(30), std::exception);
}

TEST(Multiset, Contains)
{
  Multiset<int> mset;
  mset.Insert(1);
  mset.Insert(3);
  mset.Insert(2);
  EXPECT_TRUE(mset.Contains(1));
  EXPECT_TRUE(mset.Contains(2));
  EXPECT_TRUE(mset.Contains(3));
  EXPECT_FALSE(mset.Contains(4));
}

TEST(Multiset, CountException)
{
  Multiset<int> mset;
  mset.Insert(100);
  EXPECT_NO_THROW(mset.Count(100));
  EXPECT_THROW(mset.Count(50), std::exception);
}

TEST(Multiset, FloorExact)
{
  Multiset<int> mset;
  mset.Insert(10);
  mset.Insert(20);
  mset.Insert(30);
  EXPECT_EQ(mset.Floor(20), 20);
}

TEST(Multiset, FloorNonExact)
{
  Multiset<int> mset;
  mset.Insert(10);
  mset.Insert(20);
  mset.Insert(30);

  EXPECT_EQ(mset.Floor(25), 20);

  EXPECT_THROW(mset.Floor(5), std::exception);
}

TEST(Multiset, CeilExact)
{
  Multiset<int> mset;
  mset.Insert(10);
  mset.Insert(20);
  mset.Insert(30);
  EXPECT_EQ(mset.Ceil(20), 20);
}

TEST(Multiset, CeilNonExact)
{
  Multiset<int> mset;
  mset.Insert(10);
  mset.Insert(20);
  mset.Insert(30);

  EXPECT_EQ(mset.Ceil(25), 30);

  EXPECT_THROW(mset.Ceil(35), std::exception);
}

TEST(Multiset, MinMax)
{
  Multiset<int> mset;
  mset.Insert(50);
  mset.Insert(20);
  mset.Insert(70);
  mset.Insert(40);

  EXPECT_EQ(mset.Min(), 20);
  EXPECT_EQ(mset.Max(), 70);


  Multiset<int> emptySet;
  EXPECT_THROW(emptySet.Min(), std::exception);
  EXPECT_THROW(emptySet.Max(), std::exception);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
