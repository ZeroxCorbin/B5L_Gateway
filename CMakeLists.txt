# Copyright(C)  2019-2020  OMRON Corporation                                
#                                                                           
# Licensed under the Apache License, Version 2.0 (the "License");           
# you may not use this file except in compliance with the License.          
# You may obtain a copy of the License at                                   
#                                                                           
#     http://www.apache.org/licenses/LICENSE-2.0                            
#                                                                           
# Unless required by applicable law or agreed to in writing, software       
# distributed under the License is distributed on an "AS IS" BASIS,         
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
# See the License for the specific language governing permissions and       
# limitations under the License.                                            
cmake_minimum_required(VERSION 3.5)
project(omron_b5l_a)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build)
     file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/build )
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../build/${PROJECT_NAME}/omron_b5l_a)
     file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/../build/${PROJECT_NAME}/omron_b5l_a )
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../install/${PROJECT_NAME}/lib/${PROJECT_NAME}/omron_b5l_a)
     file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/../install/${PROJECT_NAME}/lib/${PROJECT_NAME}/omron_b5l_a )
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../install/${PROJECT_NAME}/lib/${PROJECT_NAME}/omron_b5l_a)
     file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/../../install/${PROJECT_NAME}/lib/${PROJECT_NAME}/omron_b5l_a )
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../build/${PROJECT_NAME}/omron_b5l_a)
     file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/../../build/${PROJECT_NAME}/omron_b5l_a )
endif()

# If changes are not appeared after build then locat binary (omron_b5l_a) and .o file then remove it and build again

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/support/)
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_definitions(-DPI)
add_definitions(-DSOURCE_DIR_PREFIX="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(${PCL_DEFINITIONS})

#find_package(pcl_conversions REQUIRED)
find_package(PCL 1.3 REQUIRED COMPONENTS common io)
find_package(Boost COMPONENTS system filesystem REQUIRED)

link_directories(${PCL_LIBRARY_DIRS})

add_executable(
     omron_b5l_a
     src/ToF_Sample.cpp
     src/TOFApiZ.cpp
     src/uart_linux.c
     src/main.cpp
     support/clsTCPSocket.cpp
     support/libException.cpp
     
     )
#ament_target_dependencies(omron_b5l_a rclcpp std_msgs  pcl_conversions ) 

target_link_libraries(
     omron_b5l_a
     pthread
     ${PCL_LIBRARIES}
     )

install(TARGETS
  omron_b5l_a
  DESTINATION lib/${PROJECT_NAME})
