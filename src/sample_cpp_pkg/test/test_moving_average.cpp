#include <gtest/gtest.h>

#include <stdexcept>

#include "sample_cpp_pkg/moving_average.hpp"

using sample_cpp_pkg::MovingAverage;

TEST(MovingAverageTest, EmptyAverageIsZero)
{
  MovingAverage ma(3);
  EXPECT_DOUBLE_EQ(ma.average(), 0.0);
  EXPECT_EQ(ma.size(), 0u);
}

TEST(MovingAverageTest, AverageBeforeWindowIsFull)
{
  MovingAverage ma(3);
  EXPECT_DOUBLE_EQ(ma.add(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ma.add(2.0), 1.5);
}

TEST(MovingAverageTest, OldValuesDropOutOfWindow)
{
  MovingAverage ma(3);
  ma.add(1.0);
  ma.add(2.0);
  ma.add(3.0);
  EXPECT_DOUBLE_EQ(ma.add(4.0), 3.0);  // (2 + 3 + 4) / 3
  EXPECT_EQ(ma.size(), 3u);
}

TEST(MovingAverageTest, ResetClearsValues)
{
  MovingAverage ma(2);
  ma.add(10.0);
  ma.reset();
  EXPECT_EQ(ma.size(), 0u);
  EXPECT_DOUBLE_EQ(ma.add(4.0), 4.0);
}

TEST(MovingAverageTest, ZeroWindowThrows)
{
  EXPECT_THROW(MovingAverage(0), std::invalid_argument);
}
