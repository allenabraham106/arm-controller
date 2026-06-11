#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <memory> 
#include <thread>
#include "arm_controller/arm_controller.hpp"

class ArmLifeCycleNode : public rclcpp_lifecycle::LifecycleNode{
    public:
        ArmLifeCycleNode() : rclcpp_lifecycle::LifecycleNode("arm_lifecycle_node") {}
    
    private:
        std::shared_ptr<ArmController> arm_; // smart pointer
        rclcpp::Node::SharedPtr node_ptr_; // shared pointer
        std::thread spin_thread_;

        // Configuring the arm. This prepares the arm to move not to actually move
        CallbackReturn on_configure(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Configuring...");
            node_ptr_ = std::make_shared<rclcpp::Node>("arm_inner");
            arm_=std::make_shared<ArmController>(node_ptr_);
            // checks if the arm is initialized and what to do if it so not
            if(!arm_->initialize()){
                RCLCPP_ERROR(get_logger(), "ArmController Failed to Initialize");
                return CallbackReturn::FAILURE;
            }
            spin_thread_ = std::thread([this](){
                rclcpp::spin(node_ptr_);
            });
            return CallbackReturn::SUCCESS;
        }

        // Activate the arm and move to position. We start with the first position
        CallbackReturn on_activate(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Activating...");
            // checks if the arm is at the initial pose we set
            geometry_msgs::msg::Pose initial_pose;
            initial_pose.position.x = 0.3;
            initial_pose.position.y = 0.0;
            initial_pose.position.z = 0.4;
            initial_pose.orientation.w = 1.0;
            if(!arm_->safeMoveToPose(initial_pose)){
                RCLCPP_ERROR(get_logger(), "Failed to move to initial pose");
                return CallbackReturn::FAILURE;
            }
            return CallbackReturn::SUCCESS;
        }

        // Deactivating the arm so it stops
        CallbackReturn on_deactivate(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Deactivating...");
            arm_->stop();
            return CallbackReturn::SUCCESS;
        }

        // Shutting down the arm
        CallbackReturn on_shutdown(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Shuting down...");
            rclcpp::shutdown();
            if(spin_thread_.joinable()){
                spin_thread_.join();
            }
            arm_.reset();
            return CallbackReturn::SUCCESS;
        }
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ArmLifeCycleNode>();
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node->get_node_base_interface());
    executor.spin();
    rclcpp::shutdown();
    return 0;
}