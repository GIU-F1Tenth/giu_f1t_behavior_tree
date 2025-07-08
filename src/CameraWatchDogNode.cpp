/**
 * @file CameraWatchDogNode.cpp
 * @brief Implementation of CameraWatchDogNode
 */

#include "CameraWatchDogNode.hpp"

CameraWatchDogNode::CameraWatchDogNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config), camera_timeout_detected_(true)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.camera_watchdog", "/tmp/watchdog/camera_is_live");
    node_->declare_parameter("queue_sizes.subscriber", 10);

    node_->get_parameter("topics.camera_watchdog", camera_watchdog_topic_);
    node_->get_parameter("queue_sizes.subscriber", queue_size_);

    // Create subscriber
    watchdog_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        camera_watchdog_topic_, queue_size_,
        std::bind(&CameraWatchDogNode::watchdogCallback, this, std::placeholders::_1));

    RCLCPP_INFO(node_->get_logger(), "CameraWatchDogNode initialized");
}

BT::NodeStatus CameraWatchDogNode::tick()
{
    if (camera_timeout_detected_)
    {
        RCLCPP_WARN(node_->get_logger(), "Camera timeout detected - camera system not responding");
        return BT::NodeStatus::FAILURE;
    }

    RCLCPP_DEBUG(node_->get_logger(), "Camera watchdog check passed - camera system healthy");
    return BT::NodeStatus::SUCCESS;
}

void CameraWatchDogNode::watchdogCallback(const std_msgs::msg::Bool::SharedPtr msg)
{
    // msg->data is true when camera is live, false when camera has timed out
    camera_timeout_detected_ = !msg->data;

    if (camera_timeout_detected_)
    {
        RCLCPP_WARN(node_->get_logger(), "Camera system timeout detected!");
    }
    else
    {
        RCLCPP_DEBUG(node_->get_logger(), "Camera watchdog status: camera system healthy");
    }
}
