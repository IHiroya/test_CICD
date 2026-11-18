#include "sample_cpp_pkg/moving_average.hpp"

#include <stdexcept>

namespace sample_cpp_pkg
{

MovingAverage::MovingAverage(std::size_t window_size)
: window_size_(window_size)
{
  if (window_size_ == 0) {
    throw std::invalid_argument("window_size must be greater than 0");
  }
}

double MovingAverage::add(double value)
{
  values_.push_back(value);
  sum_ += value;
  if (values_.size() > window_size_) {
    sum_ -= values_.front();
    values_.pop_front();
  }
  return average();
}

double MovingAverage::average() const
{
  if (values_.empty()) {
    return 0.0;
  }
  return sum_ / static_cast<double>(values_.size());
}

std::size_t MovingAverage::size() const
{
  return values_.size();
}

void MovingAverage::reset()
{
  values_.clear();
  sum_ = 0.0;
}

}  // namespace sample_cpp_pkg
