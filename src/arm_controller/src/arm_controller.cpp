#include "arm_controller/arm_controller.hpp"
#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <geometry_msgs/msg/pose.hpp>

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

    // Publsiher that is mapping out our waypoints
    marker_pub_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>(
        "/waypoint_markers", 10
    );

    // Subscriber that listens for points to remove
    remove_waypoint_sub_ = node_->create_subscription<std_msgs::msg::Int32>(
        "/remove_waypoint",
        10, 
        std::bind(&ArmController::onRemoveWaypoint, this, std::placeholders::_1)
    );

    RCLCPP_INFO(node_->get_logger(), "Listening for clicked points...");
    RCLCPP_INFO(node_->get_logger(), "ArmController Initialized");
    return true; 
}

bool ArmController::moveToPose(const geometry_msgs::msg::Pose & target_pose){
    waypoints_.clear();
    waypoints_.push_back(target_pose);
    return executeWaypoints();
}

void ArmController::stop(){
    move_group_->stop();
    RCLCPP_INFO(node_->get_logger(), "ArmController Stopped");
}

void ArmController::onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg){
    if(std::abs(msg->point.x) > 0.85 || 
       std::abs(msg->point.y) > 0.85 ||
       msg->point.z < 0.0 || msg->point.z > 1.2){
        RCLCPP_WARN(node_->get_logger(), "Point out of workspace bounds, ignoring");
        return;
    }
    geometry_msgs::msg::Pose pose; 
    pose.position.x = msg->point.x;
    pose.position.y = msg->point.y; 
    pose.position.z = msg->point.z;
    pose.orientation.w = 1.0;
    waypoints_.push_back(pose);
    RCLCPP_INFO(node_->get_logger(), "Waypoint %zu added => x: %.2f, y: %.2f, z: %.2f",
        waypoints_.size(), msg->point.x, msg->point.y, msg->point.z
    );
    publishWaypointMarkers();
}

void ArmController::onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    if (is_moving_) {
        RCLCPP_INFO(node_->get_logger(), "Already moving, ignoring");
        return;
    }
    RCLCPP_INFO(node_->get_logger(), "GUI target: x=%.2f y=%.2f z=%.2f",
        msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    moveToPose(msg->pose);
}

// using humble cartesian planning (will not build successfully if on jazzy or newer)
bool ArmController::executeWaypoints(){
    if(waypoints_.empty()){
        RCLCPP_WARN(node_->get_logger(), "No waypoints to execute");
        return false;
    }

    RCLCPP_INFO(node_->get_logger(), "Executing %zu waypoints with OMPL", waypoints_.size());

    for (size_t i = 0; i < waypoints_.size(); ++i){
        move_group_->setPoseTarget(waypoints_[i]);
        moveit::planning_interface::MoveGroupInterface::Plan plan; 
        bool success = (move_group_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if(!success){
            RCLCPP_WARN(node_->get_logger(), "Failed to reach specified point %zu", i);
            return false;
        }
        RCLCPP_INFO(node_->get_logger(), "Executing Waypoint");
        move_group_->execute(plan);
    }
    clearWaypoints();
    return true;
}

void ArmController::clearWaypoints(){
    visualization_msgs::msg::MarkerArray clear_markers;
    visualization_msgs::msg::Marker delete_marker;
    delete_marker.action = visualization_msgs::msg::Marker::DELETEALL;
    clear_markers.markers.push_back(delete_marker);
    marker_pub_->publish(clear_markers);
    waypoints_.clear();
    RCLCPP_INFO(node_->get_logger(), "Waypoints cleared");
}
void ArmController::onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr){
    executeWaypoints();
}

void ArmController::onClearWaypoints(const std_msgs::msg::Empty::SharedPtr){
    clearWaypoints();
}

void ArmController::publishWaypointMarkers(){
    visualization_msgs::msg::MarkerArray marker_array;
    visualization_msgs::msg::Marker line;
    line.header.frame_id = "panda_link0";
    line.header.stamp = node_->now();
    line.ns = "waypoints";
    line.id = 0; 
    line.type = visualization_msgs::msg::Marker::LINE_STRIP;
    line.action = visualization_msgs::msg::Marker::ADD;
    line.scale.x = 0.01;
    line.color.r = 0.00;
    line.color.g = 1.00;
    line.color.b = 0.00;
    line.color.a = 1.00;
    for(size_t i = 0; i < waypoints_.size(); ++i){
        line.points.push_back(waypoints_[i].position);
        visualization_msgs::msg::Marker sphere;
        sphere.header.frame_id = "panda_link0";
        sphere.header.stamp = node_->now();
        sphere.ns = "waypoints";
        sphere.id = i + 1;
        sphere.type = visualization_msgs::msg::Marker::SPHERE;
        sphere.action = visualization_msgs::msg::Marker::ADD;
        sphere.pose = waypoints_[i];
        sphere.scale.x = 0.05;
        sphere.scale.y = 0.05;
        sphere.scale.z = 0.05;
        sphere.color.r = 1.0;
        sphere.color.g = 0.0;
        sphere.color.b = 0.0;
        sphere.color.a = 1.0;
        marker_array.markers.push_back(sphere);
    }
    marker_array.markers.push_back(line);
    marker_pub_->publish(marker_array);
}

 void ArmController::onRemoveWaypoint(const std_msgs::msg::Int32::SharedPtr msg){
    int index = msg->data; 
    if(index < 0 || index >= (int)waypoints_.size()){
        RCLCPP_WARN(node_->get_logger(), "Invalid index %d, please select from valid index up to %zu", index, waypoints_.size());
        return;
    }
    waypoints_.erase(waypoints_.begin() + index);
    RCLCPP_INFO(node_->get_logger(), "Removing waypoint %d, there are %zu waypoints left", index, waypoints_.size());
    publishWaypointMarkers();
}