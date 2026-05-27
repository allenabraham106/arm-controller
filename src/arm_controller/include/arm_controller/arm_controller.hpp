#pragma once 
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <memory>
#include <atomic>

class ArmController{
    public: 
        explicit ArmController(const rclcpp::Node::SharedPtr & node);
        bool initialize(); 
        bool moveToPose(double x, double y, double z);
        bool stop();
    
    private:
        rclcpp::Node::SharedPtr node_; // shared pointer for a ros2 node 
        rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_; // shared pointer to the MoveGroupInterface
        std::atomic<bool> is_moving_(false);

        void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg);
};