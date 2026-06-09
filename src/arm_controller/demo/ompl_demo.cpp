#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <geometry_msgs/msg/pose.hpp>

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("ompl_demo");
    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor->add_node(node);
    auto spinner = std::thread([&executor]() {
        executor->spin();
    });

    moveit::planning_interface::MoveGroupInterface move_group(node, "panda_arm");
    move_group.setPlanningTime(15.0);
    move_group.setPoseReferenceFrame("panda_link0");

    rclcpp::sleep_for(std::chrono::seconds(2));

    //move_group.setNamedTarget("ready");
    //auto ready_result = move_group.move();
    //RCLCPP_INFO(node->get_logger(), "Move to ready result: %d", ready_result.val);

    geometry_msgs::msg::Pose goal;
    goal.orientation.y = 0.8;
    goal.orientation.w = 0.6;
    goal.position.x = 0.1;
    goal.position.y = 0.4;
    goal.position.z = 0.4;
    move_group.setPoseTarget(goal);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    auto result = move_group.plan(plan);

    if(result == moveit::core::MoveItErrorCode::SUCCESS){
        RCLCPP_INFO(node->get_logger(), "Plan succeeded, executing...");
        move_group.execute(plan);
    } else {
        RCLCPP_ERROR(node->get_logger(), "Plan failed");
    }

    executor->cancel();
    spinner.join();
    rclcpp::shutdown();
    return 0;
}