#include "arm_controller/arm_controller.hpp"
#include <moveit_msgs/msg/robot_trajectory.hpp>

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){

}

bool ArmController::initialize(){
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_, "panda_arm"
    ); 

    // Subscribes to clicked points from RViz "publish point" tool 
    // Use case: manual point selection in RViz
    clicked_point_sub_ = node_->create_subscription<geometry_msgs::msg::PointStamped>(
        "/clicked_point", 
        10, 
        std::bind(&ArmController::onClickedPoint, this, std::placeholders::_1)
    );

    // Subscribers to slider inputs from the GUI inputs
    // Use case: precise control of position and orientation from the GUI 
    target_pose_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
        "/arm_target_pose", 
        10, 
        std::bind(&ArmController::onTargetPose, this, std::placeholders::_1)
    );

    // Subscriber that listen for the execute waypoint input
    execute_waypoints_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
        "/execute_waypoints",
        10,
        std::bind(&ArmController::onExecuteWaypoints, this, std::placeholders::_1)
    );

    // Subscriber that listens for the clear waypoint input
    clear_waypoints_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
        "/clear_waypoints",
        10, 
        std::bind(&ArmController::onClearWaypoints, this, std::placeholders::_1)
    );

    RCLCPP_INFO(node_->get_logger(), "Listening for clicked points...");
    RCLCPP_INFO(node_->get_logger(), "ArmController Initialized");
    return true; 
}

bool ArmController::moveToPose(double x, double y, double z){
    is_moving_ = true;
    geometry_msgs::msg::Pose target_pose; 
    target_pose.position.x = x;
    target_pose.position.y = y;
    target_pose.position.z = z; 
    target_pose.orientation.w = 1.0;

    // Set the target
    move_group_->setPoseTarget(target_pose); 
    
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    // Plan is populated by reference to the setPoseTarget. Using plan() occupies plan with the trajectories
    auto plan_result = move_group_->plan(plan);
    if(plan_result != moveit::core::MoveItErrorCode::SUCCESS){
        RCLCPP_ERROR(node_->get_logger(), "Planning with failed node: %d", plan_result.val);
        is_moving_ = false;
        return false;
    }

    auto exec_result = move_group_->execute(plan);
    if(exec_result != moveit::core::MoveItErrorCode::SUCCESS){
        RCLCPP_ERROR(node_->get_logger(), "Execution failed with node: %d", exec_result.val);
        is_moving_ = false;
        return false;
    }
    is_moving_ = false; 
    return true;
}

void ArmController::stop(){
    move_group_->stop();
    RCLCPP_INFO(node_->get_logger(), "ArmController Stopped");
}

void ArmController::onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg){
    geometry_msgs::msg::Pose pose; 
    pose.position.x = msg->point.x;
    pose.position.y = msg->point.y; 
    pose.position.z = msg->point.z;
    pose.orientation.w = 1.0;
    waypoints_.push_back(pose);
    RCLCPP_INFO(node_->get_logger(), "Waypoint %zu added => x: %.2f, y: %.2f, z: %.2f",
        waypoints_.size(), msg->point.x, msg->point.y, msg->point.z
    );
}

void ArmController::onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    if (is_moving_) {
        RCLCPP_INFO(node_->get_logger(), "Already moving, ignoring");
        return;
    }
    RCLCPP_INFO(node_->get_logger(), "GUI target: x=%.2f y=%.2f z=%.2f",
        msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    moveToPose(msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
}

// using humble cartesian planning (will not build successfully if on jazzy or newer)
bool ArmController::executeWaypoints(){
    if(waypoints_.empty()){
        RCLCPP_WARN(node_->get_logger(), "No waypoints to execute");
        return false;
    }
    RCLCPP_INFO(node_->get_logger(), "Executing %zu waypoints", waypoints_.size());
    moveit_msgs::msg::RobotTrajectory trajectory;
    moveit_msgs::msg::MoveItErrorCodes error_code;
    double fraction = move_group_->computeCartesianPath(
        waypoints_, //waypoints
        0.01,
        0.0,
        trajectory,
        true,
        &error_code
    );
    RCLCPP_INFO(node_->get_logger(), "Cartesian Plan Summary");
    RCLCPP_INFO(node_->get_logger(), "Path Coverage: %.0f", fraction * 100);
    RCLCPP_INFO(node_->get_logger(), "Trajectory waypoints: %zu", trajectory.joint_trajectory.points.size());
    RCLCPP_INFO(node_->get_logger(), "Joints: %zu", trajectory.joint_trajectory.joint_names.size());
    for(const auto & name : trajectory.joint_trajectory.joint_names){
        RCLCPP_INFO(node_->get_logger(), "  %s", name.c_str());
    }
    RCLCPP_INFO(node_->get_logger(), "--- Start State ---");
    auto current_state = move_group_->getCurrentState();
    std::vector<double> joint_values;
    current_state->copyJointGroupPositions(
    current_state->getJointModelGroup("panda_arm"), joint_values);
    auto joint_names = move_group_->getJointNames();
    for(size_t i = 0; i < joint_names.size(); i++){
        RCLCPP_INFO(node_->get_logger(), "  %s: %.4f rad", 
            joint_names[i].c_str(), joint_values[i]);
    }
    if(fraction < 0.9){
        RCLCPP_WARN(node_->get_logger(), "Only %.0f%% of path planned", fraction * 100.0);
    }
    move_group_->execute(trajectory);
    clearWaypoints();
    return true;
}

void ArmController::clearWaypoints(){
    waypoints_.clear();
    RCLCPP_INFO(node_->get_logger(), "Waypoints cleared");
}

void ArmController::onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr){
    executeWaypoints();
}

void ArmController::onClearWaypoints(const std_msgs::msg::Empty::SharedPtr){
    clearWaypoints();
}