#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <rclcpp/rclcpp.hpp>

// Include your custom nodes
#include "WatchDogNode.hpp"
#include "EmergencyStopNode.hpp"
#include "SafetyNode.hpp"
#include "ObjectDetectionNode.hpp"
#include "AStarPathGeneratorNode.hpp"
#include "OptimizedPathNode.hpp"
#include "PurePursuitNode.hpp"
#include "GapFollowerNode.hpp"
#include "CameraWatchDogNode.hpp"

int main(int argc, char **argv)
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);
    auto ros_node = std::make_shared<rclcpp::Node>("bt_runner");

    // Create BehaviorTree factory
    BT::BehaviorTreeFactory factory;

    // Register custom nodes with the factory
    factory.registerNodeType<WatchDogNode>("WatchDogNode");
    factory.registerNodeType<CameraWatchDogNode>("CameraWatchDogNode");
    factory.registerNodeType<EmergencyStopNode>("EmergencyStopNode");
    factory.registerNodeType<SafetyNode>("SafetyNode");
    factory.registerNodeType<ObjectDetectionNode>("ObjectDetectionNode");
    factory.registerNodeType<AStarPathGeneratorNode>("AStarPathGeneratorNode");
    factory.registerNodeType<OptimizedPathNode>("OptimizedPathNode");
    factory.registerNodeType<PurePursuitNode>("PurePursuitNode");
    factory.registerNodeType<GapFollowerNode>("GapFollowerNode");

    // Prepare the blackboard and pass the ROS node pointer
    BT::Blackboard::Ptr blackboard = BT::Blackboard::create();
    blackboard->set("node", ros_node);

    // Load the Behavior Tree from XML file
    const std::string xml_file = "/home/ubuntu/giu_f1tenth_ws/software/src/planning/giu_f1t_behavior_tree/bt_xml/v7.xml";
    auto tree = factory.createTreeFromFile(xml_file, blackboard);

    // (Optional) Logger to print to console
    // BT::StdCoutLogger logger_cout(tree);

    // Main loop: tick the tree and spin ROS callbacks
    rclcpp::Rate rate(25); // 50 Hz update rate
    while (rclcpp::ok())
    {
        tree.tickRoot();             // Execute one tick of the BT
        rclcpp::spin_some(ros_node); // Process ROS callbacks
        rate.sleep();
    }

    // Shutdown ROS cleanly
    rclcpp::shutdown();
    return 0;
}
