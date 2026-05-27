#include "arm_controller/arm_controller.hpp"

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){

}

bool ArmController::initialize(){
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_,
        "panda_arm"
    ); 
    clicked_point_sub_ = node_->create_subscription<geometry_msgs::msg::PointStamped>(
        "/clicked_point", 
        10, 
        std::bind(&ArmController::onClickedPoint, this, std::placeholders::_1)
    );
    target_pose_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
        "/arm_target_pose", 
        10, 
        std::bind(&ArmController::onTargetPose, this, std::placeholders::_1)
    );
    RCLCPP_INFO(node_->get_logger(), "Listening for clicked points...");
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

void ArmController::onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg){
    if(is_moving_){
        RCLCPP_INFO(node_->get_logger(), "Already moving, ignoring click");
        return;
    }
    is_moving_ = true; 
    RCLCPP_INFO(node_->get_logger(), "Recieved Point: x=%.2f y=%.2f z=%.2f",
                    msg->point.x, msg->point.y, msg->point.z);
    moveToPose(msg->point.x, msg->point.y, msg->point.z);
    is_moving_ = false;
}

void ArmController::onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    if (is_moving_) {
        RCLCPP_INFO(node_->get_logger(), "Already moving, ignoring");
        return;
    }
    is_moving_ = true;
    RCLCPP_INFO(node_->get_logger(), "GUI target: x=%.2f y=%.2f z=%.2f",
        msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    moveToPose(msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    is_moving_ = false;
}