// CameraWatchDogNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class CameraWatchDogNode : public BT::SyncActionNode
{
public:
    CameraWatchDogNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {
        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        node_->declare_parameter<std::string>("camera_watchdog_topic", "/tmp/watchdog/camera_is_live");

        node_->get_parameter("camera_watchdog_topic", watchdog_topic_);

        watchdog_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
            watchdog_topic_, 10,
            std::bind(&CameraWatchDogNode::watchdogCallback, this, std::placeholders::_1));
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        return timed_out_ ? BT::NodeStatus::FAILURE : BT::NodeStatus::SUCCESS;
    }

private:
    void watchdogCallback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        timed_out_ = !msg->data;
    }

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr watchdog_sub_;
    std::string watchdog_topic_;
    bool timed_out_ = true;
};
