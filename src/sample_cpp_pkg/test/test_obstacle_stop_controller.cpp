#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>

#include "sample_cpp_pkg/obstacle_stop_controller.hpp"

using sample_cpp_pkg::DistanceSensor;
using sample_cpp_pkg::MotorDriver;
using sample_cpp_pkg::ObstacleStopController;
using ::testing::DoubleEq;
using ::testing::Return;

// 本物のセンサの代わりに、テストで返す値を指定できるモック
class MockDistanceSensor : public DistanceSensor
{
public:
  MOCK_METHOD(std::optional<double>, read_distance, (), (override));
};

// 本物のモータの代わりに、どんな指令を受け取ったかを確認できるモック
class MockMotorDriver : public MotorDriver
{
public:
  MOCK_METHOD(void, set_velocity, (double velocity), (override));
};

// 各テストで共通に使うモックと制御クラス(停止距離 0.5m)を用意する
class ObstacleStopControllerTest : public ::testing::Test
{
protected:
  MockDistanceSensor sensor_;
  MockMotorDriver motor_;
  ObstacleStopController controller_{sensor_, motor_, 0.5};
};

// 障害物が停止距離より遠いとき、目標速度でモータに指令し、停止しないこと
TEST_F(ObstacleStopControllerTest, DrivesWhenObstacleIsFar)
{
  EXPECT_CALL(sensor_, read_distance()).WillOnce(Return(2.0));
  EXPECT_CALL(motor_, set_velocity(DoubleEq(1.0))).Times(1);

  EXPECT_FALSE(controller_.update(1.0));
}

// 障害物が停止距離より近いとき、モータに速度 0(停止)を指令すること
TEST_F(ObstacleStopControllerTest, StopsWhenObstacleIsNear)
{
  EXPECT_CALL(sensor_, read_distance()).WillOnce(Return(0.3));
  EXPECT_CALL(motor_, set_velocity(DoubleEq(0.0))).Times(1);

  EXPECT_TRUE(controller_.update(1.0));
}

// 障害物が停止距離ちょうどのときも停止すること(境界値)
TEST_F(ObstacleStopControllerTest, StopsAtExactlyStopDistance)
{
  EXPECT_CALL(sensor_, read_distance()).WillOnce(Return(0.5));
  EXPECT_CALL(motor_, set_velocity(DoubleEq(0.0))).Times(1);

  EXPECT_TRUE(controller_.update(1.0));
}

// センサの読み取りに失敗したとき、安全のため停止すること
TEST_F(ObstacleStopControllerTest, StopsWhenSensorFails)
{
  EXPECT_CALL(sensor_, read_distance()).WillOnce(Return(std::nullopt));
  EXPECT_CALL(motor_, set_velocity(DoubleEq(0.0))).Times(1);

  EXPECT_TRUE(controller_.update(1.0));
}

// 障害物で停止した後、障害物がなくなれば走行を再開すること
TEST_F(ObstacleStopControllerTest, ResumesAfterObstacleIsRemoved)
{
  // 1回目は近い、2回目は遠い値を返す
  EXPECT_CALL(sensor_, read_distance())
  .WillOnce(Return(0.3))
  .WillOnce(Return(2.0));
  {
    // 停止 → 再開の順に指令されることを確認する
    ::testing::InSequence seq;
    EXPECT_CALL(motor_, set_velocity(DoubleEq(0.0)));
    EXPECT_CALL(motor_, set_velocity(DoubleEq(1.0)));
  }

  EXPECT_TRUE(controller_.update(1.0));
  EXPECT_FALSE(controller_.update(1.0));
}
