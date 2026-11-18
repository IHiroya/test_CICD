"""/input (Float64) を購読し、範囲制限した値を /clamped (Float64) に配信するノード."""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64

from sample_py_pkg.clamp import clamp


class ClampNode(Node):

    def __init__(self):
        super().__init__('clamp_node')
        self.lower = self.declare_parameter('lower', -1.0).value
        self.upper = self.declare_parameter('upper', 1.0).value
        self.publisher = self.create_publisher(Float64, 'clamped', 10)
        self.subscription = self.create_subscription(Float64, 'input', self.on_input, 10)

    def on_input(self, msg: Float64):
        out = Float64()
        out.data = clamp(msg.data, self.lower, self.upper)
        self.publisher.publish(out)


def main(args=None):
    rclpy.init(args=args)
    node = ClampNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
