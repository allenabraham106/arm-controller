#include "arm_controller/arm_controller.hpp"

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){

}

// initiuallize function so create the subscriptions and shared pointers. and beigin listening for points
bool ArmController::initialize(){
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_,"panda_arm"
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

// moveToPose takes a target pose and sets the position to that and moves it.
// Requires : x, y, z
bool ArmController::moveToPose(double x, double y, double z){
    // setting pose
    geometry_msgs::msg::Pose target_pose; 
    target_pose.position.x = x;
    target_pose.position.y = y;
    target_pose.position.z = z; 
    target_pose.orientation.w = 1.0;

    // moving to pose
    move_group_->setPoseTarget(target_pose); 
    

    // checks if we are successful or not
    //planning
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    auto plan_result = move_group_->plan(plan);
    if(plan_result != moveit::core::MoveItErrorCode::SUCCESS){
        RCLCPP_ERROR(logger_, "Planning with failed node: %d", plan_result.value)
        return false;
    }

    //execute
    auto exec_result = move_group_->execute(plan);
    if(exec_result != moveit::core::MoveItErrorCode::SUCCESS){
        RCLCPP_ERROR(logger_, "Execution failed with node: %d", exec_result.value);
        return false;
    }
    return true
}

// Stops the arm in the current position
void ArmController::stop(){
    move_group_->stop();
    RCLCPP_INFO(node_->get_logger(), "ArmController Stopped");
}

// Checks if the arm is moving and handles the clicks
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

// Checks if the the arm is moving and if so it ignores other requests
void ArmController::onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
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