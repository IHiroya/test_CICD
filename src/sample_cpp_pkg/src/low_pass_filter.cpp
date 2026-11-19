#include "sample_cpp_pkg/low_pass_filter.hpp"

#include <stdexcept>

namespace sample_cpp_pkg
{

LowPassFilter::LowPassFilter(double alpha)
: alpha_(alpha)
{
  if (!(alpha_ > 0.0 && alpha_ <= 1.0)) {
    throw std::invalid_argument("alpha must be in (0, 1]");
  }
}

double LowPassFilter::update(double value)
{
  if (!initialized_) {
    value_ = value;
    initialized_ = true;
  } else {
    value_ = alpha_ * value + (1.0 - alpha_) * value_;
  }
  return value_;
}

double LowPassFilter::value() const
{
  return value_;
}

bool LowPassFilter::initialized() const
{
  return initialized_;
}

void LowPassFilter::reset()
{
  value_ = 0.0;
  initialized_ = false;
}

}  // namespace sample_cpp_pkg
