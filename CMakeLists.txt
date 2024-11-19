cmake_minimum_required(VERSION 3.5)
project(rviz2_distance_tool)

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rviz2 REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_LIBRARIES Qt5::Widgets)

include_directories(
  include
  ${rviz2_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} SHARED
  src/distance_tool.cpp
  include/rviz2_distance_tool/distance_tool.h
)

ament_target_dependencies(${PROJECT_NAME}
  rviz2
  rviz_common
  rviz_rendering
  Qt5
)

pluginlib_export_plugin_description_file(rviz_common plugin_description.xml)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_package()
