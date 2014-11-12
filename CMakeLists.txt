cmake_minimum_required(VERSION 2.8)
project(youbot_tactical_level)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy geometry_msgs sensor_msgs std_msgs 
cv_bridge image_transport) # genmsg


## Declare a catkin package
catkin_package()

include_directories(
include 
include/udp 
include/urg 
${catkin_INCLUDE_DIRS}
)


set(SOURCE
RosHelper.cpp
   main.cpp
   Behaviours/MoveWithLaser.cpp
   Behaviours/MoveZigZag.cpp
   Behaviours/MoveSpin.cpp
   Behaviours/MoveStop.cpp
	urg/HokuyoLaser.cpp
urg/Connection_information.cpp
udp/Udp.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})		


set(EXT_LIBS
${PROJECT_SOURCE_DIR}/lib/liburg_cpp.a #can't use just relative path, coz it is later used in another program (linker)
)

add_library(liburg_cpp STATIC IMPORTED)
set_target_properties(liburg_cpp PROPERTIES IMPORTED_LOCATION ${EXT_LIBS})
target_link_libraries(${PROJECT_NAME} liburg_cpp)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})


