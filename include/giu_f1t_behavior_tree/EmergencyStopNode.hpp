// EmergencyStopNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <ackermann_msgs/msg/ackermann_drive_stamped.hpp>

class EmergencyStopNode : public BT::SyncActionNode
{
public:
    EmergencyStopNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {

        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        node_->declare_parameter<std::string>("emergency_cmd_vel_topic", "/ackermann_cmd");

        node_->get_parameter("emergency_cmd_vel_topic", cmd_vel_topic_);

        cmd_pub_ = node_->create_publisher<ackermann_msgs::msg::AckermannDriveStamped>(
            cmd_vel_topic_, 10);
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        ackermann_msgs::msg::AckermannDriveStamped stop;
        // zero out velocities
        stop.drive.speed = 0.0;
        stop.drive.steering_angle = 0.0;
        stop.drive.acceleration = 0.0;
        cmd_pub_->publish(stop);
        return BT::NodeStatus::SUCCESS;
    }

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<ackermann_msgs::msg::AckermannDriveStamped>::SharedPtr cmd_pub_;
    std::string cmd_vel_topic_;
};
