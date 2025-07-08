/**
 * @file CameraWatchDogNode.hpp
 * @brief Behavior tree node for camera system monitoring
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

/**
 * @class CameraWatchDogNode
 * @brief Behavior tree condition node that monitors camera system health
 *
 * This node monitors camera system availability and returns FAILURE when
 * the camera is not responding or has timed out.
 */
class CameraWatchDogNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for CameraWatchDogNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    CameraWatchDogNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return SUCCESS if camera healthy, FAILURE if camera timeout detected
     */
    BT::NodeStatus tick() override;

private:
    /**
     * @brief Callback for camera watchdog status messages
     * @param msg Camera status message (true = camera live, false = camera timeout)
     */
    void watchdogCallback(const std_msgs::msg::Bool::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr watchdog_sub_;

    // Parameters loaded from config
    std::string camera_watchdog_topic_;
    int queue_size_;

    // State variables
    bool camera_timeout_detected_;
};
