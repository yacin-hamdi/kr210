# KUKA KR210 Robot Arm Simulation

<p align="center">
  <img src="https://img.shields.io/badge/ROS2-Humble-blue?style=for-the-badge&logo=ros" alt="ROS2 Humble"/>
  <img src="https://img.shields.io/badge/Gazebo-Ignition-orange?style=for-the-badge" alt="Gazebo Ignition"/>
  <img src="https://img.shields.io/badge/MoveIt2-Motion%20Planning-green?style=for-the-badge" alt="MoveIt2"/>
  <img src="https://img.shields.io/badge/License-MIT-lightgrey?style=for-the-badge" alt="License MIT"/>
</p>

<p align="center">
  A complete ROS2-based simulation of the <strong>KUKA KR210 L150</strong> industrial robot arm with MoveIt2 motion planning and Gazebo Ignition integration.
</p>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Package Structure](#package-structure)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

This project provides a complete ROS2 simulation environment for the **KUKA KR210 L150** industrial robot arm. The KR210 is a 6-axis articulated robot designed for heavy-duty industrial applications with a payload capacity of 210 kg and a reach of 3100 mm.

---

## Features

| Feature | Description |
|---------|-------------|
| **6-DOF Arm** | Complete URDF/Xacro model with 6 revolute joints |
| **Parallel Gripper** | Two-finger gripper for manipulation tasks |
| **MoveIt2 Motion Planning** | OMPL-based path planning with collision avoidance |
| **Gazebo Ignition Simulation** | High-fidelity physics and sensor simulation |
| **ROS2 Control** | Joint trajectory controllers for arm and gripper |
| **RViz2 Visualization** | Real-time robot state visualization |
| **Sim Time Support** | Synchronized simulation time across all nodes |

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        ROS2 Ecosystem                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐      │
│  │   MoveIt2    │───▶│  ros2_control│───▶│   Gazebo     │      │
│  │ Move Group   │    │  Controller  │    │  Ignition    │      │
│  └──────────────┘    │   Manager    │    └──────────────┘      │
│         │            └──────────────┘           │               │
│         │                   │                   │               │
│         ▼                   ▼                   ▼               │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐      │
│  │    RViz2     │    │    Arm &     │    │   Physics    │      │
│  │ Visualization│    │   Gripper    │    │  Simulation  │      │
│  └──────────────┘    │ Controllers  │    └──────────────┘      │
│                      └──────────────┘                           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Prerequisites

Ensure you have the following installed:

- **Ubuntu 22.04** (Jammy Jellyfish)
- **ROS2 Humble Hawksbill**
- **Gazebo Ignition (Fortress)**
- **MoveIt2**

### Required ROS2 Packages

```bash
sudo apt install ros-humble-ros2-control \
                 ros-humble-ros2-controllers \
                 ros-humble-gazebo-ros2-control \
                 ros-humble-ros-gz-sim \
                 ros-humble-ros-gz-bridge \
                 ros-humble-moveit \
                 ros-humble-xacro \
                 ros-humble-joint-state-publisher-gui \
                 ros-humble-robot-state-publisher
```

---

## Installation

### 1. Create a ROS2 Workspace

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

### 2. Clone the Repository

```bash
git clone https://github.com/yacin-hamdi/kr210.git
```

### 3. Install Dependencies

```bash
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
```

### 4. Build the Workspace

```bash
colcon build 
```

### 5. Source the Workspace

```bash
source ~/ros2_ws/install/setup.bash
```

> [!TIP]
> Add the source command to your `~/.bashrc` to automatically source the workspace on new terminal sessions.

---

## Usage

### Launch Full Simulation with MoveIt2

This launches Gazebo, ROS2 controllers, and MoveIt2 motion planning:

```bash
ros2 launch kr210_bringup kr210_simulation.launch.py
```

### Launch Gazebo Only

To launch just the Gazebo simulation:

```bash
ros2 launch kr210_description gazebo.launch.py
```

You can specify a custom world:

```bash
ros2 launch kr210_description gazebo.launch.py world_name:=test
```

### Launch RViz Visualization

To visualize the robot in RViz without simulation:

```bash
ros2 launch kr210_description display.launch.py
```

### Launch MoveIt2 Demo

To run the MoveIt2 demo with RViz:

```bash
ros2 launch kr210_moveit demo.launch.py
```

---

## Package Structure

```
kr210/
├── kr210_bringup/          # High-level launch files
│   └── launch/
│       ├── kr210_simulation.launch.py    # Full simulation launch
│       ├── kr210_gazebo.launch.xml       # Gazebo launch wrapper
│       └── kr210_rviz.launch.xml         # RViz launch wrapper
│
├── kr210_controller/       # ROS2 Control configuration
│   ├── config/
│   │   └── ros2_controllers.yaml         # Controller definitions
│   └── launch/
│       └── controller.launch.py          # Controller spawner
│
├── kr210_description/      # Robot model and visualization
│   ├── urdf/
│   │   ├── kr210.urdf.xacro              # Main robot description
│   │   ├── kr210_arm.xacro               # Arm links and joints
│   │   ├── kr210_gripper.xacro           # Gripper definition
│   │   ├── kr210_ros2_control.xacro      # ros2_control hardware interface
│   │   └── kr210_*_gazebo.xacro          # Gazebo-specific tags
│   ├── meshes/                           # Visual and collision meshes
│   ├── models/                           # Gazebo SDF models
│   ├── worlds/                           # Gazebo world files
│   ├── rviz/                             # RViz configuration
│   └── launch/
│       ├── gazebo.launch.py              # Gazebo simulation launch
│       └── display.launch.py             # RViz visualization launch
│
├── kr210_interfaces/       # Custom ROS2 messages/services
│   └── msg/                              # Custom message definitions
│
└── kr210_moveit/           # MoveIt2 configuration
    ├── config/
    │   ├── kr210.srdf                    # Semantic robot description
    │   ├── kinematics.yaml               # IK solver configuration (KDL)
    │   ├── joint_limits.yaml             # Joint velocity/acceleration limits
    │   ├── moveit_controllers.yaml       # MoveIt controller configuration
    │   └── initial_positions.yaml        # Home position definition
    ├── launch/
    │   ├── demo.launch.py                # Full MoveIt demo
    │   ├── move_group.launch.py          # Move group node
    │   ├── moveit.launch.py              # MoveIt launch file
    │   └── moveit_rviz.launch.py         # MoveIt RViz plugin
    └── src/
        ├── commander_moveit.cpp          # MoveIt commander node
        └── place_to_trash.cpp            # Pick and place application
```

---

## Configuration

### Robot Specifications

| Parameter | Value |
|-----------|-------|
| **Model** | KUKA KR210 L150 |
| **Degrees of Freedom** | 6 |
| **Payload** | 210 kg |
| **Reach** | 3100 mm |
| **Repeatability** | ±0.06 mm |

### Joint Configuration

| Joint | Type | Limits | Max Velocity |
|-------|------|--------|--------------|
| Joint 1 | Revolute | ±185° | 123°/s |
| Joint 2 | Revolute | -45° to +85° | 115°/s |
| Joint 3 | Revolute | -210° to +65° | 112°/s |
| Joint 4 | Revolute | ±125° | 179°/s |
| Joint 5 | Revolute | ±125° | 172°/s |
| Joint 6 | Revolute | ±350° | 219°/s |

### Controllers

The robot uses joint trajectory controllers for both the arm and gripper:

```yaml
arm_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints: [joint_1, joint_2, joint_3, joint_4, joint_5, joint_6]
  
gripper_controller:
  type: joint_trajectory_controller/JointTrajectoryController
  joints: [gripper_left_finger_joint, gripper_right_finger_joint]
```

### Kinematics Solver

The MoveIt2 configuration uses the KDL kinematics plugin:

```yaml
arm:
  kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
  kinematics_solver_search_resolution: 0.005
  kinematics_solver_timeout: 0.005
```

---

## Troubleshooting

### Common Issues

<details>
<summary><strong>Gazebo fails to spawn the robot</strong></summary>

Ensure the `GZ_SIM_RESOURCE_PATH` is correctly set. The launch file should handle this automatically, but you can manually set it:

```bash
export GZ_SIM_RESOURCE_PATH=$GZ_SIM_RESOURCE_PATH:~/ros2_ws/install/kr210_description/share
```
</details>

<details>
<summary><strong>MoveIt planning fails</strong></summary>

1. Ensure all controllers are running: `ros2 control list_controllers`
2. Check that the robot state is being published: `ros2 topic echo /joint_states`
3. Verify the planning scene is collision-free in RViz
</details>

<details>
<summary><strong>Controllers not loading</strong></summary>

Make sure the `controller_manager` is running and the `ros2_controllers.yaml` is properly configured:

```bash
ros2 control list_hardware_interfaces
ros2 control list_controllers
```
</details>

---

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## Author

**Yacin Hamdi**  
Email: yacin.ha9@gmail.com

---

## Acknowledgments

- [ROS2](https://docs.ros.org/en/humble/) - Robot Operating System 2
- [MoveIt2](https://moveit.ros.org/) - Motion Planning Framework
- [Gazebo](https://gazebosim.org/) - Physics Simulation
- [KUKA Robotics](https://www.kuka.com/) - Robot specifications and inspiration

---


