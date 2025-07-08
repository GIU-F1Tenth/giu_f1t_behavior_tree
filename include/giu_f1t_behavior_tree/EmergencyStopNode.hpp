/**
 * @file EmergencyStopNode.hpp
 * @brief Behavior tree node for emergency vehicle stopping
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

/**
 * @class EmergencyStopNode
 * @brief Behavior tree action node that immediately stops the vehicle
 *
 * This node publishes zero velocity and steering commands to bring
 * the vehicle to an immediate stop in emergency situations.
 */
class EmergencyStopNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for EmergencyStopNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    EmergencyStopNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return Always returns SUCCESS after publishing stop command
     */
    BT::NodeStatus tick() override;

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;

    // Parameters loaded from config
    std::string emergency_cmd_topic_;
    int queue_size_;

    // Emergency stop vehicle dynamics parameters
    double emergency_speed_;
    double emergency_steering_angle_;
    double emergency_acceleration_;
    double emergency_jerk_;
    double emergency_steering_velocity_;
};
