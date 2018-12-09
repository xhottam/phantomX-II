cmake_minimum_required(VERSION 2.8.3)
project( phantom_controller )

find_package( catkin REQUIRED COMPONENTS nodelet roscpp geometry_msgs sensor_msgs std_msgs )


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES phantom_controller 
  CATKIN_DEPENDS nodelet roscpp geometry_msgs sensor_msgs std_msgs 
)

include_directories(include ${catkin_INCLUDE_DIRS})


add_executable( phantom_controller src/imu.cpp )
target_link_libraries( phantom_controller ${catkin_LIBRARIES} )

install(TARGETS phantom_controller
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(FILES phantom_controller.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
