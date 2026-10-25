from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Spin up the Publisher node
        Node(
            package='cpp_pub_sub',
            executable='talker',
            name='custom_talker_node',
            output='screen',
            parameters=[
                {'custom_timer_rate' : 10.0},
                {'use_sim_time' : False}
            ]
        ),
        # Spin up the Subscriber node
        Node(
            package='cpp_pub_sub',
            executable='listener',
            name='custom_listener_node',
            output='screen'
        )
    ])