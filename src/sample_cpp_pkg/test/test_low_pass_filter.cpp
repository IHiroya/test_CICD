#include <gtest/gtest.h>

#include <stdexcept>

#include "sample_cpp_pkg/low_pass_filter.hpp"

using sample_cpp_pkg::LowPassFilter;

TEST(LowPassFilterTest, NotInitializedBeforeFirstUpdate)
{
  LowPassFilter lpf(0.5);
  EXPECT_FALSE(lpf.initialized());
  EXPECT_DOUBLE_EQ(lpf.value(), 0.0);
}

TEST(LowPassFilterTest, FirstUpdateReturnsInput)
{
  LowPassFilter lpf(0.5);
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 10.0);
  EXPECT_TRUE(lpf.initialized());
}

TEST(LowPassFilterTest, SmoothsSubsequentValues)
{
  LowPassFilter lpf(0.5);
  lpf.update(0.0);
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 5.0);   // 0.5 * 10 + 0.5 * 0
  EXPECT_DOUBLE_EQ(lpf.update(10.0), 7.5);   // 0.5 * 10 + 0.5 * 5
}

TEST(LowPassFilterTest, AlphaOneFollowsInputExactly)
{
  LowPassFilter lpf(1.0);
  lpf.update(1.0);
  EXPECT_DOUBLE_EQ(lpf.update(42.0), 42.0);
}

TEST(LowPassFilterTest, ResetClearsState)
{
  LowPassFilter lpf(0.5);
  lpf.update(10.0);
  lpf.reset();
  EXPECT_FALSE(lpf.initialized());
  EXPECT_DOUBLE_EQ(lpf.update(4.0), 4.0);
}

TEST(LowPassFilterTest, InvalidAlphaThrows)
{
  EXPECT_THROW(LowPassFilter(0.0), std::invalid_argument);
  EXPECT_THROW(LowPassFilter(-0.1), std::invalid_argument);
  EXPECT_THROW(LowPassFilter(1.5), std::invalid_argument);
}
