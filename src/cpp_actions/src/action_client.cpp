#include <functional>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

class FibonacciActionClient : public rclcpp::Node {
public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

    explicit FibonacciActionClient(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
    : Node("fibonacci_action_client", options) {
        this->client_ptr_ = rclcpp_action::create_client<Fibonacci>(this, "fibonacci");
        this->timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&FibonacciActionClient::send_goal, this));
    }

    void send_goal() {
        this->timer_->cancel();
        if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available");
            rclcpp::shutdown();
            return;
        }

        auto goal_msg = Fibonacci::Goal();
        goal_msg.order = 10;
        RCLCPP_INFO(this->get_logger(), "Sending goal");

        auto send_goal_options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
        send_goal_options.goal_response_callback = std::bind(&FibonacciActionClient::goal_response_callback, this, std::placeholders::_1);
        send_goal_options.feedback_callback = std::bind(&FibonacciActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);
        send_goal_options.result_callback = std::bind(&FibonacciActionClient::result_callback, this, std::placeholders::_1);
        this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

private:
    rclcpp_action::Client<Fibonacci>::SharedPtr client_ptr_;
    rclcpp::TimerBase::SharedPtr timer_;

    void goal_response_callback(const GoalHandleFibonacci::SharedPtr & goal_handle) {
        if (!goal_handle) { RCLCPP_ERROR(this->get_logger(), "Goal was rejected"); } 
        else { RCLCPP_INFO(this->get_logger(), "Goal accepted, waiting for result"); }
    }

    void feedback_callback(GoalHandleFibonacci::SharedPtr, const std::shared_ptr<const Fibonacci::Feedback> feedback) {
        RCLCPP_INFO(this->get_logger(), "Next number: %d", feedback->sequence.back());
    }

    void result_callback(const GoalHandleFibonacci::WrappedResult & result) {
        switch (result.code) {
            case rclcpp_action::ResultCode::SUCCEEDED: RCLCPP_INFO(this->get_logger(), "Goal succeeded"); break;
            case rclcpp_action::ResultCode::ABORTED: RCLCPP_ERROR(this->get_logger(), "Goal aborted"); return;
            case rclcpp_action::ResultCode::CANCELED: RCLCPP_ERROR(this->get_logger(), "Goal canceled"); return;
            default: RCLCPP_ERROR(this->get_logger(), "Unknown result code"); return;
        }
        rclcpp::shutdown();
    }
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FibonacciActionClient>());
    rclcpp::shutdown();
    return 0;
}