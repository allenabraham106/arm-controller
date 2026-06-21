cmake_minimum_required(VERSION 3.8)
project(arm_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# ArmController library
add_library(arm_controller_lib
  src/arm_controller.cpp
)
target_include_directories(arm_controller_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(arm_controller_lib
  rclcpp
  moveit_ros_planning_interface
  geometry_msgs
  visualization_msgs
  std_msgs
  moveit_msgs
  shape_msgs
)

# Lifecycle node executable
add_executable(arm_lifecycle_node src/arm_lifecycle_node.cpp)
target_link_libraries(arm_lifecycle_node arm_controller_lib)
ament_target_dependencies(arm_lifecycle_node
  rclcpp
  rclcpp_lifecycle
)

install(TARGETS arm_controller_lib arm_lifecycle_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(PROGRAMS
  scripts/arm_gui.py
  scripts/fake_pointcloud.py
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()