#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2/utils.h>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("test_moveit");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    auto spinner = std::thread([&executor](){ executor.spin();});

    // auto arm = moveit::planning_interface::MoveGroupInterface(node, "arm");
    // auto gripper = moveit::planning_interface::MoveGroupInterface(node, "gripper");

    // arm.setMaxVelocityScalingFactor(1.0);
    // arm.setMaxAccelerationScalingFactor(1.0);
    // gripper.setMaxVelocityScalingFactor(1.0);
    // gripper.setMaxAccelerationScalingFactor(1.0);

    // arm.setStartStateToCurrentState();
    // arm.setNamedTarget("pose_1");
    // gripper.setStartStateToCurrentState();
    // gripper.setNamedTarget("gripper_closed");

    // moveit::planning_interface::MoveGroupInterface::Plan arm_plan, gripper_plan;
    // bool arm_success = arm.plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS;
    // bool gripper_success = gripper.plan(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS;

    // if(arm_success && gripper_success){
    //     arm.execute(arm_plan);
    //     gripper.execute(gripper_plan);
    // }
        
    
    // arm.setStartStateToCurrentState();
    // arm.setNamedTarget("home");
    // gripper.setStartStateToCurrentState();
    // gripper.setNamedTarget("gripper_open");

    // arm_success = arm.plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS;
    // gripper_success = gripper.plan(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS;

    // if(arm_success && gripper_success){
    //     arm.execute(arm_plan);
    //     gripper.execute(gripper_plan);
    // }


    auto arm = moveit::planning_interface::MoveGroupInterface(node, "arm");
    
    
    tf2::Quaternion q;
    q.setRPY(0.0, 1.0, 1.0);
    q = q.normalize();

    geometry_msgs::msg::PoseStamped target_pose;
    target_pose.header.frame_id = "base_link";
    target_pose.pose.position.x = 0.0;
    target_pose.pose.position.y = 2.0;
    target_pose.pose.position.z = 1.0;
    // target_pose.pose.orientation.x = q.getX();
    // target_pose.pose.orientation.y = q.getY();
    // target_pose.pose.orientation.z = q.getZ();
    // target_pose.pose.orientation.w = q.getW();

    arm.setStartStateToCurrentState();
    arm.setPoseTarget(target_pose);

    moveit::planning_interface::MoveGroupInterface::Plan arm_plan;
    bool success = arm.plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS;

    if(success){
        arm.execute(arm_plan);
    }

    // Cartesian Path
    std::vector<geometry_msgs::msg::Pose> waypoints;
    geometry_msgs::msg::Pose pose1 = arm.getCurrentPose().pose;
    pose1.position.z -= 0.2;
    waypoints.push_back(pose1);
    geometry_msgs::msg::Pose pose2 = pose1;
    pose2.position.y += 0.2;
    waypoints.push_back(pose2);
    geometry_msgs::msg::Pose pose3 = pose2;
    pose3.position.y -= 0.2;
    pose3.position.z += 0.2;
    waypoints.push_back(pose3);

    moveit_msgs::msg::RobotTrajectory trajectory;

    double fraction = arm.computeCartesianPath(waypoints, 0.01, trajectory);

    if (fraction == 1){
        arm.execute(trajectory);
    }


    rclcpp::shutdown();
    spinner.join();
    return 0;
}