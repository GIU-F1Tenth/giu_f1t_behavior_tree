/**
 * @file GapFollowerNode.hpp
 * @brief Behavior tree node for activating gap following behavior
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

/**
 * @class GapFollowerNode
 * @brief Behavior tree action node that activates gap following mode
 *
 * This node enables the gap follower algorithm, which allows the vehicle
 * to navigate through gaps in obstacles using reactive control.
 */
class GapFollowerNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for GapFollowerNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    GapFollowerNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return Always returns SUCCESS after enabling gap follower
     */
    BT::NodeStatus tick() override;

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr gap_follower_toggle_pub_;

    // Parameters loaded from config
    std::string gap_follower_toggle_topic_;
    int queue_size_;
};
