cmake_minimum_required(VERSION 2.8.3)
project(phantom_joystick)

find_package( catkin REQUIRED COMPONENTS nodelet roscpp sensor_msgs )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES  phantom_joystick
  CATKIN_DEPENDS nodelet roscpp sensor_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS} )

add_library( linuxarbotixpro src/LinuxArbotixPro.cpp )
#add_dependencies( linuxarbotixpro )

add_library( arbotixpro src/ArbotixPro.cpp )
add_dependencies( arbotixpro linuxarbotixpro)


add_executable( phantom_joystick src/phantom_teleop_joystick.cpp ) 
target_link_libraries( phantom_joystick ${catkin_LIBRARIES} linuxarbotixpro arbotixpro)

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
  
 
