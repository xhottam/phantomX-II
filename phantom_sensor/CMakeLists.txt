cmake_minimum_required(VERSION 2.8.3)
project( phantom_sensor )

find_package( catkin REQUIRED COMPONENTS nodelet rospy roscpp geometry_msgs sensor_msgs std_msgs serial )


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES phantom_sensor 
  CATKIN_DEPENDS nodelet roscpp geometry_msgs sensor_msgs std_msgs serial
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_library( control src/control.cpp )
add_dependencies( control sensor_msgs serial )

add_library( imu src/imu.cpp )
add_dependencies( imu  sensor_msgs )

add_executable( phantom_sensor src/phantom_sensor.cpp )
target_link_libraries( phantom_sensor ${catkin_LIBRARIES} control imu)

install(TARGETS phantom_sensor
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(DIRECTORY src
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY 3D_visualization
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(FILES phantom_sensor.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})


catkin_install_python(PROGRAMS 
	3D_visualization/display_minimuv3_visualization_from_serial.py
        3D_visualization/display_minimuv3_visualization_from_topic.py
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/src
)
