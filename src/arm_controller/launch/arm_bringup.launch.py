from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('arm_controller')
    params_file = os.path.join(pkg_share, 'config', 'arm_params.yaml')

    # Component container that runs all nodes in one process
    container = ComposableNodeContainer(
        name='arm_controller_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='arm_controller',
                plugin='WaypointManagerNode',
                name='waypoint_manager_node',
                parameters=[params_file]
            ),
            ComposableNode(
                package='arm_controller',
                plugin='ArmPlannerNode',
                name='arm_planner_node',
                parameters=[params_file]
            ),
            ComposableNode(
                package='arm_controller',
                plugin='ArmMonitorNode',
                name='arm_monitor_node',
                parameters=[params_file]
            ),
        ],
        output='screen'
    )

    return LaunchDescription([container])