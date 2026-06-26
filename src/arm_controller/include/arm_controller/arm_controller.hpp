#pragma once 

#include <rclcpp/rclcpp.hpp>
// only using .h rather than .hpp for CI compatibility. For jazzy and newer .hpp is fine
#include <moveit/move_group_interface/move_group_interface.h> 
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/empty.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include "arm_controller/msg/waypoint_command.hpp"
#include "arm_controller/msg/waypoint_status.hpp"
#include <memory>
#include <atomic>
#include <vector>

class ArmController{
    public: 
        explicit ArmController(const rclcpp::Node::SharedPtr & node);

        /**
        * @brief Determines if the arm is moving and deals with extra clicks 
        * @param msg The target pose message containing the desired end effector position
        */
        void onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
        /**
        * @brief Safely moves to a pose, rejecting the call if waypoints are already queued
        * @param target_pose The target end-effector pose
        * @return true if motion completed successfully, false if waypoints are queued or motion fails
        */
        bool moveToPose(const geometry_msgs::msg::Pose & target_pose);

        /**
         * @brief Stops all arm motion immediately
         */
        void stop();
    
    private:
        rclcpp::Node::SharedPtr node_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;

        rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr target_pose_sub_;
        rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr execute_waypoints_sub_;
        rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr clear_waypoints_sub_;
        rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
        rclcpp::Subscription<arm_controller::msg::WaypointCommand>::SharedPtr waypoint_command_sub_;
        rclcpp::Publisher<arm_controller::msg::WaypointStatus>::SharedPtr waypoint_status_pub_;

        // sets the declaration of the arm moving state to either true or false without being corrupted
        std::atomic<bool> is_moving_{false};
        std::vector<geometry_msgs::msg::Pose> waypoints_;
        std::string move_group_name_;

        void onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClearWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
        bool executeWaypoints(); 
        void clearAllWaypoints();
        void publishWaypointMarkers();
        void onWaypointCommand(const arm_controller::msg::WaypointCommand::SharedPtr msg);
        bool executeMove(const geometry_msgs::msg::Pose & target_pose);

        double workspace_limit_xy_;
        double workspace_limit_z_min_;
        double workspace_limit_z_max_;
        double marker_line_width_;
        double marker_sphere_size_;
        double marker_line_r_;
        double marker_line_g_;
        double marker_line_b_;
        double marker_sphere_r_;
        double marker_sphere_g_;
        double marker_sphere_b_;
        double marker_alpha_;
};