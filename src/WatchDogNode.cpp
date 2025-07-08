/**
 * @file WatchDogNode.cpp
 * @brief Implementation of WatchDogNode
 */

#include "WatchDogNode.hpp"

WatchDogNode::WatchDogNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config), critical_timeout_detected_(true)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.watchdog_critical", "/tmp/watchdog/critical");
    node_->declare_parameter("queue_sizes.subscriber", 10);

    node_->get_parameter("topics.watchdog_critical", watchdog_topic_);
    node_->get_parameter("queue_sizes.subscriber", queue_size_);

    // Create subscriber
    watchdog_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        watchdog_topic_, queue_size_,
        std::bind(&WatchDogNode::watchdogCallback, this, std::placeholders::_1));

    RCLCPP_INFO(node_->get_logger(), "WatchDogNode initialized");
}

BT::NodeStatus WatchDogNode::tick()
{
    if (critical_timeout_detected_)
    {
        RCLCPP_WARN(node_->get_logger(), "Critical timeout detected - triggering emergency response");
        return BT::NodeStatus::FAILURE;
    }

    RCLCPP_DEBUG(node_->get_logger(), "Watchdog check passed - systems healthy");
    return BT::NodeStatus::SUCCESS;
}

void WatchDogNode::watchdogCallback(const std_msgs::msg::Bool::SharedPtr msg)
{
    critical_timeout_detected_ = msg->data;

    if (critical_timeout_detected_)
    {
        RCLCPP_ERROR(node_->get_logger(), "Critical system timeout detected!");
    }
    else
    {
        RCLCPP_DEBUG(node_->get_logger(), "Watchdog status: systems healthy");
    }
}
