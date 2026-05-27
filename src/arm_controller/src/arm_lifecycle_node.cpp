#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <memory> 
#include <thread>
#include "arm_controller/arm_controller.hpp"

class ArmLifeCycleNode : public rclcpp_lifecycle::LifecycleNode{
    public:
        ArmLifeCycleNode() : rclcpp_lifecycle::LifecycleNode("arm_lifecycle_node"){

        }
    
    private:
        std::shared_ptr<ArmController> arm_; // smart pointer
        rclcpp::Node::SharedPtr node_ptr_; // shared pointer
        std::thread spin_thread_;

        CallbackReturn on_configure(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Configuring...");
            node_ptr_ = std::make_shared<rclcpp::Node>("arm_inner");
            arm_=std::make_shared<ArmController>(node_ptr_);
            arm_->initialize();
            spin_thread_ = std::thread([this](){
                rclcpp::spin(node_ptr_);
            });
            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_activate(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Activating...");
            arm_->moveToPose(0.3, 0.0, 0.4);
            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_deactivate(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Deactivating...");
            arm_->stop();
            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_shutdown(const rclcpp_lifecycle::State &){
            RCLCPP_INFO(get_logger(), "Shuting down...");
            arm_.reset();
            rclcpp::shutdown();
            if(spin_thread_.joinable()){
                spin_thread_.join();
            }
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