/**
 * @file ObjectDetectionNode.hpp
 * @brief Behavior tree node for object detection monitoring
 * @author Fam Shihata
 * @date 2025
 */

#pragma once

#include <behaviortree_cpp_v3/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

/**
 * @class ObjectDetectionNode
 * @brief Behavior tree condition node that monitors object detection status
 *
 * This node subscribes to object detection results and returns SUCCESS
 * when objects are detected, FAILURE otherwise.
 */
class ObjectDetectionNode : public BT::SyncActionNode
{
public:
    /**
     * @brief Constructor for ObjectDetectionNode
     * @param name Node name
     * @param config Node configuration containing blackboard reference
     */
    ObjectDetectionNode(const std::string &name, const BT::NodeConfiguration &config);

    /**
     * @brief Provides the list of ports for this node
     * @return Empty port list (no input/output ports)
     */
    static BT::PortsList providedPorts() { return {}; }

    /**
     * @brief Main execution function called during behavior tree tick
     * @return SUCCESS if objects detected, FAILURE otherwise
     */
    BT::NodeStatus tick() override;

private:
    /**
     * @brief Callback for object detection status messages
     * @param msg Object detection status message
     */
    void detectionCallback(const std_msgs::msg::Bool::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr detection_sub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr detection_pub_;

    // Parameters loaded from config
    std::string detection_input_topic_;
    std::string detection_output_topic_;
    int queue_size_;

    // State variables
    bool objects_detected_;
};
