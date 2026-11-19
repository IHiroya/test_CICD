#ifndef SAMPLE_CPP_PKG__MOVING_AVERAGE_HPP_
#define SAMPLE_CPP_PKG__MOVING_AVERAGE_HPP_

#include <cstddef>
#include <deque>

namespace sample_cpp_pkg
{

// 直近 window_size 個の値の平均を計算するクラス
class MovingAverage
{
public:
  // window_size が 0 の場合は std::invalid_argument を投げる
  explicit MovingAverage(std::size_t window_size);

  // 値を追加し、追加後の平均値を返す
  double add(double value);

  // 現在の平均値(値が1つもない場合は 0.0)
  double average() const;

  std::size_t size() const;

  // ウィンドウが値で埋まっているか
  bool is_full() const;

  void reset();

private:
  std::size_t window_size_;
  std::deque<double> values_;
  double sum_{0.0};
};

}  // namespace sample_cpp_pkg

#endif  // SAMPLE_CPP_PKG__MOVING_AVERAGE_HPP_
