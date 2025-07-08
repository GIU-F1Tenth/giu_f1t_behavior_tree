/**
 * @file PurePursuitNode.cpp
 * @brief Implementation of PurePursuitNode
 */

#include "PurePursuitNode.hpp"

PurePursuitNode::PurePursuitNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config), command_received_(false)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.pure_pursuit_cmd", "/tmp/pp_ackermann_cmd");
    node_->declare_parameter("topics.ackermann_cmd", "/ackermann_cmd");
    node_->declare_parameter("queue_sizes.subscriber", 10);
    node_->declare_parameter("queue_sizes.publisher", 10);
    node_->declare_parameter("pure_pursuit.default_frame_id", "base_link");

    node_->get_parameter("topics.pure_pursuit_cmd", pure_pursuit_topic_);
    node_->get_parameter("topics.ackermann_cmd", cmd_topic_);
    node_->get_parameter("queue_sizes.subscriber", queue_size_);
    node_->get_parameter("pure_pursuit.default_frame_id", default_frame_id_);

    // Create subscriber and publisher
    control_sub_ = node_->create_subscription<ackermann_msgs::msg::AckermannDriveStamped>(
        pure_pursuit_topic_, queue_size_,
        std::bind(&PurePursuitNode::controlCallback, this, std::placeholders::_1));

    cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(
        cmd_topic_, queue_size_);

    // Initialize with safe command using configurable frame_id
    latest_command_.header.frame_id = default_frame_id_;
    latest_command_.drive.speed = 0.0;
    latest_command_.drive.steering_angle = 0.0;
    latest_command_.drive.acceleration = 0.0;
    latest_command_.drive.jerk = 0.0;
    latest_command_.drive.steering_angle_velocity = 0.0;

    RCLCPP_INFO(node_->get_logger(), "PurePursuitNode initialized");
}

BT::NodeStatus PurePursuitNode::tick()
{
    // Update timestamp
    latest_command_.header.stamp = node_->now();

    // Publish the latest control command
    cmd_pub_->publish(latest_command_);

    if (command_received_)
    {
        RCLCPP_DEBUG(node_->get_logger(), "Published pure pursuit command: speed=%.2f, steering=%.2f",
                     latest_command_.drive.speed, latest_command_.drive.steering_angle);
    }
    else
    {
        RCLCPP_DEBUG(node_->get_logger(), "Published safe stop command (no pure pursuit command received)");
    }

    return BT::NodeStatus::SUCCESS;
}

void PurePursuitNode::controlCallback(const ackermann_msgs::msg::AckermannDriveStamped::SharedPtr msg)
{
    latest_command_ = *msg;
    command_received_ = true;

    RCLCPP_DEBUG(node_->get_logger(), "Received pure pursuit command: speed=%.2f, steering=%.2f",
                 msg->drive.speed, msg->drive.steering_angle);
}
