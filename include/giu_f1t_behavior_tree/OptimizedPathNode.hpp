/**
 * @file OptimizedPathNode.hpp
 * @brief Behavior tree node for activating optimized path following
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>

/**
 * @class OptimizedPathNode
 * @brief Behavior tree action node that activates optimized path following mode
 *
 * This node publishes a decision message to switch the path planner to use
 * an optimized/pre-computed path (e.g., CSV race path) and disables the gap follower.
 */
class OptimizedPathNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for OptimizedPathNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    OptimizedPathNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return Always returns SUCCESS after publishing decision messages
     */
    BT::NodeStatus tick() override;

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr decision_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr gap_follower_toggle_pub_;

    // Parameters loaded from config
    std::string path_chooser_topic_;
    std::string gap_follower_toggle_topic_;
    std::string optimized_decision_message_;
    int queue_size_;
};
