#!/usr/bin/env python3

import rclpy 
from rclpy.node import Node
from std_msgs.msg import Empty
from geometry_msgs.msg import PoseStamped
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QHBoxLayout,
                              QPushButton, QLabel, QDoubleSpinBox, QComboBox,
                              QListWidget, QSlider, QCheckBox)
from PyQt5.QtCore import Qt
from PyQt5.QtCore import QTimer
from sensor_msgs.msg import JointState
import tf2_ros
import sys
import threading
from std_msgs.msg import Empty, Float64
import math
from std_msgs.msg import Empty, Float64, Bool

class ArmGUI(Node):
    def __init__(self):
        super().__init__("arm_gui")
        self.execute_pub = self.create_publisher(Empty, "/execute_waypoints", 10)
        self.clear_pub = self.create_publisher(Empty, "/clear_waypoints", 10)
        self.pose_pub = self.create_publisher(PoseStamped, "/arm_target_pose", 10)
        self.waypoint_pub = self.create_publisher(PoseStamped, "/gui_add_waypoint", 10)
        self.auto_orientation_pub = self.create_publisher(
            Bool, '/auto_orientation', 10
        )
        self.is_executing = False
        self.joint_sub = self.create_subscription(
            JointState, 
            "/joint_states",
            self.on_joint_states,
            10
        )
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)
        self.current_distance = 0.0
        self.distance_sub = self.create_subscription(
            Float64, '/distance_to_obstacle', self.on_distance, 10
        )
        self.declare_parameter('collision_box.x', 0.5)
        self.declare_parameter('collision_box.y', 0.0)
        self.declare_parameter('collision_box.z', 0.1)
        self.declare_parameter('collision_box.size_x', 0.1)
        self.declare_parameter('collision_box.size_y', 0.4)
        self.declare_parameter('collision_box.size_z', 0.4)
    
    def execute(self):
        self.execute_pub.publish(Empty())
        self.is_executing = True
        self.get_logger().info("Executing Waypoints")

    def clear(self):
        self.clear_pub.publish(Empty())
        self.is_executing = False
        self.get_logger().info("Clear waypoints triggered")

    def send_pose(self, x, y, z, roll, pitch, yaw):
        msg = PoseStamped()
        msg.header.frame_id = "panda_link0"
        msg.pose.position.x = x
        msg.pose.position.y = y
        msg.pose.position.z = z
        qx, qy, qz, qw = self.euler_to_quaternion(roll, pitch, yaw)
        msg.pose.orientation.x = qx
        msg.pose.orientation.y = qy
        msg.pose.orientation.z = qz
        msg.pose.orientation.w = qw
        self.pose_pub.publish(msg)
        self.get_logger().info(f"Sending pose: x={x:.2f} y={y:.2f} z={z:.2f} r={roll:.1f} p={pitch:.1f} y={yaw:.1f}")

    def on_joint_states(self, msg):
        if not self.is_executing:
            return
        try: 
            t = self.tf_buffer.lookup_transform(
                "panda_link0",
                "panda_link8",
                rclpy.time.Time()
            )
            p = t.transform.translation
            self.get_logger().info(
                f"End Effector: x={p.x:.3f} y={p.y:.3f} z={p.z:.3f}"
            )
        except Exception:
            pass

    def send_waypoint(self, x, y, z, roll, pitch, yaw):
        msg = PoseStamped()
        msg.header.frame_id = "panda_link0"
        msg.pose.position.x = x
        msg.pose.position.y = y
        msg.pose.position.z = z
        qx, qy, qz, qw = self.euler_to_quaternion(roll, pitch, yaw)
        msg.pose.orientation.x = qx
        msg.pose.orientation.y = qy
        msg.pose.orientation.z = qz
        msg.pose.orientation.w = qw
        self.waypoint_pub.publish(msg)
        self.get_logger().info(f"Waypoint queued: x={x:.2f} y={y:.2f} z={z:.2f}")

    def on_distance(self, msg):
        self.current_distance = msg.data
    
    def euler_to_quaternion(self, roll_deg, pitch_deg, yaw_deg):
        r = math.radians(roll_deg)
        p = math.radians(pitch_deg)
        y = math.radians(yaw_deg)
        
        cr = math.cos(r/2); sr = math.sin(r/2)
        cp = math.cos(p/2); sp = math.sin(p/2)
        cy = math.cos(y/2); sy = math.sin(y/2)
        
        qw = cr*cp*cy + sr*sp*sy
        qx = sr*cp*cy - cr*sp*sy
        qy = cr*sp*cy + sr*cp*sy
        qz = cr*cp*sy - sr*sp*cy
        return qx, qy, qz, qw

    def set_auto_orientation(self, enabled):
        msg = Bool()
        msg.data = enabled
        self.auto_orientation_pub.publish(msg)
        self.get_logger().info(f"Auto orientation: {enabled}")

class MainWindow(QWidget):
    def __init__(self, node):
        super().__init__()
        self.node = node
        self.setWindowTitle("Arm Controller")
        self.setFixedSize(350, 820)
        layout = QVBoxLayout()

        title = QLabel("Arm Controller")
        title.setAlignment(Qt.AlignCenter)
        layout.addWidget(title)

        # XYZ spinboxes
        for label, attr, default in [("X", "x_spin", 0.3), ("Y", "y_spin", 0.0), ("Z", "z_spin", 0.4)]:
            row = QHBoxLayout()
            row.addWidget(QLabel(f"{label}:"))
            spin = QDoubleSpinBox()
            spin.setRange(-0.85, 0.85)
            spin.setSingleStep(0.05)
            spin.setValue(default)
            spin.setDecimals(2)
            setattr(self, attr, spin)
            row.addWidget(spin)
            layout.addLayout(row)

        # Orientation sliders (roll, pitch, yaw in degrees)
        orient_title = QLabel("Orientation (degrees):")
        orient_title.setAlignment(Qt.AlignCenter)
        layout.addWidget(orient_title)

        for label, attr in [("Roll", "roll_spin"), ("Pitch", "pitch_spin"), ("Yaw", "yaw_spin")]:
            row = QHBoxLayout()
            row.addWidget(QLabel(f"{label}:"))
            spin = QDoubleSpinBox()
            spin.setRange(-180.0, 180.0)
            spin.setSingleStep(5.0)
            spin.setValue(0.0)
            spin.setDecimals(1)
            spin.setSuffix("°")
            setattr(self, attr, spin)
            row.addWidget(spin)
            layout.addLayout(row)

        # Send pose button
        send_btn = QPushButton("Send Pose")
        send_btn.setStyleSheet("background-color: #2196F3; color: white; height: 40px;")
        send_btn.clicked.connect(self.on_send_pose)
        layout.addWidget(send_btn)

        # Home button
        home_btn = QPushButton("Home (0.3, 0.0, 0.4)")
        home_btn.setStyleSheet("background-color: #9C27B0; color: white; height: 40px;")
        home_btn.clicked.connect(self.on_home)
        layout.addWidget(home_btn)

        # Execute/Clear
        execute_btn = QPushButton("Execute Waypoints")
        execute_btn.setStyleSheet("background-color: green; color: white; height: 40px;")
        execute_btn.clicked.connect(self.node.execute)
        layout.addWidget(execute_btn)

        clear_btn = QPushButton("Clear Waypoints")
        clear_btn.setStyleSheet("background-color: red; color: white; height: 40px;")
        clear_btn.clicked.connect(lambda: [self.node.clear(), 
            self.waypoint_list.clear()])
        layout.addWidget(clear_btn)

        # Add waypoint button
        add_waypoint_btn = QPushButton("Add Waypoint")
        add_waypoint_btn.setStyleSheet("background-color: #FF9800; color: white; height: 40px;")
        add_waypoint_btn.clicked.connect(self.on_add_waypoint)
        layout.addWidget(add_waypoint_btn)

        # Waypoint list
        self.waypoint_list = QListWidget()
        self.waypoint_list.setFixedHeight(120)
        self.waypoint_list.setStyleSheet("background-color: #2a2a2a; color: white;")
        layout.addWidget(self.waypoint_list)

        # Obstacle info
        obstacle_title = QLabel("── Obstacle Info ──")
        obstacle_title.setAlignment(Qt.AlignCenter)
        obstacle_title.setStyleSheet("color: #888; font-size: 11px;")
        layout.addWidget(obstacle_title)

        box_x = node.get_parameter('collision_box.x').value
        box_y = node.get_parameter('collision_box.y').value
        box_z = node.get_parameter('collision_box.z').value
        size_x = node.get_parameter('collision_box.size_x').value
        size_y = node.get_parameter('collision_box.size_y').value
        size_z = node.get_parameter('collision_box.size_z').value

        pos_label = QLabel(f"Pos: ({box_x:.2f}, {box_y:.2f}, {box_z:.2f})m")
        pos_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(pos_label)

        size_label = QLabel(f"Size: {size_x:.2f} x {size_y:.2f} x {size_z:.2f}m")
        size_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(size_label)

        self.distance_label = QLabel("Distance to EE: --m")
        self.distance_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.distance_label)

        self.update_timer = QTimer()
        self.update_timer.timeout.connect(self.update_distance_label)
        self.update_timer.start(500)

        self.auto_orient_check = QCheckBox("Auto Orientation (let OMPL decide)")
        self.auto_orient_check.setStyleSheet("color: black;")
        self.auto_orient_check.stateChanged.connect(
            lambda state: self.node.set_auto_orientation(state == 2)
        )
        layout.addWidget(self.auto_orient_check)

        self.setLayout(layout)

    def on_send_pose(self):
        self.node.send_pose(
            self.x_spin.value(),
            self.y_spin.value(),
            self.z_spin.value(),
            self.roll_spin.value(),
            self.pitch_spin.value(),
            self.yaw_spin.value()
        )

    def on_home(self):
        self.x_spin.setValue(0.3)
        self.y_spin.setValue(0.0)
        self.z_spin.setValue(0.4)
        self.roll_spin.setValue(0.0)
        self.pitch_spin.setValue(0.0)
        self.yaw_spin.setValue(0.0)
        self.node.send_pose(0.3, 0.0, 0.4, 0.0, 0.0, 0.0)

    def on_add_waypoint(self):
        x = self.x_spin.value()
        y = self.y_spin.value()
        z = self.z_spin.value()
        r = self.roll_spin.value()
        p = self.pitch_spin.value()
        yaw = self.yaw_spin.value()
        self.node.send_waypoint(x, y, z, r, p, yaw)
        self.waypoint_list.addItem(
            f"WP{self.waypoint_list.count() + 1}: "
            f"pos=({x:.2f}, {y:.2f}, {z:.2f})m "
            f"rpy=({r:.0f}°, {p:.0f}°, {yaw:.0f}°)"
        )
    
    def update_distance_label(self):
        d = self.node.current_distance
        color = "green" if d > 0.3 else "orange" if d > 0.15 else "red"
        self.distance_label.setStyleSheet(
            f"color: {color}; font-size: 12px; font-weight: bold;"
        )
        self.distance_label.setText(f"Distance to EE: {d:.3f}m")

def main():
    rclpy.init()
    node = ArmGUI()
    spin_thread = threading.Thread(target=rclpy.spin, args=(node,), daemon=True)
    spin_thread.start()

    app = QApplication(sys.argv)
    window = MainWindow(node)
    window.show()
    app.exec_()
    rclpy.shutdown()

if __name__ == "__main__":
    main()