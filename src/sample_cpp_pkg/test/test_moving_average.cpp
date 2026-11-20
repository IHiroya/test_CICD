#include <gtest/gtest.h>

#include <stdexcept>

#include "sample_cpp_pkg/moving_average.hpp"

using sample_cpp_pkg::MovingAverage;

// 値を1つも追加していないとき、平均は 0、要素数も 0 であること
TEST(MovingAverageTest, EmptyAverageIsZero)
{
  MovingAverage ma(3);
  EXPECT_DOUBLE_EQ(ma.average(), 0.0);
  EXPECT_EQ(ma.size(), 0u);
}

// ウィンドウが埋まる前は、追加済みの値だけで平均を計算すること
TEST(MovingAverageTest, AverageBeforeWindowIsFull)
{
  MovingAverage ma(3);
  EXPECT_DOUBLE_EQ(ma.add(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ma.add(2.0), 1.5);
}

// ウィンドウサイズを超えたら、古い値から捨てて平均を計算すること
TEST(MovingAverageTest, OldValuesDropOutOfWindow)
{
  MovingAverage ma(3);
  ma.add(1.0);
  ma.add(2.0);
  ma.add(3.0);
  EXPECT_DOUBLE_EQ(ma.add(4.0), 3.0);  // (2 + 3 + 4) / 3
  EXPECT_EQ(ma.size(), 3u);
}

// reset() で値がすべて消え、その後は新しい値だけで平均を計算すること
TEST(MovingAverageTest, ResetClearsValues)
{
  MovingAverage ma(2);
  ma.add(10.0);
  ma.reset();
  EXPECT_EQ(ma.size(), 0u);
  EXPECT_DOUBLE_EQ(ma.add(4.0), 4.0);
}

// is_full() はウィンドウサイズ分の値がたまったら true、reset() 後は false になること
TEST(MovingAverageTest, IsFullAfterWindowSizeValues)
{
  MovingAverage ma(2);
  EXPECT_FALSE(ma.is_full());
  ma.add(1.0);
  EXPECT_FALSE(ma.is_full());
  ma.add(2.0);
  EXPECT_TRUE(ma.is_full());
  ma.add(3.0);
  EXPECT_TRUE(ma.is_full());
  ma.reset();
  EXPECT_FALSE(ma.is_full());
}

// ウィンドウサイズ 0 を指定すると std::invalid_argument が投げられること
TEST(MovingAverageTest, ZeroWindowThrows)
{
  EXPECT_THROW(MovingAverage(0), std::invalid_argument);
}
