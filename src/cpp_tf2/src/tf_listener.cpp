#include <chrono>
#include <memory>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.hpp>
#include <tf2_ros/transform_listener.hpp>

using namespace std::chrono_literals;

class FrameListener : public rclcpp::Node {
public:
    FrameListener() : Node("tf2_listener") {
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        timer_ = this->create_wall_timer(
            1s, std::bind(&FrameListener::on_timer, this));
    }

private:
    void on_timer() {
        try {
            // Calculate the spatial relationship between odom and camera_link
            geometry_msgs::msg::TransformStamped t = tf_buffer_->lookupTransform(
                "odom", "camera_link", tf2::TimePointZero);
                
            RCLCPP_INFO(this->get_logger(), "Camera Position relative to Odom -> X: %.2f, Y: %.2f, Z: %.2f",
                t.transform.translation.x,
                t.transform.translation.y,
                t.transform.translation.z);
        } catch (const tf2::TransformException & ex) {
            RCLCPP_WARN(this->get_logger(), "Could not transform: %s", ex.what());
        }
    }
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FrameListener>());
    rclcpp::shutdown();
    return 0;
}