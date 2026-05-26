#!/bin/bash

echo "Building ROS2 workspace..."

# Source ROS2 environment
source /opt/ros/humble/setup.bash

# Initialize rosdep for current user if needed
if [ ! -f ~/.ros/rosdep/sources.cache ]; then
    echo "Initializing rosdep for user..."
    rosdep update
fi

# Install dependencies
rosdep install --from-paths src --ignore-src -r -y

# Build the workspace
colcon build

echo "Build completed!"
echo "To source the workspace, run: source install/setup.bash"