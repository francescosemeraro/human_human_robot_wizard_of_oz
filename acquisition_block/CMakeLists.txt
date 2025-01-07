cmake_minimum_required(VERSION 3.5)
project(acquisition_block)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosbag2_cpp REQUIRED) 
find_package(rosbag2_compression REQUIRED) 
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(interface_msgs REQUIRED)
find_package(Eigen3 REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()


add_executable(store_datapoint src/store_datapoint.cpp)
ament_target_dependencies(
  store_datapoint
  rclcpp
  interface_msgs
  visualization_msgs
  interface_msgs
  builtin_interfaces
  rosbag2_cpp
  rosbag2_compression
)
target_compile_features(store_datapoint PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17


install(TARGETS
  store_datapoint
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

include_directories(include
${EIGEN3_INCLUDE_DIRS})

ament_package()
