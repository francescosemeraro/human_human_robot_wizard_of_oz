import launch
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, TextSubstitution

def generate_launch_description():

    azure_kinect_launch_prefix = get_package_share_directory('azure_kinect_ros_driver')

    included_launch = launch.actions.IncludeLaunchDescription(PythonLaunchDescriptionSource([azure_kinect_launch_prefix, '/launch/driver.launch.py']),
     launch_arguments={'color_resolution':'720P', 'fps':'30', 'body_tracking_enabled':'true', 'depth_mode':'NFOV_UNBINNED'}.items())
    print(azure_kinect_launch_prefix)
    
    bag_file_name = DeclareLaunchArgument(
        'bag_file_name', 
        default_value=TextSubstitution(text='tag'), 
        description="Tag related to the acquisition")
    
    acquisition_block_launch_prefix = get_package_share_directory('acquisition_block')
    print(acquisition_block_launch_prefix)
    included_launch2 = launch.actions.IncludeLaunchDescription(PythonLaunchDescriptionSource([acquisition_block_launch_prefix, '/launch/start_recording.launch.py']),
     launch_arguments={"bag_file_name": launch.substitutions.LaunchConfiguration('bag_file_name')}.items())

    condition = DeclareLaunchArgument(
        'condition', 
        default_value=TextSubstitution(text='not set'), 
        description="Name of the condition being administered in the user study")   
    
    robot_controller= Node(
            package='control_block',
            namespace='user_study',
            executable='test_transitions',
            name='robot_controller',
            parameters=[
                {"condition": launch.substitutions.LaunchConfiguration('condition')}]
        )
    
    sound_emitter= Node(
            package='control_block',
            namespace='user_study',
            executable='produce_sound',
            name='sound_emitter',
        )

    return LaunchDescription([
        included_launch,
        included_launch2,
        bag_file_name,
        condition,
        robot_controller,
        sound_emitter
    ])

