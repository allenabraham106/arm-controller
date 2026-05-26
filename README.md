# ROS2 Humble Docker Demo

This is the template for setting up and using ROS2 Humble in a Docker container.
This template is required by all UWRobotics software project.

## Project Structure
```
docker_demo/
├── Dockerfile              # Docker image definition
├── docker-compose.yml      # Container orchestration
├── scripts/                # Shell scripts
│   ├── setup.sh            # Host-side: detects UID/GID, prepares .env
│   ├── docker-entrypoint.sh# Container startup script
│   └── build.sh            # Build script for ROS2 workspace
├── src/                    # ROS2 source code
└── README.md               # This file
```

## How to use UWRobotics Template

### 1. Build and run the Docker Container with Docker Compose 

**Step 1: Build and Run Container**
```bash
docker compose build

# Build the Docker image and start container
docker compose up -d

# Enter the container
docker compose exec ros2-dev bash
```

**Step 2: Create New ROS2 Package Inside Container**
```bash
# Inside container - create new package
cd /ros2_ws/src
ros2 pkg create --build-type ament_cmake my_cpp_package \
   --dependencies rclcpp std_msgs \
   --license MIT

# Or create Python package
ros2 pkg create --build-type ament_python my_python_package \
   --dependencies rclpy std_msgs \
   --license MIT
```

**Step 3: Build and Test**
```bash
# Build the workspace
cd /ros2_ws
./build.sh

# Source the workspace
source install/setup.bash

# Run the node
ros2 run <package_name> <executable_name> <arg1> <arg2>

# Test the package
colcon test --packages-select <pkg1> <pkg2> <pkg3>

# View the test result
colcon test-result --verbose
```

#### Method 2: Direct Docker Commands

**Build Image:**
```bash
docker build -t ros2-humble-demo .
```

**Run Container:**
```bash
docker run -it --rm \
  --name ros2_container \
  -v $(pwd)/src:/ros2_ws/src:rw \
  -v $(pwd)/build:/ros2_ws/build:rw \
  -v $(pwd)/install:/ros2_ws/install:rw \
  --net=host \
  ros2-humble-demo
```

### 3. ROS2 Development Utils Available in Container

#### Package Creation
```bash
# C++ package
ros2 pkg create --build-type ament_cmake package_name --dependencies rclcpp std_msgs geometry_msgs

# Python package
ros2 pkg create --build-type ament_python package_name --dependencies rclpy std_msgs

# Generating executable packages
ros2 pkg create --build-type ament_cmake --license Apache-2.0 --node-name my_node my_package
```

#### Building and Dependencies
```bash
# Install dependencies
rosdep install --from-paths src --ignore-src -r -y

# Build specific packages
colcon build --packages-select package_name

# Build with debug info
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Debug
```

#### Testing and Quality
```bash
# Run tests
colcon test --packages-select package_name

# View test results
colcon test-result --verbose

# Lint code
ament_lint_auto package_name
```

#### Runtime Tools
```bash
# List nodes
ros2 node list

# Check topics
ros2 topic list
ros2 topic echo /topic_name

# Launch files
ros2 launch package_name launch_file.py

# Parameter management
ros2 param list
ros2 param get /node_name parameter_name
```

## Quick Start

1. **Clone and setup (automatic user detection):**
   ```bash
   git clone <this-repo>
   cd docker_demo
   ./scripts/setup.sh
   ```

2. **Start the development environment:**
   ```bash
   docker compose build
   docker compose up -d
   ```

3. **Enter the container:**
   ```bash
   docker compose exec ros2-dev bash
   ```

4. **Build and test your ROS2 packages:**
   ```bash
   # Inside the container
   cd /ros2_ws
   ./build.sh

   # Source the workspace
   source install/setup.bash

   # Run the demo executable
   ros2 run hello_world test_exec
   ```

5. **Access files from VS Code:**
   - All files in `build/`, `install/`, and `log/` should be created your computer not docker user 
   - Edit source files in `src/` normally

## Stopping the Environment

```bash
# Stop container
docker-compose down

# Remove everything including volumes
docker-compose down -v
```