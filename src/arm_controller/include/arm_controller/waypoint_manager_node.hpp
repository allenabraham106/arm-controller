#pragma once

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/empty.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include "arm_controller/msg/waypoint_command.hpp"
#include "arm_controller/msg/waypoint_status.hpp"
#include <memory>
#include <atomic>
#include <vector>

class WaypointManagerNode : public rclcpp::Node {
public:
    explicit WaypointManagerNode(const rclcpp::NodeOptions & options);

private:
    // Subscriptions
    rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_;
    rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr execute_waypoints_sub_;
    rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr clear_waypoints_sub_;
    rclcpp::Subscription<arm_controller::msg::WaypointCommand>::SharedPtr waypoint_command_sub_;

    // Publishers
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
    rclcpp::Publisher<arm_controller::msg::WaypointStatus>::SharedPtr waypoint_status_pub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr waypoints_pub_;

    // State
    std::vector<geometry_msgs::msg::Pose> waypoints_;
    std::atomic<bool> is_executing_{false};

    // Marker params
    double marker_line_width_;
    double marker_sphere_size_;
    double marker_line_r_;
    double marker_line_g_;
    double marker_line_b_;
    double marker_sphere_r_;
    double marker_sphere_g_;
    double marker_sphere_b_;
    double marker_alpha_;

    // Callbacks
    void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
    void onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
    void onClearWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
    void onWaypointCommand(const arm_controller::msg::WaypointCommand::SharedPtr msg);

    // Helpers
    void publishWaypointMarkers();
    void clearAllWaypoints();
    void publishWaypoints();
};