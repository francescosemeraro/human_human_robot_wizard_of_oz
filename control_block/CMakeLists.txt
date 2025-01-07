cmake_minimum_required(VERSION 3.8)
project(control_block)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(interface_msgs REQUIRED)
find_package(rosbag2_cpp REQUIRED) 
find_package(rosbag2_compression REQUIRED) 
find_package(std_msgs REQUIRED)


add_executable(test_transitions src/test_transitions.cpp)
ament_target_dependencies(
  test_transitions
  rclcpp
  trajectory_msgs
  builtin_interfaces
  sensor_msgs
)
target_compile_features(test_transitions PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

add_executable(produce_sound src/produce_sound.cpp)
ament_target_dependencies(
  produce_sound
  rclcpp
  std_msgs
)
target_compile_features(produce_sound PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS test_transitions
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS produce_sound
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()