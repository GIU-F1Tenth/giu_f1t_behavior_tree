// ObjectDetectionNode.hpp
#pragma once
#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class ObjectDetectionNode : public BT::SyncActionNode
{
public:
    ObjectDetectionNode(const std::string &name, const BT::NodeConfiguration &config)
        : SyncActionNode(name, config)
    {
        node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

        node_->declare_parameter<std::string>("object_detection_topic", "/obj_detected");

        node_->get_parameter("object_detection_topic", object_detection_topic_);

        detect_sub_ = node_->create_subscription<std_msgs::msg::Bool>("/tmp" + object_detection_topic_, 10,
                                                                                  std::bind(&ObjectDetectionNode::detectionCallback, this, std::placeholders::_1));
        detect_pub_ = node_->create_publisher<std_msgs::msg::Bool>(object_detection_topic_, 10);
    }

    static BT::PortsList providedPorts() { return {}; }

    BT::NodeStatus tick() override
    {
        auto msg = std_msgs::msg::Bool();
        msg.data = latest_objects_;
        detect_pub_->publish(msg);
        if (!msg.data) {
            return BT::NodeStatus::FAILURE; 
        }
        return BT::NodeStatus::SUCCESS;
    }

private:
    void detectionCallback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        latest_objects_ = msg->data;
    }

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr detect_sub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr detect_pub_;
    bool latest_objects_;
    std::string object_detection_topic_;
};
