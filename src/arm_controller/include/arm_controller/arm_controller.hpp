#pragma once 

#include <rclcpp/rclcpp.hpp>
// only using .h rather than .hpp for CI compatibility. For jazzy and newer .hpp is fine
#include <moveit/move_group_interface/move_group_interface.h> 
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/empty.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <std_msgs/msg/int32.hpp>
#include <memory>
#include <atomic>
#include <vector>

class ArmController{
    public: 
        explicit ArmController(const rclcpp::Node::SharedPtr & node) {};

        /**
         * @brief Initializes the MoveIt planning interface and prepares the arm for movement
         * @return true if initialization succeeded, false otherwise
         */
        bool initialize(); 

        /**
        * @brief Stops all arm motion immediately
        */
        void stop();
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
        bool safeMoveToPose(const geometry_msgs::msg::Pose & target_pose);

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
        rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr remove_waypoint_sub_;
        
        // sets the declaration of the arm moving state to either true or false without being corrupted
        std::atomic<bool> is_moving_{false};
        std::vector<geometry_msgs::msg::Pose> waypoints_;

        void onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClearWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
        bool executeWaypoints(); 
        void clearAllWaypoints();
        void publishWaypointMarkers();
        void onRemoveWaypoint(const std_msgs::msg::Int32::SharedPtr msg);
        bool isInWorkspace( const geometry_msgs::msg::PointStamped::SharedPtr msg);
        bool moveToPose(const geometry_msgs::msg::Pose & target_pose);

        static constexpr double WORKSPACE_LIMIT_XY = 0.85;
        static constexpr double WORKSPACE_LIMIT_Z_MIN = 0.0;
        static constexpr double WORKSPACE_LIMIT_Z_MAX = 1.2;
        static constexpr double MARKER_LINE_WIDTH = 0.01;
        static constexpr double MARKER_SPHERE_SIZE = 0.05;
        static constexpr double MARKER_LINE_R = 0.0;
        static constexpr double MARKER_LINE_B = 0.0;
        static constexpr double MARKER_LINE_G = 1.0;
        static constexpr double MARKER_SPHERE_G = 0.0;
        static constexpr double MARKER_SPHERE_B = 0.0;
        static constexpr double MARKER_ALPHA = 1.0;
        static constexpr double MARKER_SPHERE_R = 1.0;
};