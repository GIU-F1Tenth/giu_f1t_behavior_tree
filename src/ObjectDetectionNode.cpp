/**
 * @file ObjectDetectionNode.cpp
 * @brief Implementation of ObjectDetectionNode
 */

#include "ObjectDetectionNode.hpp"

ObjectDetectionNode::ObjectDetectionNode(const std::string &name, const BT::NodeConfiguration &config)
    : SyncActionNode(name, config), objects_detected_(false)
{
    node_ = config.blackboard->template get<rclcpp::Node::SharedPtr>("node");

    // Declare and get parameters
    node_->declare_parameter("topics.object_detection_input", "/tmp/obj_detected");
    node_->declare_parameter("topics.object_detection", "/obj_detected");
    node_->declare_parameter("queue_sizes.subscriber", 10);
    node_->declare_parameter("queue_sizes.publisher", 10);

    node_->get_parameter("topics.object_detection_input", detection_input_topic_);
    node_->get_parameter("topics.object_detection", detection_output_topic_);
    node_->get_parameter("queue_sizes.subscriber", queue_size_);

    // Create subscriber and publisher
    detection_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        detection_input_topic_, queue_size_,
        std::bind(&ObjectDetectionNode::detectionCallback, this, std::placeholders::_1));

    detection_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
        detection_output_topic_, queue_size_);

    RCLCPP_INFO(node_->get_logger(), "ObjectDetectionNode initialized");
}

BT::NodeStatus ObjectDetectionNode::tick()
{
    // Publish current detection status
    std_msgs::msg::Bool detection_msg;
    detection_msg.data = objects_detected_;
    detection_pub_->publish(detection_msg);

    if (objects_detected_)
    {
        RCLCPP_DEBUG(node_->get_logger(), "Objects detected");
        return BT::NodeStatus::SUCCESS;
    }
    else
    {
        RCLCPP_DEBUG(node_->get_logger(), "No objects detected");
        return BT::NodeStatus::FAILURE;
    }
}

void ObjectDetectionNode::detectionCallback(const std_msgs::msg::Bool::SharedPtr msg)
{
    objects_detected_ = msg->data;

    if (objects_detected_)
    {
        RCLCPP_INFO(node_->get_logger(), "Object detection status updated: objects detected");
    }
    else
    {
        RCLCPP_DEBUG(node_->get_logger(), "Object detection status updated: no objects");
    }
}
