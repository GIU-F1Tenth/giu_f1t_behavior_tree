/**
 * @file SafetyNode.hpp
 * @brief Behavior tree node for safety monitoring and emergency stopping
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

/**
 * @class SafetyNode
 * @brief Behavior tree condition node that monitors safety status
 *
 * This node continuously monitors safety conditions and can trigger
 * emergency stops when unsafe conditions are detected.
 */
class SafetyNode : public BT::SyncActionNode
{
public:
  /**
   * @brief Constructor for SafetyNode
   * @param name Node name
   * @param config Node configuration containing blackboard reference
   */
  SafetyNode(const std::string &name, const BT::NodeConfiguration &config);

  /**
   * @brief Provides the list of ports for this node
   * @return Empty port list (no input/output ports)
   */
  static BT::PortsList providedPorts() { return {}; }

  /**
   * @brief Main execution function called during behavior tree tick
   * @return SUCCESS if safe, FAILURE if unsafe conditions detected
   */
  BT::NodeStatus tick() override;

private:
  /**
   * @brief Callback for safety status messages
   * @param msg Safety status message
   */
  void safetyCallback(const std_msgs::msg::Bool::SharedPtr msg);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr safety_sub_;
  rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;

  // Parameters loaded from config
  std::string safety_topic_;
  std::string cmd_topic_;
  int queue_size_;

  // State variables
  bool unsafe_condition_detected_;
};
