/**
 * @file PurePursuitNode.hpp
 * @brief Behavior tree node for pure pursuit path following
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

/**
 * @class PurePursuitNode
 * @brief Behavior tree action node that handles pure pursuit control commands
 *
 * This node subscribes to pure pursuit control commands and forwards them
 * to the vehicle's command topic for execution.
 */
class PurePursuitNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for PurePursuitNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    PurePursuitNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return Always returns SUCCESS after publishing command
     */
    BT::NodeStatus tick() override;

private:
    /**
     * @brief Callback for pure pursuit control commands
     * @param msg Ackermann drive command message
     */
    void controlCallback(const ackermann_msgs::msg::AckermannDriveStamped::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr control_sub_;
    rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;

    // Parameters loaded from config
    std::string pure_pursuit_topic_;
    std::string cmd_topic_;
    std::string default_frame_id_;
    int queue_size_;

    // State variables
    ackermann_msgs::msg::AckermannDriveStamped latest_command_;
    bool command_received_;
};
