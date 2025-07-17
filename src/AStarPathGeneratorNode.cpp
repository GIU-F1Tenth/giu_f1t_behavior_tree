/**
 * @file AStarPathGeneratorNode.cpp
 * @brief Implementation of AStarPathGeneratorNode
 */

#include "AStarPathGeneratorNode.hpp"

AStarPathGeneratorNode::AStarPathGeneratorNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.path_chooser", "/path_chooser");
    node_->declare_parameter("topics.gap_follower_toggle", "/gap_follower_toggle");
    node_->declare_parameter("path_decisions.astar_path", "astar_path");
    node_->declare_parameter("queue_sizes.publisher", 10);

    node_->get_parameter("topics.path_chooser", path_chooser_topic_);
    node_->get_parameter("topics.gap_follower_toggle", gap_follower_toggle_topic_);
    node_->get_parameter("path_decisions.astar_path", astar_decision_message_);
    node_->get_parameter("queue_sizes.publisher", queue_size_);

    // Create publishers
    decision_pub_ = node_->create_publisher<std_msgs::msg::String>(
        path_chooser_topic_, queue_size_);
    gap_follower_toggle_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
        gap_follower_toggle_topic_, queue_size_);

    RCLCPP_INFO(node_->get_logger(), "AStarPathGeneratorNode initialized");
}

BT::NodeStatus AStarPathGeneratorNode::tick()
{
    // Publish decision to use A* path
    std_msgs::msg::String decision_msg;
    decision_msg.data = astar_decision_message_;
    decision_pub_->publish(decision_msg);

    // Disable gap follower
    std_msgs::msg::Bool toggle_msg;
    toggle_msg.data = false;
    gap_follower_toggle_pub_->publish(toggle_msg);

    RCLCPP_DEBUG(node_->get_logger(), "Published A* path decision and disabled gap follower");

    return BT::NodeStatus::SUCCESS;
}
