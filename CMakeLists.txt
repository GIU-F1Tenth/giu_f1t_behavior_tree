cmake_minimum_required(VERSION 3.8)
project(giu_f1t_behavior_tree)

# Set C++ standard
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(ackermann_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

# Include directories
include_directories(
  include
  include/${PROJECT_NAME}
)

# Build the main behavior tree executable with all node implementations
add_executable(bt_main
  src/bt_main.cpp
  src/WatchDogNode.cpp
  src/CameraWatchDogNode.cpp
  src/EmergencyStopNode.cpp
  src/SafetyNode.cpp
  src/ObjectDetectionNode.cpp
  src/AStarPathGeneratorNode.cpp
  src/OptimizedPathNode.cpp
  src/PurePursuitNode.cpp
  src/GapFollowerNode.cpp
)

# Target dependencies
ament_target_dependencies(bt_main
  rclcpp
  behaviortree_cpp_v3
  ament_index_cpp
  nav_msgs
  geometry_msgs
  ackermann_msgs
  sensor_msgs
  std_msgs
  visualization_msgs
  tf2_ros
)

# Install targets
install(TARGETS
  bt_main
  DESTINATION lib/${PROJECT_NAME}
)

# Install directories
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY bt_xml
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include/
)

# Testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # Skip linter checks that require additional setup
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Export information for downstream packages
ament_export_include_directories(include)
ament_export_dependencies(
  rclcpp
  behaviortree_cpp_v3
  ament_index_cpp
  ackermann_msgs
  sensor_msgs
  std_msgs
  nav_msgs
  geometry_msgs
  visualization_msgs
  tf2_ros
)

ament_package()
