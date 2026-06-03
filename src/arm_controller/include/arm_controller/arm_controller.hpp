#pragma once 
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>           // target end effector pose
#include <geometry_msgs/msg/point_stamped.hpp>  // clicked point in RViz
#include <geometry_msgs/msg/pose_stamped.hpp>   // target pose from GUI
#include <std_msgs/msg/empty.hpp>
#include <memory>
#include <atomic>
#include <vector>

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
        bool moveToPose(double x, double y, double z);
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
        void onExecuteWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClearWaypoints(const std_msgs::msg::Empty::SharedPtr msg);
        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
        bool executeWaypoints(); 
        void clearWaypoints();
};