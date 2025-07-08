#!/usr/bin/env python3
"""
F1Tenth Behavior Tree Launch File

This launch file starts the behavior tree controller for F1Tenth autonomous racing.
It loads the configuration parameters and starts the main behavior tree node.

Usage:
    ros2 launch giu_f1t_behavior_tree bt.launch.py
    
Optional arguments:
    config_file:=path/to/custom/config.yaml    # Use custom configuration
    bt_xml:=path/to/behavior_tree.xml          # Use custom behavior tree
    tick_rate:=30.0                           # Override tick rate
    groot_logging:=true                       # Enable Groot visualization
"""

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node, SetParameter
from launch_ros.descriptions import ComposableNode
from launch_ros.actions import ComposableNodeContainer


def generate_launch_description():
    # Package directory
    pkg_dir = get_package_share_directory('giu_f1t_behavior_tree')

    # Default configuration file
    default_config_file = os.path.join(
        pkg_dir, 'config', 'behavior_tree_params.yaml')

    # Launch arguments
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=default_config_file,
        description='Path to the behavior tree configuration file'
    )

    bt_xml_arg = DeclareLaunchArgument(
        'bt_xml',
        default_value='bt_xml/v7.xml',
        description='Behavior tree XML file path (relative to package share directory)'
    )

    tick_rate_arg = DeclareLaunchArgument(
        'tick_rate',
        default_value='25.0',
        description='Behavior tree execution frequency in Hz'
    )

    groot_logging_arg = DeclareLaunchArgument(
        'groot_logging',
        default_value='false',
        description='Enable Groot logging for behavior tree visualization'
    )

    log_level_arg = DeclareLaunchArgument(
        'log_level',
        default_value='INFO',
        description='ROS logging level (DEBUG, INFO, WARN, ERROR)'
    )

    namespace_arg = DeclareLaunchArgument(
        'namespace',
        default_value='',
        description='Namespace for the behavior tree node'
    )

    # Parameter overrides
    parameter_overrides = {
        'behavior_tree_xml_path': LaunchConfiguration('bt_xml'),
        'tick_rate_hz': LaunchConfiguration('tick_rate'),
        'enable_groot_logging': LaunchConfiguration('groot_logging'),
    }

    # Main behavior tree node
    behavior_tree_node = Node(
        package='giu_f1t_behavior_tree',
        executable='bt_main',
        name='behavior_tree_controller',
        namespace=LaunchConfiguration('namespace'),
        output='screen',
        parameters=[
            LaunchConfiguration('config_file'),
            parameter_overrides
        ],
        arguments=['--ros-args', '--log-level',
                   LaunchConfiguration('log_level')],
        respawn=True,
        respawn_delay=2.0,
    )

    # Optional: Node for debugging/monitoring (commented out by default)
    # monitor_node = Node(
    #     package='giu_f1t_behavior_tree',
    #     executable='bt_monitor',  # Would need to be implemented
    #     name='bt_monitor',
    #     output='screen',
    #     condition=IfCondition(LaunchConfiguration('enable_monitoring'))
    # )

    return LaunchDescription([
        # Launch arguments
        config_file_arg,
        bt_xml_arg,
        tick_rate_arg,
        groot_logging_arg,
        log_level_arg,
        namespace_arg,

        # Nodes
        behavior_tree_node,
    ])
