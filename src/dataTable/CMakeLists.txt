INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/include)


# Tell CMake to build a shared library from dataTable.c
ADD_LIBRARY (dataTable SHARED dataTable.c)

# make sure the library gets installed
INSTALL_TARGETS(/lib dataTable)
