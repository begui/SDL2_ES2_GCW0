cmake_minimum_required(VERSION 2.8)
##############################################
# Prevent cmake generated files in root directory
##############################################
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Please run inside of a build folder.")
endif()
##############################################
# Set project name 
##############################################
set(PROJECTNAME "sdl2_es2")
project(${PROJECTNAME})
option(BUILD_GCW0 "Build for the GCW0" OFF)
##############################################
# Set GCW0 toolchain 
##############################################
if(UNIX)
	if(BUILD_GCW0)
		message(STATUS "Building for GCW0")
		include(/opt/gcw0-toolchain/usr/share/buildroot/toolchainfile.cmake)
		add_definitions(-DUSE_GCW0)
	endif(BUILD_GCW0)
	message(STATUS "Setting Complier options")
	add_definitions(-std=c++11 -g -Wall -Wextra)
endif(UNIX)
##############################################
# Find library dependencies 
##############################################
find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
if(BUILD_GCW0)
	pkg_search_module(GLESv2 REQUIRED glesv2)
else()
	pkg_search_module(OpenGL REQUIRED gl)
endif(BUILD_GCW0)

##############################################
# Set Source  
##############################################
set(SRC
	sdl2_es2_main.cc
)
##############################################
# Create GCW0 opk script  
##############################################
if(BUILD_GCW0)
	file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
	configure_file( opk/default.gcw0.desktop ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	file( WRITE  ${CMAKE_CURRENT_BINARY_DIR}/create_opk.sh "/opt/gcw0-toolchain/usr/bin/mksquashfs ./data/* ${PROJECTNAME}.bin default.gcw0.desktop ${PROJECTNAME}.opk -all-root -noappend -no-exports -no-xattrs")
endif(BUILD_GCW0)
##############################################
# Build Binary 
##############################################
add_executable(${PROJECTNAME}.bin ${SRC})
if(BUILD_GCW0)
	target_link_libraries(${PROJECTNAME}.bin
		${SDL2_LIBRARIES}
		${GLESv2_LIBRARIES}
		)
else()
	target_link_libraries(${PROJECTNAME}.bin
		${SDL2_LIBRARIES}
		${OpenGL_LIBRARIES}
		)
endif(BUILD_GCW0)
