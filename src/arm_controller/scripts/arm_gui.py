#!/usr/bin/env python3

import rclpy
import threading
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped 
import tkinter as tk

class ArmGUI(Node):
    def __init__(self):
        super().__init__("arm_gui")
        self.publisher = self.create_publisher(PoseStamped, "/arm_target_pose", 10)
        self.window = tk.Tk()
        self.window.title("Arm Controller")
        self.window.geometry("400x400")

        # x slider 
        tk.Label(self.window, text="X (m)").pack()
        self.x_slider = tk.Scale(self.window, from_=-1.0, to=1.0, resolution=0.01, orient=tk.HORIZONTAL, length=300)
        self.x_slider.pack()

        # y slider
        tk.Label(self.window, text="Y (m)").pack()
        self.y_slider = tk.Scale(self.window, from_=-1.0, to=1.0, resolution=0.01, orient=tk.HORIZONTAL, length=300)
        self.y_slider.pack()

        # z slider
        tk.Label(self.window, text="Z (m)").pack()
        self.z_slider = tk.Scale(self.window, from_=0.0, to=1.2, resolution=0.01, orient=tk.HORIZONTAL, length=300)
        self.z_slider.set(0.4)
        self.z_slider.pack()

        # move button
        tk.Button(self.window, text="Move Arm", command=self.publish_pose, height=2, width=20).pack(pady=20)

    def publish_pose(self):
        msg=PoseStamped()
        msg.header.frame_id = "panda_link0"
        msg.pose.position.x = self.x_slider.get()
        msg.pose.position.y = self.y_slider.get()
        msg.pose.position.z = self.z_slider.get()
        msg.pose.orientation.w  = 1.0
        self.publisher.publish(msg)
        self.get_logger().info(f"Sent pose: x={msg.pose.position.x:.2f} y={msg.pose.position.y:.2f} z={msg.pose.position.z:.2f}")

def main():
    rclpy.init()
    node = ArmGUI()
    
    # spin ROS2 in background thread
    spin_thread = threading.Thread(target=rclpy.spin, args=(node,), daemon=True)
    spin_thread.start()
    
    # run GUI on main thread
    node.window.mainloop()
    
    rclpy.shutdown()

if __name__ == "__main__":
    main()


       