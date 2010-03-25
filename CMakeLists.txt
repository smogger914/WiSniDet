cmake_minimum_required(VERSION 2.6)

PROJECT(seniordesign CXX)
ADD_DEFINITIONS (-Wall -g)

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (include)

SET(SOURCES	
		src/pmd/pmd.c
		src/cpumon/cpumon.c
		src/main/main.c
	)

INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE (seniordesign 	
															${SOURCES})
