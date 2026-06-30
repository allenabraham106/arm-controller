#include "arm_controller/arm_monitor_node.hpp"
#include <cmath>

ArmMonitorNode::ArmMonitorNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("arm_monitor_node", options)
{
    // Declare params
    declare_parameter("joint_state_topic", "/joint_states");
    declare_parameter("is_executing_topic", "/is_executing");
    declare_parameter("distance_topic", "/distance_to_box");
    declare_parameter("joint_state_out_topic", "/arm_joint_states");
    declare_parameter("near_limit_topic", "/near_joint_limit");
    declare_parameter("joint_upper_limits", std::vector<double>{2.8973, 1.7628, 2.8973, -0.0698, 2.8973, 3.7525, 2.8973});
    declare_parameter("joint_lower_limits", std::vector<double>{-2.8973, -1.7628, -2.8973, -3.0718, -2.8973, -0.0175, -2.8973});
    declare_parameter("monitor_timer_ms", 100);
    declare_parameter("box_x", 0.5);
    declare_parameter("box_y", 0.0);
    declare_parameter("box_z", 0.15);
    declare_parameter("joint_limit_warning_threshold", 0.1);

    // Get params
    std::string joint_state_topic = get_parameter("joint_state_topic").as_string();
    std::string is_executing_topic = get_parameter("is_executing_topic").as_string();
    std::string distance_topic = get_parameter("distance_topic").as_string();
    std::string joint_state_out_topic = get_parameter("joint_state_out_topic").as_string();
    std::string near_limit_topic = get_parameter("near_limit_topic").as_string();
    int monitor_timer_ms = get_parameter("monitor_timer_ms").as_int();
    box_x_ = get_parameter("box_x").as_double();
    box_y_ = get_parameter("box_y").as_double();
    box_z_ = get_parameter("box_z").as_double();
    joint_limit_warning_threshold_ = get_parameter("joint_limit_warning_threshold").as_double();
    joint_upper_limits_ = get_parameter("joint_upper_limits").as_double_array();
    joint_lower_limits_ = get_parameter("joint_lower_limits").as_double_array();

    // Subscriptions
    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
        joint_state_topic, 10,
        std::bind(&ArmMonitorNode::onJointState, this, std::placeholders::_1)
    );
    is_executing_sub_ = create_subscription<std_msgs::msg::Bool>(
        is_executing_topic, 10,
        std::bind(&ArmMonitorNode::onIsExecuting, this, std::placeholders::_1)
    );

    // Publishers
    distance_pub_ = create_publisher<std_msgs::msg::Float64>(distance_topic, 10);
    joint_state_pub_ = create_publisher<sensor_msgs::msg::JointState>(joint_state_out_topic, 10);
    near_limit_pub_ = create_publisher<std_msgs::msg::Bool>(near_limit_topic, 10);

    // Timer
    monitor_timer_ = create_wall_timer(
        std::chrono::milliseconds(monitor_timer_ms),
        std::bind(&ArmMonitorNode::onMonitorTimer, this)
    );

    RCLCPP_INFO(get_logger(), "ArmMonitorNode initialized");
}

void ArmMonitorNode::onJointState(const sensor_msgs::msg::JointState::SharedPtr msg){
    latest_joint_state_ = *msg;
    joint_state_pub_->publish(latest_joint_state_);
}

void ArmMonitorNode::onIsExecuting(const std_msgs::msg::Bool::SharedPtr msg){
    is_executing_ = msg->data;
}

void ArmMonitorNode::onMonitorTimer(){
    publishDistanceToBox();
    checkJointLimits();
}

void ArmMonitorNode::publishDistanceToBox(){
    if(latest_joint_state_.position.empty()){
        return;
    }
    // Use the last known end effector position from joint states
    // For now publish a placeholder — full FK would require a TF listener
    std_msgs::msg::Float64 dist_msg;
    dist_msg.data = std::sqrt(
        std::pow(box_x_, 2) +
        std::pow(box_y_, 2) +
        std::pow(box_z_, 2)
    );
    distance_pub_->publish(dist_msg);
}

void ArmMonitorNode::checkJointLimits(){
    if(latest_joint_state_.position.empty()){
        return;
    }
    // Panda joint limits (approximate, in radians)
    std::vector<double> upper_limits = {2.8973, 1.7628, 2.8973, -0.0698, 2.8973, 3.7525, 2.8973};
    std::vector<double> lower_limits = {-2.8973, -1.7628, -2.8973, -3.0718, -2.8973, -0.0175, -2.8973};

    bool near_limit = false;
    for(size_t i = 0; i < latest_joint_state_.position.size() && i < upper_limits.size(); i++){
        double pos = latest_joint_state_.position[i];
        if(pos > upper_limits[i] - joint_limit_warning_threshold_ ||
           pos < lower_limits[i] + joint_limit_warning_threshold_){
            RCLCPP_WARN(get_logger(), "Joint %zu near limit: %.4f rad", i, pos);
            near_limit = true;
        }
    }

    std_msgs::msg::Bool near_limit_msg;
    near_limit_msg.data = near_limit;
    near_limit_pub_->publish(near_limit_msg);
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ArmMonitorNode)