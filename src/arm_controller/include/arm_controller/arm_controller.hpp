#pragma once 
#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <memory>

class ArmController{
    public: 
        explicit ArmContoller(const rclcpp::Node::SharedPtr & node);
        bool initialize(); 
        bool moveToPose(double x, double y, double z);
        bool stop();
    
    private:
        rclcpp::Node::SharedPtr node_; // shared pointer for a ros2 node 
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_; // shared pointer to the MoveGroupInterface
}