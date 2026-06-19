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
        .to_moveit_configs()
    )

    arm_lifecycle_node = Node(
        package="arm_controller",
        executable="arm_lifecycle_node",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            os.path.join(
                get_package_share_directory("arm_controller"),
                "config", "arm_controller_params.yaml"
            ),
        ],
    )

    return LaunchDescription([arm_lifecycle_node])