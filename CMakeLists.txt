SET (CMAKE_C_COMPILER             "clang")
SET (CMAKE_CXX_COMPILER            "clang++")


cmake_minimum_required(VERSION 3.0)

project(odbtool)

set(SOURCES
	src/main.cpp
	src/CanClient.cpp
)

add_executable(odbtool ${SOURCES})


set_target_properties(odbtool PROPERTIES
				CXX_STANDARD 17
				CXX_EXTENSIONS OFF
				)
 
set_target_properties(odbtool PROPERTIES
			OUTPUT_NAME "odbtool"
			)

			  
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(odbtool  PRIVATE Threads::Threads)

target_include_directories(odbtool
	PRIVATE
	src
)

set(CMAKE_BINARY_DIR "bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
