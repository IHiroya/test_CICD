#include <gtest/gtest.h>

#include <stdexcept>

#include "sample_cpp_pkg/low_pass_filter.hpp"

using sample_cpp_pkg::LowPassFilter;

// 最初の入力前は未初期化で、出力値は 0 であること
TEST(LowPassFilterTest, NotInitializedBeforeFirstUpdate)
{
  LowPassFilter lpf(0.5);
  EXPECT_FALSE(lpf.initialized());
  EXPECT_DOUBLE_EQ(lpf.value(), 0.0);
}

// 最初の入力は平滑化せずにそのまま出力し、初期化済みになること
TEST(LowPassFilterTest, FirstUpdateReturnsInput)
{
  LowPassFilter lpf(0.5);
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 10.0);
  EXPECT_TRUE(lpf.initialized());
}

// 2回目以降は y = alpha * x + (1 - alpha) * y_prev の式どおりに平滑化されること
TEST(LowPassFilterTest, SmoothsSubsequentValues)
{
  LowPassFilter lpf(0.5);
  lpf.update(0.0);
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 5.0);   // 0.5 * 10 + 0.5 * 0
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 7.5);   // 0.5 * 10 + 0.5 * 5
}

// alpha = 1 のときは平滑化されず、入力にそのまま追従すること
TEST(LowPassFilterTest, AlphaOneFollowsInputExactly)
{
  LowPassFilter lpf(1.0);
  lpf.update(1.0);
  EXPECT_DOUBLE_EQ(lpf.update(42.0), 42.0);
}

// reset() で未初期化に戻り、次の入力がそのまま出力されること
TEST(LowPassFilterTest, ResetClearsState)
{
  LowPassFilter lpf(0.5);
  lpf.update(10.0);
  lpf.reset();
  EXPECT_FALSE(lpf.initialized());
  EXPECT_DOUBLE_EQ(lpf.update(4.0), 4.0);
}

// alpha が (0, 1] の範囲外なら std::invalid_argument が投げられること
TEST(LowPassFilterTest, InvalidAlphaThrows)
{
  EXPECT_THROW(LowPassFilter(0.0), std::invalid_argument);
  EXPECT_THROW(LowPassFilter(-0.1), std::invalid_argument);
  EXPECT_THROW(LowPassFilter(1.5), std::invalid_argument);
}
