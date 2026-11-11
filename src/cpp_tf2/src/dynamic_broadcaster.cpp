#include <chrono>
#include <memory>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.hpp>
#include <tf2_ros/transform_broadcaster.hpp>

using namespace std::chrono_literals;

class DynamicFramePublisher : public rclcpp::Node {
public:
    DynamicFramePublisher() : Node("dynamic_tf2_broadcaster"), x_pos_(0.0) {
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
        timer_ = this->create_wall_timer(
            100ms, std::bind(&DynamicFramePublisher::publish_dynamic_transform, this));
    }

private:
    void publish_dynamic_transform() {
        geometry_msgs::msg::TransformStamped t;
        
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "odom";
        t.child_frame_id = "base_link";

        // Simulate forward movement
        x_pos_ += 0.01;
        t.transform.translation.x = x_pos_;
        t.transform.translation.y = 0.0;
        t.transform.translation.z = 0.0;

        tf2::Quaternion q;
        q.setRPY(0, 0, 0);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        tf_broadcaster_->sendTransform(t);
    }
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
    double x_pos_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DynamicFramePublisher>());
    rclcpp::shutdown();
    return 0;
}