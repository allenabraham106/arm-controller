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
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>


class ArmController{
    public: 
        explicit ArmController(const rclcpp::Node::SharedPtr & node);
        /**
        * @brief Initializes the MoveIt planning interface and prepares the arm for movement
        * @return true if initialization succeeded, false otherwise
        */
        bool initialize(); 
        /**
        * @brief Moves the arm end-effector to the specified position
        * @param x Target position in meters along the x-axis
        * @param y Target position in meters along the y-axis
        * @param z Target position in meters along the z-axis
        * @return true if motion completed successfully, false otherwise
        */
        bool moveToPose(const geometry_msgs::msg::Pose & target_pose);
        /**
        * @brief Stops all arm motion immediately
        */
        void stop();
        /**
        * @brief Determines if the arm is moving and deals with extra clicks 
        * @param msg The target pose message containing the desired end effector position
        */
        void onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

    
    private:
        rclcpp::Node::SharedPtr node_; // shared pointer for a ros2 node 
        rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr target_pose_sub_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_; // shared pointer to the MoveGroupInterface
        std::atomic<bool> is_moving_{false}; // sets the declaration of the arm moving state to either true or false without being corrupted
        std::vector<geometry_msgs::msg::Pose> waypoints_;
        rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr execute_waypoints_sub_;
        rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr clear_waypoints_sub_;
        rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
        rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr remove_waypoint_sub_;
        moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr add_waypoint_sub_;
        void onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClearWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
        bool executeWaypoints(); 
        void clearWaypoints();
        void publishWaypointMarkers();
        void onRemoveWaypoint(const std_msgs::msg::Int32::SharedPtr msg);
        void addCollisionbox();
        void onAddWaypoint(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
        double workspace_x_max_;
        double workspace_y_max_;
        double workspace_z_min_;
        double workspace_z_max_;
};