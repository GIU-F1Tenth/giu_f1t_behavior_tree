/**
 * @file GapFollowerNode.cpp
 * @brief Implementation of GapFollowerNode
 */

#include "GapFollowerNode.hpp"

GapFollowerNode::GapFollowerNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.gap_follower_toggle", "/gap_follower_toggle");
    node_->declare_parameter("queue_sizes.publisher", 10);

    node_->get_parameter("topics.gap_follower_toggle", gap_follower_toggle_topic_);
    node_->get_parameter("queue_sizes.publisher", queue_size_);

    // Create publisher
    gap_follower_toggle_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
        gap_follower_toggle_topic_, queue_size_);

    RCLCPP_INFO(node_->get_logger(), "GapFollowerNode initialized");
}

BT::NodeStatus GapFollowerNode::tick()
{
    // Enable gap follower
    std_msgs::msg::Bool toggle_msg;
    toggle_msg.data = true;
    gap_follower_toggle_pub_->publish(toggle_msg);

    RCLCPP_DEBUG(node_->get_logger(), "Gap follower enabled");

    return BT::NodeStatus::SUCCESS;
}
