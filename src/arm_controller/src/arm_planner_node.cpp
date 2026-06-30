#include "arm_controller/arm_planner_node.hpp"
#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <stdexcept>

ArmPlannerNode::ArmPlannerNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("arm_planner_node", options)
{
    // Declare params
    declare_parameter("move_group_name", "panda_arm");
    declare_parameter("waypoints_topic", "/waypoints");
    declare_parameter("target_pose_topic", "/arm_target_pose");
    declare_parameter("workspace_limit_xy", 0.85);
    declare_parameter("workspace_limit_z_min", 0.0);
    declare_parameter("workspace_limit_z_max", 1.2);

    // Get params
    move_group_name_ = get_parameter("move_group_name").as_string();
    std::string waypoints_topic = get_parameter("waypoints_topic").as_string();
    std::string target_pose_topic = get_parameter("target_pose_topic").as_string();
    workspace_limit_xy_ = get_parameter("workspace_limit_xy").as_double();
    workspace_limit_z_min_ = get_parameter("workspace_limit_z_min").as_double();
    workspace_limit_z_max_ = get_parameter("workspace_limit_z_max").as_double();

    // MoveIt
    move_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
        shared_from_this(), move_group_name_
    );
    move_group_->setWorkspace(
        -workspace_limit_xy_, -workspace_limit_xy_, workspace_limit_z_min_,
        workspace_limit_xy_, workspace_limit_xy_, workspace_limit_z_max_
    );

    // Subscriptions
    waypoints_sub_ = create_subscription<geometry_msgs::msg::PoseArray>(
        waypoints_topic, 10,
        std::bind(&ArmPlannerNode::onWaypoints, this, std::placeholders::_1)
    );
    target_pose_sub_ = create_subscription<geometry_msgs::msg::PoseStamped>(
        target_pose_topic, 10,
        std::bind(&ArmPlannerNode::onTargetPose, this, std::placeholders::_1)
    );

    addCollisionBox();

    RCLCPP_INFO(get_logger(), "ArmPlannerNode initialized");
}

void ArmPlannerNode::onWaypoints(const geometry_msgs::msg::PoseArray::SharedPtr msg){
    if(is_executing_){
        RCLCPP_WARN(get_logger(), "Already executing, ignoring waypoints");
        return;
    }
    if(msg->poses.empty()){
        RCLCPP_WARN(get_logger(), "Received empty waypoint list");
        return;
    }
    executePlan(msg->poses);
}

void ArmPlannerNode::onTargetPose(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
    if(is_executing_){
        RCLCPP_WARN(get_logger(), "Already executing, ignoring target pose");
        return;
    }
    moveToPose(msg->pose);
}

bool ArmPlannerNode::executePlan(const std::vector<geometry_msgs::msg::Pose> & waypoints){
    is_executing_ = true;

    for(const auto & target_pose : waypoints){
        move_group_->setPoseTarget(target_pose);
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        bool success = (move_group_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if(!success){
            RCLCPP_ERROR(get_logger(), "OMPL planning failed for waypoint");
            is_executing_ = false;
            return false;
        }

        move_group_->execute(plan);
    }

    is_executing_ = false;
    return true;
}

bool ArmPlannerNode::moveToPose(const geometry_msgs::msg::Pose & target_pose){
    is_executing_ = true;
    move_group_->setPoseTarget(target_pose);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);
    if(success){
        move_group_->execute(plan);
    } else {
        RCLCPP_ERROR(get_logger(), "Failed to plan to target pose");
    }
    is_executing_ = false;
    return success;
}

void ArmPlannerNode::addCollisionBox(){
    moveit_msgs::msg::CollisionObject collision_object;
    collision_object.header.frame_id = move_group_->getPlanningFrame();
    collision_object.id = "collision_box";

    shape_msgs::msg::SolidPrimitive primitive;
    primitive.type = primitive.BOX;
    primitive.dimensions = {0.3, 0.3, 0.3};

    geometry_msgs::msg::Pose box_pose;
    box_pose.position.x = 0.5;
    box_pose.position.y = 0.0;
    box_pose.position.z = 0.15;
    box_pose.orientation.w = 1.0;

    collision_object.primitives.push_back(primitive);
    collision_object.primitive_poses.push_back(box_pose);
    collision_object.operation = collision_object.ADD;

    planning_scene_interface_.applyCollisionObject(collision_object);
    RCLCPP_INFO(get_logger(), "Collision box added");
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ArmPlannerNode)