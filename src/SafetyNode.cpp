/**
 * @file SafetyNode.cpp
 * @brief Implementation of SafetyNode
 */

#include "SafetyNode.hpp"

SafetyNode::SafetyNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config), unsafe_condition_detected_(false)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.safety_node", "/tmp/safety");
    node_->declare_parameter("topics.ackermann_cmd", "/ackermann_cmd");
    node_->declare_parameter("queue_sizes.subscriber", 10);
    node_->declare_parameter("queue_sizes.publisher", 10);

    node_->get_parameter("topics.safety_node", safety_topic_);
    node_->get_parameter("topics.ackermann_cmd", cmd_topic_);
    node_->get_parameter("queue_sizes.subscriber", queue_size_);

    // Create subscriber and publisher
    safety_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        safety_topic_, queue_size_,
        std::bind(&SafetyNode::safetyCallback, this, std::placeholders::_1));

    cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(
        cmd_topic_, queue_size_);

    RCLCPP_INFO(node_->get_logger(), "SafetyNode initialized");
}

BT::NodeStatus SafetyNode::tick()
{
    if (unsafe_condition_detected_)
    {
        // Publish emergency stop command
        ackermann_msgs::msg::AckermannDriveStamped stop_cmd;
        stop_cmd.header.stamp = node_->now();
        stop_cmd.header.frame_id = "base_link";

        stop_cmd.drive.speed = 0.0;
        stop_cmd.drive.steering_angle = 0.0;
        stop_cmd.drive.acceleration = 0.0;
        stop_cmd.drive.jerk = 0.0;
        stop_cmd.drive.steering_angle_velocity = 0.0;

        cmd_pub_->publish(stop_cmd);

        RCLCPP_WARN(node_->get_logger(), "Unsafe condition detected - emergency stop engaged");
        return BT::NodeStatus::FAILURE;
    }

    RCLCPP_DEBUG(node_->get_logger(), "Safety check passed");
    return BT::NodeStatus::SUCCESS;
}

void SafetyNode::safetyCallback(const std_msgs::msg::Bool::SharedPtr msg)
{
    unsafe_condition_detected_ = msg->data;

    if (unsafe_condition_detected_)
    {
        RCLCPP_WARN(node_->get_logger(), "Safety violation detected");
    }
}
