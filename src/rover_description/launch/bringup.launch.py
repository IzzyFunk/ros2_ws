import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    pkg_share = get_package_share_directory('rover_description')
    
    # 1. Bring up the Robot State Publisher and Gazebo Simulation
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(pkg_share, 'launch', 'gazebo.launch.py'))
    )
    
    # 2. Bridge the ROS 2 and Gazebo networks for sensors and drive commands
    bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist',
            '/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
            '/camera/image_raw@sensor_msgs/msg/Image[gz.msgs.Image'
        ],
        output='screen'
    )
    
    # 3. Launch RViz2 with your saved configuration layout
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', os.path.join(pkg_share, 'rviz', 'rover.rviz')],
        output='screen'
    )
    
    # 4. Bring up the Gamepad Teleop nodes
    joystick_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(pkg_share, 'launch', 'joystick.launch.py'))
    )

    return LaunchDescription([
        gazebo_launch,
        bridge_node,
        rviz_node,
        joystick_launch
    ])