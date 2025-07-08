// OptimizedPathNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class OptimizedPathNode : public BT::SyncActionNode
{
public:
    OptimizedPathNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {
        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        decision_pub_ = node_->create_publisher<std_msgs::msg::String>(
            "/path_chooser", 10
        );
        toggle_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
            "/gap_follower_toggle", 10);
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        std_msgs::msg::String decision_msg;
        decision_msg.data = "csv_race_path";
        decision_pub_->publish(decision_msg);
        std_msgs::msg::Bool toggle_msg;
        toggle_msg.data = false; // Disable the gap follower
        toggle_pub_->publish(toggle_msg);
        return BT::NodeStatus::SUCCESS;
    }

private:

    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr decision_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr toggle_pub_;
};
