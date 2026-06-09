#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <geometry_msgs/msg/pose.hpp>

int main(int argc, char ** argv){
    // Node setup 
    rclcpp::init(argc, argv);
    std::vector<geometry_msgs::msg::Pose> waypoints;
    moveit_msgs::msg::RobotTrajectory trajectory;
    moveit_msgs::msg::MoveItErrorCodes error_code;
    auto node = rclcpp::Node::make_shared("cartesian_demo");
    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor->add_node(node);
    auto spinner = std::thread([&executor]() {
        executor->spin();
    });

    // Connecting arm
    moveit::planning_interface::MoveGroupInterface move_group(node, "panda_arm");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    //Adding the obstacle
    moveit_msgs::msg::CollisionObject box;
    box.header.frame_id = "panda_link0";
    box.id = "demo_obstacle";

    shape_msgs::msg::SolidPrimitive primitive;
    primitive.type = shape_msgs::msg::SolidPrimitive::BOX;
    primitive.dimensions = {
        0.1,
        0.3,
        0.3
    };

    geometry_msgs::msg::Pose box_pose;
    box_pose.position.x = 0.2;
    box_pose.position.y = 0.2;
    box_pose.position.z = 0.25;
    box_pose.orientation.w = 1.0;

    box.primitives.push_back(primitive);
    box.primitive_poses.push_back(box_pose);
    box.operation = moveit_msgs::msg::CollisionObject::ADD;

    planning_scene_interface.applyCollisionObject(box);

    geometry_msgs::msg::Pose wp1;
    wp1.orientation.w = 1.0;
    wp1.position.x = 0.1;
    wp1.position.y = 0.4;
    wp1.position.z = 0.4;
    geometry_msgs::msg::Pose wp2;
    wp2.orientation.w = 1.0;
    wp2.position.x = 0.3;
    wp2.position.y = 0.0;
    wp2.position.z = 0.25;

    waypoints.push_back(wp1);
    waypoints.push_back(wp2);

    // calling the cartesian
    double fraction = move_group.computeCartesianPath(
        waypoints,
        0.01,
        0.0,
        trajectory,
        true,
        &error_code
    );

    // Reporting the stats
    RCLCPP_INFO(node->get_logger(), "Succesfully move %.2f of the map", fraction * 100.0);
    if(fraction < 0.99){
        RCLCPP_WARN(node->get_logger(), "Blocked at %.1f%%", fraction * 100.0);
    }
    move_group.execute(trajectory);
    executor->cancel();
    spinner.join();
    rclcpp::shutdown();
    return 0;
}
