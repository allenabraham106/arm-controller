#include "arm_controller/arm_controller.hpp"
#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <cmath>

ArmController::ArmController(const rclcpp::Node::SharedPtr & node) : node_(node){

}

bool ArmController::initialize(){
    // Declare parameters
    node_->declare_parameter("planning_group", std::string("panda_arm"));
    node_->declare_parameter("planning_time", 15.0);
    node_->declare_parameter("workspace.x_max", 0.855);
    node_->declare_parameter("workspace.y_max", 0.855);
    node_->declare_parameter("workspace.z_min", 0.0);
    node_->declare_parameter("workspace.z_max", 1.2);
    node_->declare_parameter("collision_box.x", 0.5);
    node_->declare_parameter("collision_box.y", 0.0);
    node_->declare_parameter("collision_box.z", 0.1);
    node_->declare_parameter("collision_box.size_x", 0.1);
    node_->declare_parameter("collision_box.size_y", 0.4);
    node_->declare_parameter("collision_box.size_z", 0.4);

    // Get parameters
    std::string planning_group = node_->get_parameter("planning_group").as_string();
    double planning_time = node_->get_parameter("planning_time").as_double();
    workspace_x_max_ = node_->get_parameter("workspace.x_max").as_double();
    workspace_y_max_ = node_->get_parameter("workspace.y_max").as_double();
    workspace_z_min_ = node_->get_parameter("workspace.z_min").as_double();
    workspace_z_max_ = node_->get_parameter("workspace.z_max").as_double();

    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        node_, planning_group
    );
    move_group_->setPlanningTime(planning_time);
    /* move_group_->setWorkspace(
        -workspace_x_max_, -workspace_y_max_, workspace_z_min_,
        workspace_x_max_,  workspace_y_max_, workspace_z_max_
    ); */

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

    // Subscriber that listens for the addWaypoint from our gui
    add_waypoint_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
        "/gui_add_waypoint",
        10, 
        std::bind(&ArmController::onAddWaypoint, this, std::placeholders::_1)
    );

    // publisher that publishes distance to the objects infront of us
    distance_pub_ = node_->create_publisher<std_msgs::msg::Float64>(
        "/distance_to_obstacle", 10
    );
    distance_timer_ = node_->create_wall_timer(
        std::chrono::milliseconds(500),
        std::bind(&ArmController::publishDistanceToBox, this)
    );

    // Subscriber that listens to whent he gui for our orientation is toggled on or off
    auto_orientation_sub_ = node_->create_subscription<std_msgs::msg::Bool>(
        "/auto_orientation",
        10,
        [this](const std_msgs::msg::Bool::SharedPtr msg){
            auto_orientation_ = msg->data;
            RCLCPP_INFO(node_->get_logger(), "Auto orientation: %s", 
                auto_orientation_ ? "ON" : "OFF");
        }
    );

    // Subscriber for updating a specific waypoint from GUI
    update_waypoint_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
        "/gui_update_waypoint",
        10,
        std::bind(&ArmController::onUpdateWaypoint, this, std::placeholders::_1)
    );

    // Subscriber for triggering path preview
    preview_waypoints_sub_ = node_->create_subscription<std_msgs::msg::Empty>(
        "/preview_waypoints",
        10,
        [this](const std_msgs::msg::Empty::SharedPtr){
            onPreviewWaypoints();
        }
    );

    // Publisher for displaying planned path in RViz
    preview_pub_ = node_->create_publisher<moveit_msgs::msg::DisplayTrajectory>(
        "/move_group/display_planned_path", 10
    );

    RCLCPP_INFO(node_->get_logger(), "Listening for clicked points...");
    RCLCPP_INFO(node_->get_logger(), "ArmController Initialized");

    addCollisionbox();
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
    if(std::abs(msg->point.x) > workspace_x_max_ || 
        std::abs(msg->point.y) > workspace_y_max_ ||
        msg->point.z < workspace_z_min_ || 
        msg->point.z > workspace_z_max_){
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
    is_moving_ = true;
    moveToPose(msg->pose);
}

bool ArmController::executeWaypoints(){
    if(waypoints_.empty()){
        RCLCPP_WARN(node_->get_logger(), "No waypoints to execute");
        return false;
    }

    RCLCPP_INFO(node_->get_logger(), "Executing %zu waypoints with OMPL", waypoints_.size());

    for (size_t i = 0; i < waypoints_.size(); ++i){
        if(auto_orientation_){
            move_group_->setPositionTarget(
                waypoints_[i].position.x,
                waypoints_[i].position.y,
                waypoints_[i].position.z
            );
        } else {
            move_group_->setPoseTarget(waypoints_[i]);
        }
        moveit::planning_interface::MoveGroupInterface::Plan plan; 
        bool success = (move_group_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if(!success){
            RCLCPP_WARN(node_->get_logger(), "Failed to reach specified point %zu", i);
            is_moving_ = false;
            return false;
        }
        RCLCPP_INFO(node_->get_logger(), "Executing Waypoint");
        move_group_->execute(plan);
    }
    clearWaypoints();
    is_moving_ = false;
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

void ArmController::addCollisionbox(){
    moveit_msgs::msg::CollisionObject block; 
    block.header.frame_id = "panda_link0";
    block.id = "obstacle_block";

    shape_msgs::msg::SolidPrimitive primitive;
    primitive.type = primitive.BOX;
    double box_size_x = node_->get_parameter("collision_box.size_x").as_double();
    double box_size_y = node_->get_parameter("collision_box.size_y").as_double();
    double box_size_z = node_->get_parameter("collision_box.size_z").as_double();
    primitive.dimensions = {box_size_x, box_size_y, box_size_z};

    geometry_msgs::msg::Pose block_pose;
    block_pose.position.x = node_->get_parameter("collision_box.x").as_double();
    block_pose.position.y = node_->get_parameter("collision_box.y").as_double();
    block_pose.position.z = node_->get_parameter("collision_box.z").as_double();

    block.primitives.push_back(primitive);
    block.primitive_poses.push_back(block_pose);
    block.operation = block.ADD;

    planning_scene_interface_.applyCollisionObject(block);
    RCLCPP_INFO(node_->get_logger(), "Added collision block to planning scene");
}

void ArmController::onAddWaypoint(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    geometry_msgs::msg::Pose pose = msg->pose;
    waypoints_.push_back(pose);
    RCLCPP_INFO(node_->get_logger(), "GUI waypoint %zu added => x: %.2f, y: %.2f, z: %.2f", waypoints_.size(), pose.position.x, pose.position.y, pose.position.z);
    publishWaypointMarkers();
}

void ArmController::publishDistanceToBox(){
    double box_x = node_->get_parameter("collision_box.x").as_double();
    double box_y = node_->get_parameter("collision_box.y").as_double();
    double box_z = node_->get_parameter("collision_box.z").as_double();

    geometry_msgs::msg::PoseStamped ee_pose = move_group_->getCurrentPose();

    double dx = ee_pose.pose.position.x - box_x;
    double dy = ee_pose.pose.position.y - box_y;
    double dz = ee_pose.pose.position.z - box_z;
    double distance = std::sqrt(dx*dx + dy*dy + dz*dz);

    std_msgs::msg::Float64 msg;
    msg.data = distance;
    distance_pub_->publish(msg);
}

void ArmController::onUpdateWaypoint(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    int index = std::stoi(msg->header.frame_id);
    if(index < 0 || index >= (int)waypoints_.size()){
        RCLCPP_WARN(node_->get_logger(), "Update waypoint: invalid index %d", index);
        return;
    }
    waypoints_[index] = msg->pose;
    RCLCPP_INFO(node_->get_logger(), "Updated waypoint %d", index);
    publishWaypointMarkers();
}

void ArmController::onPreviewWaypoints(){
    if(waypoints_.empty()){
        RCLCPP_WARN(node_->get_logger(), "No waypoints to preview");
        return;
    }
    RCLCPP_INFO(node_->get_logger(), "Previewing %zu waypoints", waypoints_.size());

    moveit_msgs::msg::DisplayTrajectory display_trajectory;
    display_trajectory.model_id = move_group_->getName();

    for(size_t i = 0; i < waypoints_.size(); ++i){
        if(auto_orientation_){
            move_group_->setPositionTarget(
                waypoints_[i].position.x,
                waypoints_[i].position.y,
                waypoints_[i].position.z
            );
        } else {
            move_group_->setPoseTarget(waypoints_[i]);
        }
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        bool success = (move_group_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
        if(success){
            display_trajectory.trajectory.push_back(plan.trajectory);
            move_group_->setStartState(*move_group_->getCurrentState());
        } else {
            RCLCPP_WARN(node_->get_logger(), "Preview: failed to plan waypoint %zu", i);
        }
    }
    move_group_->setStartStateToCurrentState();
    preview_pub_->publish(display_trajectory);
    RCLCPP_INFO(node_->get_logger(), "Preview published");
}