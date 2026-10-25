#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class BasicPublisher : public rclcpp::Node {
public:
    BasicPublisher() : Node("basic_publisher"), count_(0) {

        // 1. Declare the parameter FIRST
        this->declare_parameter("custom_timer_rate", 1.0);

        // 2. Fetch the parameter value that was passed in by the launch file
        double timer_rate = this->get_parameter("custom_timer_rate").as_double();

        // 3. Initialize your publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
        
        // 4. Convert rate to ms and initialize timer using dynamic variable
        auto timer_ms = std::chrono::milliseconds(static_cast<int>(1000 / timer_rate));
        timer_ = this->create_wall_timer(
            timer_ms, std::bind(&BasicPublisher::timer_callback, this));
    }

private:
    void timer_callback() {
        auto message = std_msgs::msg::String();
        message.data = "Publishing standard message: " + std::to_string(count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: \"%s\"", message.data.c_str());
        publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BasicPublisher>());
    rclcpp::shutdown();
    return 0;
}