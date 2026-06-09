#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
import struct 
import math


class FakePointCloudPublisher(Node):
    def __init__(self):
        super().__init__('fake_pointcloud_publisher')
        self.publisher = self.create_publisher(
            PointCloud2, 
            '/camera/depth/points', 10 
        )
        self.timer = self.create_timer(1.0, self.publish_cloud)
        self.get_logger().info("Publishing fake point cloud data to /camera/depth/points")

    def publish_cloud(self):
        points = [

        ]
        for y in [i * 0.02 for i in range(-10, 10)]:
            for z in [i * 0.02 for i in range (5,25)]: 
                points.append((0.5, y, z))

        header = Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'world'

        fields = [
            PointField(name = 'x', offset = 0, datatype=PointField.FLOAT32, count = 1),
            PointField(name = 'y', offset = 4, datatype=PointField.FLOAT32, count = 1),
            PointField(name = 'z', offset = 8, datatype=PointField.FLOAT32, count = 1),
        ]

        point_step = 12
        data = bytearray()
        for (x, y, z) in points:
            data += struct.pack('fff', x, y, z)
        
        cloud = PointCloud2()
        cloud.header = header
        cloud.height = 1
        cloud.width = len(points)
        cloud.fields = fields
        cloud.is_bigendian = False
        cloud.point_step = point_step
        cloud.row_step = point_step * len(points)
        cloud.data = bytes(data)
        cloud.is_dense = True
        self.publisher.publish(cloud)
        self.get_logger().info(f"Published {len(points)} points")

def main(args = None):
    rclpy.init(args=args)
    node = FakePointCloudPublisher()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()