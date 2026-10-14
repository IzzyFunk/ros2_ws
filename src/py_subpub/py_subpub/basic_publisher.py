import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class BasicPublisher(Node):
    def __init__(self):
        #Initialize the node with name 'BasicPublisher'
        super().__init__('basic_publisher')

        #Create a publisher on 'chatter' topic with a queue size of 10
        self.publisher_ = self.create_publisher(String, 'chatter', 10)

        #Create a timer that triggers the callback every 0.5 seconds
        timer_period = 0.5
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    def timer_callback(self):
        msg = String()
        msg.data = f'Publishing Standard Message: {self.i}'
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: "{msg.data}"')
        self.i += 1

def main(args=None):
    rclpy.init(args=args)
    basic_publisher = BasicPublisher

    #Keep node running until interrupted
    rclpy.spin(basic_publisher())

    #Clean up and shut down
    basic_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()