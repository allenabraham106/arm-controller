import os
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
from launch.actions import ExecuteProcess, IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Panda URDF 
    panda_config = get_package_share_directory("moveit_resources_panda_moveit_config")
    xacro_file = os.path.join(panda_config, 'config', 'panda.urdf.xacro')
    robot_description_raw = xacro.process_file(xacro_file).toxml()

    robot_description = {'robot_description': robot_description_raw}

    # Robot State Publisher 
    robot_state_publisher = Node(
        package = 'robot_state_publisher', 
        executable = 'robot_state_publisher', 
        output = 'screen',
        parameters = [robot_description]
    )

    # Launch Gazebo
    gazebo = ExecuteProcess(
        cmd = ['gazebo', '--verbose', '-s', 'libgazebo_ros_factory.so'],
        output = 'screen'
    )

    # Spawn Robot 
    spawn_robot = Node(
        package = 'gazebo_ros', 
        executable = 'spawn_entity.py',
        arguments = ['-topic', 'robot_description', '-entity', 'panda'],
        output = 'screen'
    )

    # Load joint state broadcaster
    load_joint_state_broadcaster = ExecuteProcess(
        cmd = ['ros2', 'control', 'load_controller', '--set-state', 'active', 'joint_state_broadcaster'], 
        output = 'screen'
    )

    # Load arm controller
    load_arm_controller = ExecuteProcess(
        cmd = ['ros2', 'control', 'load_controller', '--set-state', 'active', 'panda_arm_controller'], 
        output = 'screen'
    )

    # Start controllers after robots are spawned in
    start_joint_state_broadcaster = RegisterEventHandler(
        event_handler = OnProcessExit(
            target_action = spawn_robot,
            on_exit = [load_joint_state_broadcaster]
        )
    )

    start_arm_controller = RegisterEventHandler(
        event_handler = OnProcessExit(
            target_action = load_joint_state_broadcaster,
            on_exit = [load_arm_controller]
        )
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        spawn_robot,
        start_joint_state_broadcaster, 
        start_arm_controller
    ])