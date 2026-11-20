#include "sample_cpp_pkg/obstacle_stop_controller.hpp"

namespace sample_cpp_pkg
{

ObstacleStopController::ObstacleStopController(
  DistanceSensor & sensor, MotorDriver & motor, double stop_distance)
: sensor_(sensor), motor_(motor), stop_distance_(stop_distance)
{
}

bool ObstacleStopController::update(double target_velocity)
{
  const auto distance = sensor_.read_distance();

  // センサが読めない場合は安全のため停止する
  if (!distance || *distance <= stop_distance_) {
    motor_.set_velocity(0.0);
    return true;
  }

  motor_.set_velocity(target_velocity);
  return false;
}

}  // namespace sample_cpp_pkg
