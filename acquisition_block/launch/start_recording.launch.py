import launch
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, TextSubstitution

def generate_launch_description():

    bag_file_name = DeclareLaunchArgument(
        'bag_file_name', 
        default_value=TextSubstitution(text='tag'), 
        description="Tag related to the acquisition")
    
    datapoint_recorder= Node(
            package='acquisition_block',
            namespace='user_study',
            executable='store_datapoint',
            name='store_datapoint',
            parameters=[
                {"bag_file_name": launch.substitutions.LaunchConfiguration('bag_file_name')}]
        )
    
    return LaunchDescription([
        datapoint_recorder
    ])