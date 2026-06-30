#pragma once

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/empty.hpp>
#include <std_msgs/msg/bool.hpp>
#include <memory>
#include <atomic>
#include <vector>

class ArmPlannerNode : public rclcpp::Node {
public:
    explicit ArmPlannerNode(const rclcpp::NodeOptions & options);

private:
    // MoveIt
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;

    // Subscriptions
    rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr waypoints_sub_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr target_pose_sub_;

    // State
    std::atomic<bool> is_executing_{false};
    std::string move_group_name_;

    // Workspace params
    double workspace_limit_xy_;
    double workspace_limit_z_min_;
    double workspace_limit_z_max_;

    // Callbacks
    void onWaypoints(const geometry_msgs::msg::PoseArray::SharedPtr msg);
    void onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

    // Helpers
    bool executePlan(const std::vector<geometry_msgs::msg::Pose> & waypoints);
    bool moveToPose(const geometry_msgs::msg::Pose & target_pose);
    void addCollisionBox();
};