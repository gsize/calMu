#####################################################
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

#####################################################
project(calMu)

message("project source dir: ${PROJECT_SOURCE_DIR}")

SET(SRC ${PROJECT_SOURCE_DIR}/src/xcom.cpp ${PROJECT_SOURCE_DIR}/src/GateMaterialsDB.cpp )

#add_definitions(-D_WINDOWS_PLATFORM)

#add include files
include_directories(
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/src
${PROJECT_SOURCE_DIR}/../include
)

IF (WIN32)
	message(STATUS "Now is windows")
	add_definitions(-Dwin32)
ELSE ()
	message(STATUS "Now is Linux")
	add_definitions(-DLinux)
	set(CMAKE_CXX_STANDARD 11)	
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")	
	#list(APPEND CMAKE_CXX_FLAGS " -std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs" )	
ENDIF()


add_executable(calMuMap  ${SRC} ${PROJECT_SOURCE_DIR}/src/main.cpp )
add_executable(calMu ${SRC} ${PROJECT_SOURCE_DIR}/src/calMu.cpp)



