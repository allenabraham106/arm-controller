#include "arm_controller/arm_controller.hpp"

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){

}

bool ArmController::initialize(){
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_, "panda_arm"
    ); 
    RCLCPP_INFO(node_->get_logger(), "ArmController Initialized");
    return true; 
}

bool ArmController::moveToPose(double x, double y, double z){
    geometry_msgs::msg::Pose target_pose; 
    target_pose.position.x = x;
    target_pose.position.y = y;
    target_pose.position.z = z; 
    target_pose.orientation.w = 1.0;

    move_group_->setPoseTarget(target_pose); 
    auto result = move_group_->move();
    return result == moveit::core::MoveItErrorCode::SUCCESS;
}

bool ArmController::stop(){
    move_group_->stop();
    RCLCPP_INFO(node_->get_logger(), "ArmController Stopped");
    return true;
}