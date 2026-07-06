#include "arm_controller/waypoint_manager_node.hpp"
#include <stdexcept>

WaypointManagerNode::WaypointManagerNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("waypoint_manager_node", options)
{
    // Declare params
    declare_parameter("waypoint_markers_topic", "/waypoint_markers");
    declare_parameter("waypoint_status_topic", "/waypoint_status");
    declare_parameter("gui_command_topic", "/gui_command");
    declare_parameter("waypoints_topic", "/waypoints");
    declare_parameter("marker_line_width", 0.01);
    declare_parameter("marker_sphere_size", 0.05);
    declare_parameter("marker_line_r", 0.0);
    declare_parameter("marker_line_g", 1.0);
    declare_parameter("marker_line_b", 0.0);
    declare_parameter("marker_sphere_r", 1.0);
    declare_parameter("marker_sphere_g", 0.0);
    declare_parameter("marker_sphere_b", 0.0);
    declare_parameter("marker_alpha", 1.0);

    // Get params
    std::string waypoint_markers_topic = get_parameter("waypoint_markers_topic").as_string();
    std::string waypoint_status_topic = get_parameter("waypoint_status_topic").as_string();
    std::string waypoints_topic = get_parameter("waypoints_topic").as_string();
    std::string gui_command_topic = get_parameter("gui_command_topic").as_string();
    marker_line_width_ = get_parameter("marker_line_width").as_double();
    marker_sphere_size_ = get_parameter("marker_sphere_size").as_double();
    marker_line_r_ = get_parameter("marker_line_r").as_double();
    marker_line_g_ = get_parameter("marker_line_g").as_double();
    marker_line_b_ = get_parameter("marker_line_b").as_double();
    marker_sphere_r_ = get_parameter("marker_sphere_r").as_double();
    marker_sphere_g_ = get_parameter("marker_sphere_g").as_double();
    marker_sphere_b_ = get_parameter("marker_sphere_b").as_double();
    marker_alpha_ = get_parameter("marker_alpha").as_double();

    // Subscriptions
    gui_command_sub_ = create_subscription<arm_controller::msg::GUICommand>(
        gui_command_topic, 10, 
        std::bind(&WaypointManagerNode::onGUICommand, this, std::placeholders::_1)
    );

    // Publishers
    marker_pub_ = create_publisher<visualization_msgs::msg::MarkerArray>(
        waypoint_markers_topic, 10
    );
    waypoint_status_pub_ = create_publisher<arm_controller::msg::WaypointStatus>(
        waypoint_status_topic, 10
    );
    waypoints_pub_ = create_publisher<geometry_msgs::msg::PoseArray>(
        waypoints_topic, 10
    );

    RCLCPP_INFO(get_logger(), "WaypointManagerNode initialized");
}

void WaypointManagerNode::onGUICommand(const arm_controller::msg::GUICommand::SharedPtr msg){
    if(msg->type == arm_controller::msg::GUICommand::CLICK_WAYPOINT){
         geometry_msgs::msg::Pose pose;
        pose.position.x = msg->pose.position.x;
        pose.position.y = msg->pose.position.y;
        pose.position.z = msg->pose.position.z;
        pose.orientation.w = 1.0;
        waypoints_.push_back(pose);
        RCLCPP_INFO(get_logger(), "Waypoint %zu added => x: %.2f, y: %.2f, z: %.2f",
            waypoints_.size(), msg->pose.position.x, msg->pose.position.y, msg->pose.position.z
        );
        publishWaypointMarkers();
    }else if(msg->type == arm_controller::msg::GUICommand::EXECUTE_WAYPOINTS){
        publishWaypoints();
    }else if(msg->type == arm_controller::msg::GUICommand::CLEAR_WAYPOINTS){
        clearAllWaypoints();
    } else if(msg->type == arm_controller::msg::GUICommand::REMOVE_WAYPOINT){
        int index = msg->index;
        arm_controller::msg::WaypointStatus status;
        if(is_executing_){
            status.result = arm_controller::msg::WaypointStatus::CURRENTLY_EXECUTING;
        } else if(index >= 0 && index < (int)waypoints_.size()){
            waypoints_.erase(waypoints_.begin() + index);
            publishWaypointMarkers();
            status.result = arm_controller::msg::WaypointStatus::SUCCESS;
        } else {
            status.result = arm_controller::msg::WaypointStatus::OUT_OF_RANGE;
        }
        waypoint_status_pub_->publish(status);
    }
}

void WaypointManagerNode::publishWaypointMarkers(){
    visualization_msgs::msg::MarkerArray marker_array;
    visualization_msgs::msg::Marker line;
    line.header.frame_id = "panda_link0";
    line.header.stamp = now();
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
        sphere.header.stamp = now();
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

void WaypointManagerNode::clearAllWaypoints(){
    visualization_msgs::msg::MarkerArray clear_markers;
    visualization_msgs::msg::Marker delete_marker;
    delete_marker.action = visualization_msgs::msg::Marker::DELETEALL;
    clear_markers.markers.push_back(delete_marker);
    marker_pub_->publish(clear_markers);
    waypoints_.clear();
    RCLCPP_INFO(get_logger(), "Waypoints cleared");
}

void WaypointManagerNode::publishWaypoints(){
    geometry_msgs::msg::PoseArray pose_array;
    pose_array.header.stamp = now();
    pose_array.header.frame_id = "panda_link0";
    pose_array.poses = waypoints_;
    waypoints_pub_->publish(pose_array);
    RCLCPP_INFO(get_logger(), "Published %zu waypoints for execution", waypoints_.size());
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(WaypointManagerNode)