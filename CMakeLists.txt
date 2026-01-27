cmake_minimum_required(VERSION 3.10)

project(workHosting CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ${CMAKE_PROJECT_NAME}_SRC LIST_DIRECTORIES FALSE ./src/*.cpp)

set(${CMAKE_PROJECT_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
if(${CMAKE_PROJECT_NAME}_SRC)
	set(${CMAKE_PROJECT_NAME}_lib_FOUND)
	add_library(${CMAKE_PROJECT_NAME}_LIB STATIC ${${CMAKE_PROJECT_NAME}_SRC})
	target_include_directories(${CMAKE_PROJECT_NAME}_LIB
		PUBLIC ${${CMAKE_PROJECT_NAME}_INCLUDE_DIR})
endif()

# setup zeromq
add_subdirectory(third_party/libzmq)
add_subdirectory(third_party/cppzmq/)

#  # setup userver
# find_package(userver COMPONENTS core chaotic redis postgresql QUIET)
# if(NOT userver_FOUND)
# 	message(STATUS "Global userver installation not found. Trying to fetch local userver at ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver directory")
# 	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver")
# 		message(STATUS "Using local ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver framework")
# 		set(USERVER_FEATURE_MONGODB ON CACHE BOOL "" FORCE)
# 		add_subdirectory(third_party/userver)
# 	else()
# 		message(FATAL_ERROR "Userver not found not global neither locally")
# 	endif()
# endif()

add_subdirectory(services)

