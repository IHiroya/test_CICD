cmake_minimum_required(VERSION 3.8)
project(sample_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# ROSに依存しないロジック部分をライブラリとして分離する(単体テストしやすくするため)
add_library(moving_average src/moving_average.cpp)
target_include_directories(moving_average PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(moving_average PUBLIC cxx_std_17)

add_library(low_pass_filter src/low_pass_filter.cpp)
target_include_directories(low_pass_filter PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(low_pass_filter PUBLIC cxx_std_17)

add_library(obstacle_stop_controller src/obstacle_stop_controller.cpp)
target_include_directories(obstacle_stop_controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(obstacle_stop_controller PUBLIC cxx_std_17)

# ROSノード本体
add_executable(moving_average_node src/moving_average_node.cpp)
target_link_libraries(moving_average_node
  moving_average
  rclcpp::rclcpp
  ${std_msgs_TARGETS})

install(TARGETS moving_average low_pass_filter obstacle_stop_controller moving_average_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

# `colcon test` で実行される単体テスト
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_moving_average test/test_moving_average.cpp)
  target_link_libraries(test_moving_average moving_average)

  ament_add_gtest(test_low_pass_filter test/test_low_pass_filter.cpp)
  target_link_libraries(test_low_pass_filter low_pass_filter)

  # モックを使うテストは ament_add_gmock で登録する(GTest + GMock がリンクされる)
  find_package(ament_cmake_gmock REQUIRED)
  ament_add_gmock(test_obstacle_stop_controller test/test_obstacle_stop_controller.cpp)
  target_link_libraries(test_obstacle_stop_controller obstacle_stop_controller)
endif()

ament_package()
