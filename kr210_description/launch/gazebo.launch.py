

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, Command, PythonExpression
from launch_ros.substitutions import FindPackageShare
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory
import os 
from pathlib import Path

def generate_launch_description():

    robot_description_arg = DeclareLaunchArgument(
        name="robot_description", 
        default_value=PathJoinSubstitution([
            FindPackageShare("kr210_description"), 
            "urdf", 
            "kr210.urdf.xacro"
        ])
    )

    world_name_arg = DeclareLaunchArgument(
        name="world_name",
        default_value="test"
    )
    world_name = LaunchConfiguration("world_name")
    robot_description = ParameterValue(Command(["xacro ", LaunchConfiguration("robot_description")]), value_type=str)

    world_path = PathJoinSubstitution([
        FindPackageShare("kr210_description"),
        "worlds", 
        PythonExpression(["'", world_name, "'", " + '.world'"])
    ])

    
    

    resource_path = str(Path(get_package_share_directory("kr210_description")).parent.resolve())
    resource_path += os.pathsep + os.path.join(get_package_share_directory("kr210_description"), "models")
    gazebo_resource_path = SetEnvironmentVariable(
        name="GZ_SIM_RESOURCE_PATH", 
        value=resource_path
    )

    robot_state_publisher = Node(
        package="robot_state_publisher", 
        executable="robot_state_publisher", 
        parameters=[
            {"robot_description":robot_description}, 
            {"use_sim_time": True}
        ]
    )

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare("ros_gz_sim"),
                "launch",
                "gz_sim.launch.py"
            ])
        ),
        launch_arguments=[("gz_args", PythonExpression(["' -v 4 -r ", world_path, "'"]))]
        # launch_arguments=[("gz_args", PythonExpression(["' -v 4 -r ", "empty.sdf", "'"]))]
    )

    gz_spawn_entity = Node(
        package="ros_gz_sim", 
        executable="create", 
        output="screen", 
        arguments=["-topic", "robot_description", 
                    "-name", "kr210",
                   ]
    )

    

    gz_ros2_bridge = Node(
        package="ros_gz_bridge", 
        executable="parameter_bridge", 
        arguments=[
            "/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock",
        ]
    )
    return LaunchDescription([
        world_name_arg,
        robot_description_arg,
        robot_state_publisher,
        gazebo_resource_path,
        gazebo, 
        gz_spawn_entity, 
        gz_ros2_bridge, 

    ])