// GapFollowerNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class GapFollowerNode : public BT::SyncActionNode
{
public:
    GapFollowerNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {
        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        toggle_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
            "/gap_follower_toggle", 10);
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        std_msgs::msg::Bool toggle_msg;
        toggle_msg.data = true; // Enable the gap follower
        toggle_pub_->publish(toggle_msg);
        return BT::NodeStatus::SUCCESS;
    }

private:
    //     void scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    //     {
    //         latest_scan_ = *msg;
    //     }

    //     rclcpp::Node::SharedPtr node_;
    //     rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;



    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr toggle_pub_;
    rclcpp::Node::SharedPtr node_;
};
