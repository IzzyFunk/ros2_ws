cmake_minimum_required(VERSION 3.20)
project(cpp_tf2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)

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

add_executable(static_broadcaster src/static_broadcaster.cpp)
target_link_libraries(static_broadcaster PUBLIC rclcpp::rclcpp tf2::tf2 tf2_ros::tf2_ros geometry_msgs::geometry_msgs)

add_executable(dynamic_broadcaster src/dynamic_broadcaster.cpp)
target_link_libraries(dynamic_broadcaster PUBLIC rclcpp::rclcpp tf2::tf2 tf2_ros::tf2_ros geometry_msgs::geometry_msgs)

add_executable(tf_listener src/tf_listener.cpp)
target_link_libraries(tf_listener PUBLIC rclcpp::rclcpp tf2::tf2 tf2_ros::tf2_ros geometry_msgs::geometry_msgs)

install(TARGETS
  static_broadcaster
  dynamic_broadcaster
  tf_listener
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()