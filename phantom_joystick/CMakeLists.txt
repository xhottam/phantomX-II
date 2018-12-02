cmake_minimum_required(VERSION 2.8.3)

set( CMAKE_CXX_FLAGS "-pthread")

project(phantom_joystick)

find_package( catkin REQUIRED COMPONENTS nodelet roscpp sensor_msgs )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES  phantom_joystick
  CATKIN_DEPENDS nodelet roscpp sensor_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS} /home/odroid/PhantomII-Framework/Framework/include /home/odroid/PhantomII-Framework/Linux/include ) 

add_executable( phantom_joystick src/phantom_teleop_joystick.cpp ) 
#link_directories( /home/ros/HROS1-Framework/Linux/lib )
target_link_libraries( phantom_joystick ${catkin_LIBRARIES} /home/odroid/PhantomII-Framework/Linux/lib/darwin.a )
#add_dependencies( phantom_joystick )

install(TARGETS phantom_joystick
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

install(FILES phantom_joystick.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
  
 
