#include <rclcpp/rclcpp.hpp>
#include <kr210_interfaces/msg/target_pose.hpp>

class PlaceToTrash: public rclcpp::Node
{
    public:
        PlaceToTrash(): Node("Place_to_trash"){
            pick_pub_ = create_publisher<kr210_interfaces::msg::TargetPose>("pick_object", 10);
            place_pub_ = create_publisher<kr210_interfaces::msg::TargetPose>("place_object", 10);
            pick_place_sub_ = create_subscription<kr210_interfaces::msg::TargetPose>("pick_place", 10,
                std::bind(&PlaceToTrash::pickAndPlaceCallback, this, std::placeholders::_1)
            );

            trash_pos_.x = 0.55;
            trash_pos_.y = -1.16;
            trash_pos_.z = 1.2;
            trash_pos_.roll = 0.0;
            trash_pos_.pitch = 0.0;
            trash_pos_.yaw = 0.0;
            
        }

    private:
        rclcpp::Subscription<kr210_interfaces::msg::TargetPose>::SharedPtr pick_place_sub_;
        rclcpp::Publisher<kr210_interfaces::msg::TargetPose>::SharedPtr pick_pub_;
        rclcpp::Publisher<kr210_interfaces::msg::TargetPose>::SharedPtr place_pub_;
        kr210_interfaces::msg::TargetPose trash_pos_;

    void pickAndPlaceCallback(const kr210_interfaces::msg::TargetPose &msg){
        pick_pub_->publish(msg);
        place_pub_->publish(trash_pos_);
    }
};


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PlaceToTrash>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}