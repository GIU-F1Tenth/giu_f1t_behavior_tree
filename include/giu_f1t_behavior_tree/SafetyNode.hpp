// SafetyNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/range.hpp>
#include <std_msgs/msg/bool.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

class SafetyNode : public BT::SyncActionNode
{
public:
  SafetyNode(const std::string &name, const BT::NodeConfiguration &config)
      : SyncActionNode(name, config)
  {
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    node_->declare_parameter("safety_node_topic", "/tmp/safety");
    node_->declare_parameter("cmd_vel_topic", "/ackermann_cmd");

    node_->get_parameter("safety_node_topic", safety_node_topic_);
    node_->get_parameter("cmd_vel_topic", cmd_vel_topic_);

    safety_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        safety_node_topic_, 10,
        std::bind(&SafetyNode::safetyCallback, this, std::placeholders::_1));
    cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(cmd_vel_topic_, 10);
  }

  static BT::PortsList providedPorts() { return {}; }

  BT::NodeStatus tick() override
  {

    if(range_){
      ackermann_msgs::msg::AckermannDriveStamped stop;
      stop.drive.speed = 0.0;
      stop.drive.steering_angle = 0.0;
      stop.drive.acceleration = 0.0;

      cmd_pub_->publish(stop);
      return BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::SUCCESS;
  }

private:
  void safetyCallback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    range_ = msg->data;
  }

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr safety_sub_;
  rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;
  std::string safety_node_topic_; 
  std::string cmd_vel_topic_;
  bool range_;
};
