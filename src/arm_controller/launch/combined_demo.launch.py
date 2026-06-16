import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    moveit_config = (
        MoveItConfigsBuilder("moveit_resources_panda")
        .robot_description(file_path="config/panda.urdf.xacro")
        .robot_description_semantic(file_path="config/panda.srdf")
        .planning_scene_monitor(
            publish_robot_description=True, publish_robot_description_semantic=True
        )
        .sensors_3d(file_path="config/sensors_3d.yaml")
        .planning_pipelines(pipelines=["ompl"])
        .trajectory_execution(file_path="config/gripper_moveit_controllers.yaml")
        .to_moveit_configs()
    )

    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[moveit_config.to_dict()],
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.planning_pipelines,
            moveit_config.robot_description_kinematics,
            moveit_config.joint_limits,
        ],
    )

    static_tf_node = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher",
        output="log",
        arguments=["0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "world", "panda_link0"],
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="both",
        parameters=[moveit_config.robot_description],
    )

    ros2_controllers_path = os.path.join(
        get_package_share_directory("moveit_resources_panda_moveit_config"),
        "config",
        "ros2_controllers.yaml",
    )

    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[ros2_controllers_path],
        remappings=[("/controller_manager/robot_description", "/robot_description")],
        output="screen",
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"],
    )

    panda_arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["panda_arm_controller", "-c", "/controller_manager"],
    )

    panda_hand_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["panda_hand_controller", "-c", "/controller_manager"],
    )

    fake_pointcloud_node = Node(
        package="arm_controller",
        executable="fake_pointcloud.py",
        output="screen",
    )

    arm_lifecycle_node = Node(
        package="arm_controller",
        executable="arm_lifecycle_node",
        output="screen",
        parameters=[moveit_config.to_dict()],
    )

    return LaunchDescription([
        static_tf_node,
        robot_state_publisher,
        move_group_node,
        rviz_node,
        ros2_control_node,
        joint_state_broadcaster_spawner,
        panda_arm_controller_spawner,
        panda_hand_controller_spawner,
        fake_pointcloud_node,
        arm_lifecycle_node,
    ])