#include "arm_controller/arm_controller.hpp"
#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <stdexcept>

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){
    node_->declare_parameter("move_group_name", "panda_arm");
    node_->declare_parameter("clicked_point_topic", "/clicked_point");
    node_->declare_parameter("target_pose_topic", "/arm_target_pose");
    node_->declare_parameter("execute_waypoints_topic", "/execute_waypoints");
    node_->declare_parameter("clear_waypoints_topic", "/clear_waypoints");
    node_->declare_parameter("waypoint_markers_topic", "/waypoint_markers");
    node_->declare_parameter("remove_waypoint_topic", "/remove_waypoint");
    node_->declare_parameter("workspace_limit_xy", 0.85);
    node_->declare_parameter("workspace_limit_z_min", 0.0);
    node_->declare_parameter("workspace_limit_z_max", 1.2);
    node_->declare_parameter("marker_line_width", 0.01);
    node_->declare_parameter("marker_sphere_size", 0.05);
    node_->declare_parameter("marker_line_r", 0.0);
    node_->declare_parameter("marker_line_g", 1.0);
    node_->declare_parameter("marker_line_b", 0.0);
    node_->declare_parameter("marker_sphere_r", 1.0);
    node_->declare_parameter("marker_sphere_g", 0.0);
    node_->declare_parameter("marker_sphere_b", 0.0);
    node_->declare_parameter("marker_alpha", 1.0);

    std::string clicked_point_topic = node_->get_parameter("clicked_point_topic").as_string();
    std::string target_pose_topic = node_->get_parameter("target_pose_topic").as_string();
    std::string execute_waypoints_topic = node_->get_parameter("execute_waypoints_topic").as_string();
    std::string clear_waypoints_topic = node_->get_parameter("clear_waypoints_topic").as_string();
    std::string waypoint_markers_topic = node_->get_parameter("waypoint_markers_topic").as_string();
    std::string remove_waypoint_topic = node_->get_parameter("remove_waypoint_topic").as_string();
    move_group_name_ = node_->get_parameter("move_group_name").as_string();
    workspace_limit_xy_ = node_->get_parameter("workspace_limit_xy").as_double();
    workspace_limit_z_min_ = node_->get_parameter("workspace_limit_z_min").as_double();
    workspace_limit_z_max_ = node_->get_parameter("workspace_limit_z_max").as_double();
    marker_line_width_ = node_->get_parameter("marker_line_width").as_double();
    marker_sphere_size_ = node_->get_parameter("marker_sphere_size").as_double();
    marker_line_r_ = node_->get_parameter("marker_line_r").as_double();
    marker_line_g_ = node_->get_parameter("marker_line_g").as_double();
    marker_line_b_  = node_->get_parameter("marker_line_b").as_double();
    marker_sphere_r_ = node_->get_parameter("marker_sphere_r").as_double();
    marker_sphere_g_ = node_->get_parameter("marker_sphere_g").as_double();
    marker_sphere_b_ = node_->get_parameter("marker_sphere_b").as_double();
    marker_alpha_ = node_->get_parameter("marker_alpha").as_double();

    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_, move_group_name_
    );

    move_group_->setWorkspace(
        -workspace_limit_xy_, -workspace_limit_xy_, workspace_limit_z_min_,
        workspace_limit_xy_, workspace_limit_xy_, workspace_limit_z_max_
    );

    // Subscribes to clicked points from RViz "publish point" tool 
    clicked_point_sub_ = node_->create_subscription<geometry_msgs::msg::PointStamped>(
        clicked_point_topic, 10,
        std::bind(&ArmController::onClickedPoint, this, std::placeholders::_1)
    );

    // Subscribers custom pose controller interface
    target_pose_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
        target_pose_topic, 10,
        std::bind(&ArmController::onTargetPose, this, std::placeholders::_1)
    );
    
    RCLCPP_INFO(node_->get_logger(), " Start listening for clicked points...");

    // Subscriber that listen for the execute waypoint signal 
    execute_waypoints_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
        execute_waypoints_topic, 10,
        std::bind(&ArmController::onExecuteWaypoints, this, std::placeholders::_1)
    );

    // Subscriber that listens for the clear waypoint signal 
    clear_waypoints_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
        clear_waypoints_topic, 10,
        std::bind(&ArmController::onClearWaypoints, this, std::placeholders::_1)
    );

    // Publsiher that is mapping out our waypoints
    marker_pub_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>(
        waypoint_markers_topic, 10
    );

    // Subscriber that listens for points to remove
    remove_waypoint_sub_ = node_->create_subscription<std_msgs::msg::Int32>(
        remove_waypoint_topic, 10,
        std::bind(&ArmController::onRemoveWaypoint, this, std::placeholders::_1)
    );

    RCLCPP_INFO(node_->get_logger(), "Listening for clicked points...");
    RCLCPP_INFO(node_->get_logger(), "ArmController Initialized");
}

bool ArmController::executeMove(const geometry_msgs::msg::Pose & target_pose){
    // TODO: Potential Defect why clear the vector? 
    waypoints_.clear();
    waypoints_.push_back(target_pose);
    return executeWaypoints();
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

    RCLCPP_INFO(node_->get_logger(), "Executing %zu waypoints", waypoints_.size());
    moveit_msgs::msg::RobotTrajectory trajectory;
    moveit_msgs::msg::MoveItErrorCodes error_code;
    // TODO: Move all planner to it's own interface
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

    RCLCPP_INFO(node_->get_logger(), "--- Start State ---");
    auto current_state = move_group_->getCurrentState();
    std::vector<double> joint_values;
    current_state->copyJointGroupPositions(
    current_state->getJointModelGroup(move_group_name_), joint_values);
    auto joint_names = move_group_->getJointNames();
    for(size_t i = 0; i < joint_names.size(); i++){
        RCLCPP_INFO(node_->get_logger(), "  %s: %.4f rad", 
        joint_names[i].c_str(), joint_values[i]);
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
    line.scale.x = marker_line_width_;
    line.color.r = marker_line_r_;
    line.color.g = marker_line_g_;
    line.color.b = marker_line_b_;
    line.color.a = marker_alpha_;
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
        sphere.scale.x = marker_sphere_size_;
        sphere.scale.y = marker_sphere_size_;
        sphere.scale.z = marker_sphere_size_;
        sphere.color.r = marker_sphere_r_;
        sphere.color.g = marker_sphere_g_;
        sphere.color.b = marker_sphere_b_;
        sphere.color.a = marker_alpha_;
        marker_array.markers.push_back(sphere);
    }
    marker_array.markers.push_back(line);
    marker_pub_->publish(marker_array);
}

void ArmController::onRemoveWaypoint(const std_msgs::msg::Int32::SharedPtr msg){
    int index = msg->data; 
    if(index >= 0 && index < (int)waypoints_.size()){
        waypoints_.erase(waypoints_.begin() + index);
        RCLCPP_INFO(node_->get_logger(), "Removing waypoint %d, there are %zu waypoints left", index, waypoints_.size());
        publishWaypointMarkers();
    } else {
        throw std::out_of_range("Waypoint index out of range: " + std::to_string(index));
    }
}

bool ArmController::moveToPose(const geometry_msgs::msg::Pose & target_pose){
    if(!waypoints_.empty()){
        RCLCPP_ERROR(node_->get_logger(), "Cannot call moveToPose with %zu waypoints queued. Call clearAllWaypoints() first", waypoints_.size());
        return false;
    }
    return executeMove(target_pose);
}