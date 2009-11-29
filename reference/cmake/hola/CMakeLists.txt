PROJECT (projecte CXX)

# directorive to add flags to the compiling process
add_definitions (-Wall -W -ggdb)

SET (SRC projecte)

# enables 'make install'
# DESTINATION bin tells to install in $prefix/bin
INSTALL (FILES projecte DESTINATION bin)

# no need mention Hola.cpp because it's included in projecte.cpp
ADD_EXECUTABLE (projecte ${SRC})
