#!/usr/bin/env python3
import rclpy 
from rclpy.node import Node
from std_msgs.msg import Empty
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QLabel
from PyQt5.QtCore import Qt
from sensor_msgs.msg import JointState
import tf2_ros
import geometry_msgs.msg
import sys
import threading

class ArmGUI(Node):
    def __init__(self):
        super().__init__("arm_gui")
        self.execute_pub = self.create_publisher(Empty, "/execute_waypoints", 10)
        self.clear_pub = self.create_publisher(Empty, "/clear_waypoints", 10)
        self.is_executing = False
        self.joint_sub = self.create_subscription(
            JointState, 
            "/joint_states",
            self.on_joint_states,
            10
        )
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)
    
    def execute(self):
        self.execute_pub.publish(Empty())
        self.is_executing = True
        self.get_logger().info("Executing Waypoints")
        self.get_logger().info("Status: Getting to waypoints")
    
    def clear(self):
        self.clear_pub.publish(Empty())
        self.is_executing = False
        self.get_logger().info("Clear waypoints triggered")

    def on_joint_states(self, msg):
        if not self.is_executing:
            return
        self.get_logger().info("Joint States")
        for name, pos in zip(msg.name, msg.position):
            self.get_logger().info(f"{name}: {pos:.3f} rad")
        try: 
            t = self.tf_buffer.lookup_transform(
                "panda_link0",
                "panda_link8",
                rclpy.time.Time()
            )
            p = t.transform.translation
            self.get_logger().info(
                f"End Effector: x = {p.x:.3f} y = {p.y:.3f} z = {p.z:.3f}"
            )
        except Exception:
            pass
    


class MainWindow(QWidget):
    def __init__(self, node):
        super().__init__()
        self.node = node
        self.setWindowTitle("Arm Controller")
        self.setFixedSize(300, 200)
        layout = QVBoxLayout()
        title = QLabel("Waypoint Control")
        title.setAlignment(Qt.AlignCenter)
        layout.addWidget(title)

        execute_btn = QPushButton("Execute Button")
        execute_btn.setStyleSheet("background-color: green; color: white; height: 50px;")
        execute_btn.clicked.connect(self.node.execute)
        layout.addWidget(execute_btn)

        clear_btn = QPushButton("Clear Button")
        clear_btn.setStyleSheet("background-color: red; color: white; height: 50px;")
        clear_btn.clicked.connect(self.node.clear)
        layout.addWidget(clear_btn)

        self.setLayout(layout)



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