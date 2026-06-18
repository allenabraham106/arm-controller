import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    urdf_path = os.path.join(
        get_package_share_directory("dummy_urdf"),
        "urdf", "dummy_urdf.urdf"
    )

    moveit_config = (
        MoveItConfigsBuilder("dummy_urdf", package_name="dummy_urdf_moveit_config")
        .robot_description(file_path=urdf_path)
        .robot_description_semantic(file_path="config/dummy_urdf.srdf")
        .robot_description_kinematics(file_path="config/kinematics.yaml")
        .joint_limits(file_path="config/joint_limits.yaml")
        .planning_scene_monitor(
            publish_robot_description=True,
            publish_robot_description_semantic=True
        )
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )

    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {"planning_pipelines": ["ompl"],
            "default_planning_pipeline": "ompl",
            "ompl": {
                "planning_plugin": "ompl_interface/OMPLPlanner",
                "request_adapters": "default_planning_request_adapters/ResolveConstraintFrames default_planning_request_adapters/ValidateWorkspaceBounds default_planning_request_adapters/CheckStartStateBounds default_planning_request_adapters/CheckStartStateCollision",
                "response_adapters": "default_planning_response_adapters/AddTimeOptimalParameterization default_planning_response_adapters/ValidateSolution default_planning_response_adapters/DisplayMotionPath",
            }},
        ],
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
        arguments=["0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "world", "Link_1"],
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="both",
        parameters=[moveit_config.robot_description],
    )

    ros2_controllers_path = os.path.join(
        get_package_share_directory("dummy_urdf_moveit_config"),
        "config", "ros2_controllers.yaml",
    )

    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            moveit_config.robot_description,
            os.path.join(
                get_package_share_directory("dummy_urdf_moveit_config"),
                "config", "ros2_controllers.yaml",
            ),
        ],
        output="screen",
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager",
        ],
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "arm_controller",
            "--controller-manager",
            "/controller_manager",
        ],
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
        arm_controller_spawner,
        arm_lifecycle_node,
    ])