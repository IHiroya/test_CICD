#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "sample_cpp_pkg/moving_average.hpp"

// /input (Float64) を購読し、移動平均を /average (Float64) に配信するノード
class MovingAverageNode : public rclcpp::Node
{
public:
  MovingAverageNode()
  : Node("moving_average_node"),
    filter_(static_cast<std::size_t>(declare_parameter<int>("window_size", 5)))
  {
    publisher_ = create_publisher<std_msgs::msg::Float64>("average", 10);
    subscription_ = create_subscription<std_msgs::msg::Float64>(
      "input", 10,
      [this](const std_msgs::msg::Float64 & msg) {
        std_msgs::msg::Float64 out;
        out.data = filter_.add(msg.data);
        publisher_->publish(out);
      });
  }

private:
  sample_cpp_pkg::MovingAverage filter_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MovingAverageNode>());
  rclcpp::shutdown();
  return 0;
}
