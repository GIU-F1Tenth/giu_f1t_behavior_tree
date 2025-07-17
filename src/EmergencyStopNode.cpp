/**
 * @file EmergencyStopNode.cpp
 * @brief Implementation of EmergencyStopNode
 */

#include "EmergencyStopNode.hpp"

EmergencyStopNode::EmergencyStopNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.emergency_ackermann_cmd", "/ackermann_cmd");
    node_->declare_parameter("queue_sizes.publisher", 10);
    node_->declare_parameter("safety.emergency_stop.speed", 0.0);
    node_->declare_parameter("safety.emergency_stop.steering_angle", 0.0);
    node_->declare_parameter("safety.emergency_stop.acceleration", 0.0);
    node_->declare_parameter("safety.emergency_stop.jerk", 0.0);
    node_->declare_parameter("safety.emergency_stop.steering_angle_velocity", 0.0);

    node_->get_parameter("topics.emergency_ackermann_cmd", emergency_cmd_topic_);
    node_->get_parameter("queue_sizes.publisher", queue_size_);
    node_->get_parameter("safety.emergency_stop.speed", emergency_speed_);
    node_->get_parameter("safety.emergency_stop.steering_angle", emergency_steering_angle_);
    node_->get_parameter("safety.emergency_stop.acceleration", emergency_acceleration_);
    node_->get_parameter("safety.emergency_stop.jerk", emergency_jerk_);
    node_->get_parameter("safety.emergency_stop.steering_angle_velocity", emergency_steering_velocity_);

    // Create publisher
    cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(
        emergency_cmd_topic_, queue_size_);

    RCLCPP_INFO(node_->get_logger(), "EmergencyStopNode initialized");
}

BT::NodeStatus EmergencyStopNode::tick()
{
    // Create stop command using configurable parameters
    ackermann_msgs::msg::AckermannDriveStamped stop_cmd;
    stop_cmd.header.stamp = node_->now();
    stop_cmd.header.frame_id = "base_link";

    // Use configurable emergency stop values
    stop_cmd.drive.speed = emergency_speed_;
    stop_cmd.drive.steering_angle = emergency_steering_angle_;
    stop_cmd.drive.acceleration = emergency_acceleration_;
    stop_cmd.drive.jerk = emergency_jerk_;
    stop_cmd.drive.steering_angle_velocity = emergency_steering_velocity_;

    cmd_pub_->publish(stop_cmd);

    RCLCPP_WARN(node_->get_logger(), "Emergency stop command published");

    return BT::NodeStatus::SUCCESS;
}
