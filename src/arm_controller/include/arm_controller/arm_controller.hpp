#pragma once 
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <memory>
#include <atomic>

class ArmController{
    public: 
        explicit ArmController(const rclcpp::Node::SharedPtr & node);
        bool initialize(); 
        bool moveToPose(double x, double y, double z);
        void stop();
        // Shared ptr so that we are able to access the Pose that we are currently on
        void onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

    
    private:
        rclcpp::Node::SharedPtr node_; // shared pointer for a ros2 node 
        rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr target_pose_sub_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_; // shared pointer to the MoveGroupInterface
        std::atomic<bool> is_moving_{false}; // sets the declaration of the arm moving state to either true or false without being corrupted

        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
};