#include <memory>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.hpp>
#include <tf2_ros/static_transform_broadcaster.hpp>

class StaticFramePublisher : public rclcpp::Node {
public:
    explicit StaticFramePublisher() : Node("static_tf2_broadcaster") {
        tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        this->publish_static_transform();
    }

private:
    void publish_static_transform() {
        geometry_msgs::msg::TransformStamped t;
        
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "base_link";
        t.child_frame_id = "camera_link";

        // Translation (X, Y, Z in meters)
        t.transform.translation.x = 0.3;
        t.transform.translation.y = 0.0;
        t.transform.translation.z = 0.1;

        // Rotation (Roll, Pitch, Yaw in radians)
        tf2::Quaternion q;
        q.setRPY(0, 0, 0);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        tf_broadcaster_->sendTransform(t);
        RCLCPP_INFO(this->get_logger(), "Publishing static transform: base_link -> camera_link");
    }
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StaticFramePublisher>());
    rclcpp::shutdown();
    return 0;
}