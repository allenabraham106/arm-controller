#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <iostream>

int main(int argc, char** argv){
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>("terminal_controller");

    auto pub = node->create_publisher<geometry_msgs::msg::PoseStamped>(
        "/arm_target_pose", 10
    );

    while(rclcpp::ok()){
        double x, y, z, roll, yaw, pitch;

        std::cout << "Enter target position and orientation\n";
        std::cout << "Range for x:(-0.85, 0.85), y:(-0.85, 0.85), z:(0.0, 1.2)\n";
        std::cout << "Range for roll, pitch, yaw (-3.14, 3.14)\n\n";
        std::cout << "X:  ";    std::cin>>x;
        std::cout << "Y:  ";    std::cin>>y;
        std::cout << "Z:  ";    std::cin>>z;
        std::cout << "Roll:  "; std::cin>>roll;
        std::cout << "Yaw:  ";  std::cin>>yaw;
        std::cout << "Pitch: "; std::cin>>pitch;

        if(!std::cin){
            break;
        }

        geometry_msgs::msg::PoseStamped msg;
        msg.header.stamp = node->now();
        msg.header.frame_id = "world";

        msg.pose.position.x = x; 
        msg.pose.position.y = y;
        msg.pose.position.z = z;

        tf2::Quaternion q; 
        q.setRPY(roll, pitch, yaw);
        msg.pose.orientation = tf2::toMsg(q);

        pub->publish(msg);
        RCLCPP_INFO(node->get_logger(),
        "Published Target: x=%.2f, y=%.2f, z=%.2f, roll=%.2f, pitch=%.2f, yaw=%.2f",
        x, y, z, roll, pitch, yaw
        );
        rclcpp::spin_some(node);
    }
    rclcpp::shutdown();
    return 0;
}