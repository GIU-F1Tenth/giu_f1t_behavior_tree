/**
 * @file WatchDogNode.hpp
 * @brief Behavior tree node for critical system monitoring
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

/**
 * @class WatchDogNode
 * @brief Behavior tree condition node that monitors critical system status
 *
 * This node monitors critical system health and returns FAILURE when
 * critical timeouts or failures are detected, triggering emergency procedures.
 */
class WatchDogNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for WatchDogNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    WatchDogNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return SUCCESS if systems healthy, FAILURE if critical timeout detected
     */
    BT::NodeStatus tick() override;

private:
    /**
     * @brief Callback for critical watchdog status messages
     * @param msg Watchdog status message (true = timeout/failure detected)
     */
    void watchdogCallback(const std_msgs::msg::Bool::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr watchdog_sub_;

    // Parameters loaded from config
    std::string watchdog_topic_;
    int queue_size_;

    // State variables
    bool critical_timeout_detected_;
};
