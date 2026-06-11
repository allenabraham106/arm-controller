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
    if(!isInWorkspace(msg)){
        RCLCPP_WARN(node_->get_logger(), "Published point is out of the workspace");
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
    safeMoveToPose(msg->pose);
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
        waypoints_,
        0.01,
        0.0,
        trajectory,
        true,
        &error_code
    );

    RCLCPP_INFO(node_->get_logger(), "Cartesian Plan Summary");
    RCLCPP_INFO(node_->get_logger(), "Path Coverage: %.0f%%", fraction * 100.0);
    RCLCPP_INFO(node_->get_logger(), "Trajectory waypoints: %zu", trajectory.joint_trajectory.points.size());
    RCLCPP_INFO(node_->get_logger(), "Joints: %zu", trajectory.joint_trajectory.joint_names.size());
    for(const auto & name : trajectory.joint_trajectory.joint_names){
        RCLCPP_INFO(node_->get_logger(), "  %s", name.c_str());
    }

    if(fraction < 0.9){
        RCLCPP_WARN(node_->get_logger(), "Only %.0f%% of path planned", fraction * 100.0);
        return false;
    }

    move_group_->execute(trajectory);
    clearAllWaypoints();
    return true;
}

void ArmController::clearAllWaypoints(){
    // publish a DELETEALL marker to remove all markers from RViz before clearing the waypoint list
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
    clearAllWaypoints();
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
    line.scale.x = MARKER_LINE_WIDTH;
    line.color.r = MARKER_LINE_R;
    line.color.g = MARKER_LINE_G;
    line.color.b = MARKER_LINE_B;
    line.color.a = MARKER_ALPHA;
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
        sphere.scale.x = MARKER_SPHERE_SIZE;
        sphere.scale.y = MARKER_SPHERE_SIZE;
        sphere.scale.z = MARKER_SPHERE_SIZE;
        sphere.color.r = MARKER_SPHERE_R;
        sphere.color.g = MARKER_SPHERE_G;
        sphere.color.b = MARKER_SPHERE_B;
        sphere.color.a = MARKER_ALPHA;
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

bool ArmController::safeMoveToPose(const geometry_msgs::msg::Pose & target_pose){
    if(!waypoints_.empty()){
        RCLCPP_ERROR(node_->get_logger(), "Cannot call moveToPose with %zu waypoints queued. Call clearAllWaypoints() first", waypoints_.size());
        return false;
    }
    return moveToPose(target_pose);
}

bool ArmController::isInWorkspace(const geometry_msgs::msg::PointStamped::SharedPtr msg){
    return  std::abs(msg->point.x) <= WORKSPACE_LIMIT_XY &&
            std::abs(msg->point.y) <= WORKSPACE_LIMIT_XY &&
            msg->point.z >= WORKSPACE_LIMIT_Z_MIN &&
            msg->point.z <= WORKSPACE_LIMIT_Z_MAX;
}