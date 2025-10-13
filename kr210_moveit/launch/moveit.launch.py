from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    use_sim_time_arg = DeclareLaunchArgument(
        name="use_sim_time", 
        default_value="true"
    )

    use_sim_time = LaunchConfiguration("use_sim_time")

    moveit_config = (
        MoveItConfigsBuilder("kr210", package_name="kr210_moveit")
        .robot_description(file_path=os.path.join(get_package_share_directory("kr210_description"), "urdf", "kr210.urdf.xacro"))
        .robot_description_semantic(file_path="config/kr210.srdf")
        .trajectory_execution(file_path="config/moveit_controllers.yaml")
        .to_moveit_configs()
    )


    
    



    move_group_node = Node(
        package="moveit_ros_move_group", 
        executable="move_group",
        output="screen", 
        parameters=[
            moveit_config.to_dict(),
            {"use_sim_time": use_sim_time}, 
            {"publish_robot_description_semantic": True}
        ]
    )

    

    rviz_config = os.path.join(get_package_share_directory("kr210_moveit"), "config", "moveit.rviz")

    rviz_node = Node(
        package="rviz2", 
        executable="rviz2", 
        name="rviz2", 
        output="screen", 
        arguments=["-d", rviz_config], 
        parameters=[
            moveit_config.robot_description, 
            moveit_config.robot_description_semantic, 
            moveit_config.robot_description_kinematics, 
            moveit_config.joint_limits
        ]
    )


    


    return LaunchDescription([
        use_sim_time_arg,
        move_group_node, 
        rviz_node
    ])