#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/bool.hpp>
#include <memory>
#include <vector>

class ArmMonitorNode : public rclcpp::Node {
public:
    explicit ArmMonitorNode(const rclcpp::NodeOptions & options);

private:
    // Member variables
    std::vector<double> joint_upper_limits_;
    std::vector<double> joint_lower_limits_;
    
    // Subscriptions
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr is_executing_sub_;

    // Publishers
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr distance_pub_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr near_limit_pub_;

    // Timer
    rclcpp::TimerBase::SharedPtr monitor_timer_;

    // State
    sensor_msgs::msg::JointState latest_joint_state_;
    bool is_executing_{false};

    // Collision box position for distance calc
    double box_x_;
    double box_y_;
    double box_z_;

    // Joint limit params
    double joint_limit_warning_threshold_;

    // Callbacks
    void onJointState(const sensor_msgs::msg::JointState::SharedPtr msg);
    void onIsExecuting(const std_msgs::msg::Bool::SharedPtr msg);

    // Timer callback
    void onMonitorTimer();

    // Helpers
    void publishDistanceToBox();
    void checkJointLimits();
};