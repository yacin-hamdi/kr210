from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, TimerAction
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch_ros.substitutions import FindPackageShare

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

    

    

   

    

    return LaunchDescription([
        gazebo, 
        controller, 
        moveit
    ])