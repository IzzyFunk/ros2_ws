import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class BasicSubscriber(Node):
    def __init__(self):
        super().__init__('basic_subscriber')
        # Subscribe to the 'chatter' topic with a queue size of 10
        self.subscription = self.create_subscription(
            String,
            'chatter',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        self.get_logger().info(f'I heard: "{msg.data}"')

def main(args=None):
    rclpy.init(args=args)
    basic_subscriber = BasicSubscriber()
    
    # Keep listening until interrupted
    rclpy.spin(basic_subscriber)
    
    # Clean up
    basic_subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()