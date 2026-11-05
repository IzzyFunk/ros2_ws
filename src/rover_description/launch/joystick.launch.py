import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    #Get the path to your package and joystick configuration file
    pkg_share = get_package_share_directory('rover_description')
    joy_params = os.path.join(pkg_share, 'config', 'joystick.yaml')

    #Node to launch the joystick driver and teleoperation node
    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node'
    )

    #Node to launch the teleoperation node that converts joystick inputs to velocity commands
    teleop_node = Node(
        package='teleop_twist_joy',
        executable='teleop_node',
        name='teleop_twist_joy_node',
        parameters=[joy_params]
    )

    return LaunchDescription([
        joy_node,
        teleop_node
    ])