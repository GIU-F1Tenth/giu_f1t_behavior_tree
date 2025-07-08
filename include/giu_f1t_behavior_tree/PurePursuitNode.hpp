// PurePursuitNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/path.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

class PurePursuitNode : public BT::SyncActionNode
{
public:
    PurePursuitNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {
        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        node_->declare_parameter<std::string>("pure_pursuit_topic", "/tmp/pp_ackermann_cmd");
        node_->declare_parameter<std::string>("pure_cmd_vel_topic", "/ackermann_cmd");

        node_->get_parameter("pure_pursuit_topic", pure_pursuit_topic_);
        node_->get_parameter("pure_cmd_vel_topic", cmd_vel_topic_);

        pure_pursuit_sub_ = node_->create_subscription<ackermann_msgs::msg::AckermannDriveStamped>(
            pure_pursuit_topic_, 10, std::bind(&PurePursuitNode::pathCallback, this, std::placeholders::_1));
        cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(
            cmd_vel_topic_, 10);
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        cmd_pub_->publish(latest_command_);
        return BT::NodeStatus::SUCCESS;
    }

private:
    void pathCallback(const ackermann_msgs::msg::AckermannDriveStamped::SharedPtr msg)
    {
        latest_command_ = *msg;
    }

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr pure_pursuit_sub_;
    rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;
    std::string pure_pursuit_topic_;
    std::string cmd_vel_topic_;
    ackermann_msgs::msg::AckermannDriveStamped latest_command_;
};
