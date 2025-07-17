/**
 * @file bt_main.cpp
 * @brief Main entry point for the F1Tenth behavior tree controller
 * @author Fam Shihata
 * @date 2025
 */

#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <rclcpp/rclcpp.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>

// Include all custom behavior tree nodes
#include "WatchDogNode.hpp"
#include "CameraWatchDogNode.hpp"
#include "EmergencyStopNode.hpp"
#include "SafetyNode.hpp"
#include "ObjectDetectionNode.hpp"
#include "AStarPathGeneratorNode.hpp"
#include "OptimizedPathNode.hpp"
#include "PurePursuitNode.hpp"
#include "GapFollowerNode.hpp"

/**
 * @brief Register all custom behavior tree nodes with the factory
 * @param factory BehaviorTree factory instance
 */
void registerNodes(BT::BehaviorTreeFactory &factory)
{
    factory.registerNodeType<WatchDogNode>("WatchDogNode");
    factory.registerNodeType<CameraWatchDogNode>("CameraWatchDogNode");
    factory.registerNodeType<EmergencyStopNode>("EmergencyStopNode");
    factory.registerNodeType<SafetyNode>("SafetyNode");
    factory.registerNodeType<ObjectDetectionNode>("ObjectDetectionNode");
    factory.registerNodeType<AStarPathGeneratorNode>("AStarPathGeneratorNode");
    factory.registerNodeType<OptimizedPathNode>("OptimizedPathNode");
    factory.registerNodeType<PurePursuitNode>("PurePursuitNode");
    factory.registerNodeType<GapFollowerNode>("GapFollowerNode");
}

/**
 * @brief Main function for the behavior tree controller
 */
int main(int argc, char **argv)
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);

    // Create ROS node
    auto ros_node = std::make_shared<rclcpp::Node>("bt_runner");

    RCLCPP_INFO(ros_node->get_logger(), "Starting F1Tenth Behavior Tree Controller");

    // Declare parameters with defaults
    ros_node->declare_parameter("behavior_tree_xml_path", "bt_xml/v7.xml");
    ros_node->declare_parameter("tick_rate_hz", 25.0);
    ros_node->declare_parameter("enable_groot_logging", false);
    ros_node->declare_parameter("fallback_xml_path", "/home/ubuntu/giu_f1tenth_ws/software/src/planning/giu_f1t_behavior_tree/bt_xml/v7.xml");
    ros_node->declare_parameter("logging.log_level", "INFO");
    ros_node->declare_parameter("logging.enable_node_status_logging", true);

    // Get parameters
    std::string xml_relative_path, xml_file, fallback_xml_path;
    double tick_rate_hz;
    bool enable_groot_logging, enable_node_status_logging;
    std::string log_level;

    ros_node->get_parameter("behavior_tree_xml_path", xml_relative_path);
    ros_node->get_parameter("tick_rate_hz", tick_rate_hz);
    ros_node->get_parameter("enable_groot_logging", enable_groot_logging);
    ros_node->get_parameter("fallback_xml_path", fallback_xml_path);
    ros_node->get_parameter("logging.log_level", log_level);
    ros_node->get_parameter("logging.enable_node_status_logging", enable_node_status_logging);

    // Construct full path to XML file
    try
    {
        std::string package_share_directory = ament_index_cpp::get_package_share_directory("giu_f1t_behavior_tree");
        xml_file = package_share_directory + "/" + xml_relative_path;
    }
    catch (const std::exception &e)
    {
        RCLCPP_ERROR(ros_node->get_logger(), "Failed to find package share directory: %s", e.what());
        // Use configurable fallback path
        xml_file = fallback_xml_path;
        RCLCPP_WARN(ros_node->get_logger(), "Using fallback XML path: %s", xml_file.c_str());
    }

    RCLCPP_INFO(ros_node->get_logger(), "Loading behavior tree from: %s", xml_file.c_str());

    // Create BehaviorTree factory and register nodes
    BT::BehaviorTreeFactory factory;
    registerNodes(factory);

    // Prepare the blackboard and pass the ROS node pointer
    BT::Blackboard::Ptr blackboard = BT::Blackboard::create();
    blackboard->set("node", ros_node);

    // Load the Behavior Tree from XML file
    BT::Tree tree;
    try
    {
        tree = factory.createTreeFromFile(xml_file, blackboard);
        RCLCPP_INFO(ros_node->get_logger(), "Behavior tree loaded successfully");
    }
    catch (const std::exception &e)
    {
        RCLCPP_ERROR(ros_node->get_logger(), "Failed to load behavior tree: %s", e.what());
        rclcpp::shutdown();
        return 1;
    }

    // Optional: Enable Groot logging for visualization
    std::unique_ptr<BT::StdCoutLogger> logger_cout;
    if (enable_groot_logging)
    {
        logger_cout = std::make_unique<BT::StdCoutLogger>(tree);
        RCLCPP_INFO(ros_node->get_logger(), "Groot logging enabled");
    }

    // Main execution loop
    RCLCPP_INFO(ros_node->get_logger(), "Starting behavior tree execution at %.1f Hz", tick_rate_hz);
    rclcpp::Rate rate(tick_rate_hz);

    while (rclcpp::ok())
    {
        // Execute one tick of the behavior tree
        BT::NodeStatus tree_status = tree.tickRoot();

        // Process ROS callbacks
        rclcpp::spin_some(ros_node);

        // Log tree status on changes (configurable)
        static BT::NodeStatus last_status = BT::NodeStatus::IDLE;
        if (enable_node_status_logging && tree_status != last_status)
        {
            std::string status_str;
            switch (tree_status)
            {
            case BT::NodeStatus::SUCCESS:
                status_str = "SUCCESS";
                break;
            case BT::NodeStatus::FAILURE:
                status_str = "FAILURE";
                break;
            case BT::NodeStatus::RUNNING:
                status_str = "RUNNING";
                break;
            case BT::NodeStatus::IDLE:
                status_str = "IDLE";
                break;
            default:
                status_str = "UNKNOWN";
                break;
            }
            RCLCPP_DEBUG(ros_node->get_logger(), "Behavior tree status: %s", status_str.c_str());
            last_status = tree_status;
        }

        rate.sleep();
    }

    RCLCPP_INFO(ros_node->get_logger(), "Shutting down F1Tenth Behavior Tree Controller");
    rclcpp::shutdown();
    return 0;
}
