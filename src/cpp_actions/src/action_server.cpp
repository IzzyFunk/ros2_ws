#include <functional>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

class FibonacciActionServer : public rclcpp::Node {
public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

    explicit FibonacciActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
    : Node("fibonacci_action_server", options) {
        using namespace std::placeholders;

        // Create the action server and bind the three required callbacks
        this->action_server_ = rclcpp_action::create_server<Fibonacci>(
            this,
            "fibonacci",
            std::bind(&FibonacciActionServer::handle_goal, this, _1, _2),
            std::bind(&FibonacciActionServer::handle_cancel, this, _1),
            std::bind(&FibonacciActionServer::handle_accepted, this, _1));
    }

private:
    rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;

    // Callback 1: Accept or reject incoming goal requests
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const Fibonacci::Goal> goal) {
        RCLCPP_INFO(this->get_logger(), "Received goal request with order %d", goal->order);
        (void)uuid;
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    // Callback 2: Accept or reject cancellation requests
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
        RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
        (void)goal_handle;
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    // Callback 3: Spin up a new thread to execute the long-running task
    void handle_accepted(const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
        using namespace std::placeholders;
        // This needs to return quickly, so we spin up a new thread for execution
        std::thread{std::bind(&FibonacciActionServer::execute, this, _1), goal_handle}.detach();
    }

    // The actual long-running task logic
    void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle) {
        RCLCPP_INFO(this->get_logger(), "Executing goal");
        rclcpp::Rate loop_rate(1);
        const auto goal = goal_handle->get_goal();
        auto feedback = std::make_shared<Fibonacci::Feedback>();
        auto & sequence = feedback->sequence;
        sequence.push_back(0);
        sequence.push_back(1);
        auto result = std::make_shared<Fibonacci::Result>();

        for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i) {
            // Check if there is a cancel request
            if (goal_handle->is_canceling()) {
                result->sequence = sequence;
                goal_handle->canceled(result);
                RCLCPP_INFO(this->get_logger(), "Goal canceled");
                return;
            }
            // Update sequence
            sequence.push_back(sequence[i] + sequence[i - 1]);
            
            // Publish feedback
            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "Publishing feedback");
            
            loop_rate.sleep();
        }

        // Check if goal is done
        if (rclcpp::ok()) {
            result->sequence = sequence;
            goal_handle->succeed(result);
            RCLCPP_INFO(this->get_logger(), "Goal succeeded");
        }
    }
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FibonacciActionServer>());
    rclcpp::shutdown();
    return 0;
}