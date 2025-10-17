from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, TimerAction
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

def generate_launch_description():
    
    gazebo = IncludeLaunchDescription(
        PathJoinSubstitution([
            FindPackageShare("kr210_description"), 
            "launch",
            "gazebo.launch.py"
        ])
    )

    moveit = IncludeLaunchDescription(
        PathJoinSubstitution([
            FindPackageShare("kr210_moveit"), 
            "launch", 
            "moveit.launch.py"
        ])
        
    )

    controller = IncludeLaunchDescription(
                    PathJoinSubstitution([
                        FindPackageShare("kr210_controller"),
                        "launch", 
                        "controller.launch.py"
                    ])
                )

    
    commander = Node(
        package="kr210_moveit",
        executable="commander_moveit"
    )

    pick_place = Node(
        package="kr210_moveit", 
        executable="place_to_trash"
    )
    

   

    

    return LaunchDescription([
        gazebo, 
        controller, 
        moveit, 
        commander, 
        # pick_place
    ])