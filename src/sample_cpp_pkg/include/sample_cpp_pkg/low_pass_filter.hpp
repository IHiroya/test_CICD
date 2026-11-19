#ifndef SAMPLE_CPP_PKG__LOW_PASS_FILTER_HPP_
#define SAMPLE_CPP_PKG__LOW_PASS_FILTER_HPP_

namespace sample_cpp_pkg
{

// 1次ローパスフィルタ(指数移動平均)
//   y[k] = alpha * x[k] + (1 - alpha) * y[k-1]
// alpha が大きいほど入力への追従が速く、小さいほど平滑化が強い
class LowPassFilter
{
public:
  // alpha が (0, 1] の範囲外の場合は std::invalid_argument を投げる
  explicit LowPassFilter(double alpha);

  // 値を入力し、フィルタ後の値を返す(最初の入力はそのまま出力する)
  double update(double value);

  // 現在の出力値(入力が1つもない場合は 0.0)
  double value() const;

  bool initialized() const;
  void reset();

private:
  double alpha_;
  double value_{0.0};
  bool initialized_{false};
};

}  // namespace sample_cpp_pkg

#endif  // SAMPLE_CPP_PKG__LOW_PASS_FILTER_HPP_
