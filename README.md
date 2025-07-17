# F1Tenth Behavior Tree Controller

A comprehensive ROS2 behavior tree package for F1Tenth autonomous racing vehicles. This package provides a modular, hierarchical behavior tree architecture for high-speed autonomous racing, featuring safety-first design, multi-strategy path planning, and robust fault tolerance.

## 🏎️ Features

### Core Capabilities

-   **Hierarchical Behavior Tree**: Modular decision-making architecture
-   **Safety-First Design**: Multiple redundant safety systems and watchdogs
-   **Multi-Strategy Path Planning**: A\*, optimized paths, and reactive gap following
-   **Real-Time Performance**: Configurable execution rates up to 50Hz
-   **Fault Tolerance**: Graceful degradation and emergency procedures

### Behavior Tree Nodes

-   **WatchDogNode**: Critical system health monitoring
-   **CameraWatchDogNode**: Camera system status monitoring
-   **EmergencyStopNode**: Immediate vehicle stopping
-   **SafetyNode**: Continuous safety monitoring and collision avoidance
-   **ObjectDetectionNode**: Object detection status processing
-   **AStarPathGeneratorNode**: A\* path planning activation
-   **OptimizedPathNode**: Pre-computed optimal path following
-   **PurePursuitNode**: Pure pursuit controller interface
-   **GapFollowerNode**: Reactive gap following for obstacle avoidance

## 📦 Package Structure

```
giu_f1t_behavior_tree/
├── bt_xml/                          # Behavior tree XML definitions
│   ├── behavior_tree.xml            # Primary behavior tree
│   ├── v7.xml                       # Current production tree
│   ├── minimal_tree_gap_pure.xml    # Minimal test configuration
│   └── v[2-6].xml                   # Historical tree versions
├── config/
│   └── behavior_tree_params.yaml    # Complete parameter configuration
├── include/giu_f1t_behavior_tree/   # Header files
│   ├── AStarPathGeneratorNode.hpp
│   ├── CameraWatchDogNode.hpp
│   ├── EmergencyStopNode.hpp
│   ├── GapFollowerNode.hpp
│   ├── ObjectDetectionNode.hpp
│   ├── OptimizedPathNode.hpp
│   ├── PurePursuitNode.hpp
│   ├── SafetyNode.hpp
│   └── WatchDogNode.hpp
├── launch/
│   └── bt.launch.py                 # Comprehensive launch configuration
├── src/                            # Implementation files
│   ├── bt_main.cpp                 # Main behavior tree executor
│   └── [Node].cpp                  # Individual node implementations
├── CMakeLists.txt                  # Build configuration
├── package.xml                     # Package metadata
└── README.md                       # This file
```

## 🚀 Quick Start

### Prerequisites

-   ROS2 (Humble or later)
-   BehaviorTree.CPP v3
-   Ackermann message support
-   F1Tenth vehicle hardware or simulation

### Installation

1. **Clone the repository:**

    ```bash
    cd ~/your_ws/src
    git clone <repository_url> giu_f1t_behavior_tree
    ```

2. **Install dependencies:**

    ```bash
    cd ~/your_ws
    rosdep install --from-paths src --ignore-src -r -y
    ```

3. **Build the package:**
    ```bash
    colcon build --packages-select giu_f1t_behavior_tree
    source install/setup.bash
    ```

### Basic Usage

1. **Launch with default production configuration:**

    ```bash
    ros2 launch giu_f1t_behavior_tree bt.launch.py
    ```

2. **Launch for development/debugging:**

    ```bash
    ros2 launch giu_f1t_behavior_tree bt.launch.py \
    groot_logging:=true
    ```

3. **Launch with custom behavior tree:**

    ```bash
    ros2 launch giu_f1t_behavior_tree bt.launch.py \
      bt_xml:=bt_xml/minimal_tree_gap_pure.xml \
      tick_rate:=50.0
    ```

## ⚙️ Configuration

The package includes multiple configuration files for different use cases:

### Configuration Files

-   **`behavior_tree_params.yaml`**: Production configuration for real vehicle deployment

### Behavior Tree Definitions

-   **`minimal_tree_gap_pure.xml`**: Minimal test configuration
-   **`v7.xml`**: Legacy production tree (maintained for compatibility)

## 🔧 Advanced Usage

### Custom Behavior Trees

1. **Create a new XML file** in `bt_xml/`:

    ```xml
    <?xml version="1.0" encoding="UTF-8"?>
    <root BTCPP_format="4" main_tree_to_execute="Main tree">
      <BehaviorTree ID="Main tree">
        <!-- Your tree structure here -->
      </BehaviorTree>
    </root>
    ```

2. **Update configuration** to use your tree:
    ```yaml
    behavior_tree_xml_path: "bt_xml/your_tree.xml"
    ```

### Groot Visualization

1. **Enable logging:**

    ```bash
    ros2 launch giu_f1t_behavior_tree bt.launch.py groot_logging:=true
    ```

2. **Connect Groot** to visualize tree execution in real-time

### Performance Tuning

-   **Increase tick rate** for faster response: `tick_rate_hz: 50.0`
-   **Adjust queue sizes** for high-throughput topics
-   **Enable performance logging** for timing analysis

### Configuration Selection Guide

**Params (`behavior_tree_params.yaml`)**:

-   Real vehicle deployment
-   Conservative safety settings
-   Standard topic names
-   Optimized for reliability

## 📊 Monitoring and Debugging

### ROS2 Topics

**Published Topics:**

-   `/ackermann_cmd` - Vehicle control commands
-   `/path_chooser` - Path planning decisions
-   `/gap_follower_toggle` - Gap follower activation
-   `/obj_detected` - Object detection status

**Subscribed Topics:**

-   `/tmp/safety` - Safety system status
-   `/tmp/obj_detected` - Object detection input
-   `/tmp/watchdog/critical` - System health
-   `/tmp/pp_ackermann_cmd` - Pure pursuit commands

### Logging

-   **Node status changes**: Logged at DEBUG level
-   **Safety events**: Logged at WARN/ERROR level
-   **System health**: Logged at INFO level

## 🛡️ Safety Features

### Multi-Layer Safety Architecture

1. **Watchdog Layer**: System health monitoring
2. **Safety Node**: Continuous collision avoidance
3. **Emergency Stop**: Immediate vehicle shutdown
4. **Graceful Degradation**: Fallback to safer behaviors

### Emergency Procedures

-   **Critical system failure** → Emergency stop
-   **Safety violation** → Emergency stop
-   **Camera timeout** → Switch to gap following
-   **Object detection** → Activate avoidance maneuvers

## 🤝 Contributing

1. **Follow ROS2 conventions** for naming and structure
2. **Add comprehensive documentation** for new nodes
3. **Include safety considerations** in all modifications
4. **Test thoroughly** in simulation before hardware deployment

### Adding New Nodes

1. **Create header file** in `include/giu_f1t_behavior_tree/`
2. **Implement source file** in `src/`
3. **Register node** in `bt_main.cpp`
4. **Update CMakeLists.txt** with new source files
5. **Add configuration parameters** to yaml file

## 📜 License

This project is licensed under the MIT License - see the package.xml file for details.

## 👨‍💻 Author

**Fam Shihata** - [fam@awadlouis.com](mailto:fam@awadlouis.com)

## 🙏 Acknowledgments

-   BehaviorTree.CPP framework
-   F1Tenth community
-   ROS2 ecosystem contributors

---

For more detailed technical documentation, refer to the individual header files and the behavior tree XML definitions.
