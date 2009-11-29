cmake_minimum_required(VERSION 2.6)

PROJECT(seniordesign C)

ADD_DEFINITIONS (-Wall -g)

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (include)

SET(SOURCES	
						src/ping/modPing.c
						src/dataTable/dataTable.c 
						src/main/main.c
						)

