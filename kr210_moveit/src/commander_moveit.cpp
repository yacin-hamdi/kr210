
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/utils.h>
#include <geometry_msgs/msg/pose_stamped.hpp>


class CommanderMoveIt
{
    public:
        CommanderMoveIt(std::shared_ptr<rclcpp::Node> node) 
        {
            node_ = node;

            // RCLCPP_INFO(get_logger(), "Commander node starting...");
            arm_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, "arm");
            gripper_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, "gripper");

            // RCLCPP_INFO(get_logger(), "Commander node started");

            arm_->setMaxVelocityScalingFactor(1.0);
            arm_->setMaxAccelerationScalingFactor(1.0);
            gripper_->setMaxVelocityScalingFactor(1.0);
            gripper_->setMaxAccelerationScalingFactor(1.0);

            gotToNamedTarget("pose_1");
            gotToNamedTarget("home");
            
        }

        void gotToNamedTarget(const std::string &name)
        {
            arm_->setStartStateToCurrentState();
            arm_->setNamedTarget(name);
            planAndExecute(arm_);
        }

        void goToJointTarget(const std::vector<double> &joints)
        {
            arm_->setStartStateToCurrentState();
            arm_->setJointValueTarget(joints);
            planAndExecute(arm_);
        }

        void goToPoseTarget(double x, double y, double z, double roll, double pitch, double yaw)
        {
            tf2::Quaternion q;
            q.setRPY(roll, pitch, yaw);
            q = q.normalize();

            geometry_msgs::msg::PoseStamped target_pose;
            target_pose.header.frame_id = "base_link";
            target_pose.pose.position.x = x;
            target_pose.pose.position.y = y;
            target_pose.pose.position.z = z;
            // target_pose.pose.orientation.x = q.getX();
            // target_pose.pose.orientation.y = q.getY();
            // target_pose.pose.orientation.z = q.getZ();
            // target_pose.pose.orientation.w = q.getW();

            arm_->setStartStateToCurrentState();
            arm_->setPoseTarget(target_pose);
            planAndExecute(arm_);
        }


    private:

    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> gripper_;
    std::shared_ptr<rclcpp::Node> node_;
    
    void planAndExecute(const std::shared_ptr<moveit::planning_interface::MoveGroupInterface> &interface)
    {
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        bool success = interface->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS;
        if(success){
            interface->execute(plan);
        }
    }


        
};


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("commander");
    auto commander = CommanderMoveIt(node);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

