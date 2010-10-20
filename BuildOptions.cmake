#--------------------------------------------------------------------#
#                                                                    #
#                          BinPAC - Build Setup                      #
#                                                                    #
#--------------------------------------------------------------------#
#
# The installation directory
set(CMAKE_INSTALL_PREFIX /usr/local
    CACHE STRING "Installation directory" FORCE)

set(ENABLE_DEBUG false
    CACHE STRING "Compile with debugging symbols" FORCE)

set(ENABLE_RELEASE false
    CACHE STRING "Use -O3 compiler optimizations" FORCE)
