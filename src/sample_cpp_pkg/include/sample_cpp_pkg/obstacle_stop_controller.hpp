#ifndef SAMPLE_CPP_PKG__OBSTACLE_STOP_CONTROLLER_HPP_
#define SAMPLE_CPP_PKG__OBSTACLE_STOP_CONTROLLER_HPP_

#include <optional>

namespace sample_cpp_pkg
{

// 距離センサのインターフェース(実機ではハードウェアやトピックから値を読む)
class DistanceSensor
{
public:
  virtual ~DistanceSensor() = default;
  // 前方の障害物までの距離 [m]。読み取りに失敗した場合は std::nullopt
  virtual std::optional<double> read_distance() = 0;
};

// モータドライバのインターフェース(実機ではモータに速度指令を送る)
class MotorDriver
{
public:
  virtual ~MotorDriver() = default;
  virtual void set_velocity(double velocity) = 0;
};

// 障害物が近いとき、またはセンサが読めないときにモータを停止させる制御クラス
// センサとモータはインターフェース経由で受け取るので、テストではモックに差し替えられる
class ObstacleStopController
{
public:
  // stop_distance 以下の距離に障害物があれば停止する
  ObstacleStopController(DistanceSensor & sensor, MotorDriver & motor, double stop_distance);

  // センサを読み、target_velocity で走行するか停止するかを決めてモータに指令する
  // 停止した場合は true を返す
  bool update(double target_velocity);

private:
  DistanceSensor & sensor_;
  MotorDriver & motor_;
  double stop_distance_;
};

}  // namespace sample_cpp_pkg

#endif  // SAMPLE_CPP_PKG__OBSTACLE_STOP_CONTROLLER_HPP_
