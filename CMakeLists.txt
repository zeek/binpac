cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
project(BinPAC C CXX)
include(cmake/CommonCMakeConfig.cmake)
include(cmake/RequireCXXStd.cmake)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" BINPAC_VERSION LIMIT_COUNT 1)
string(REPLACE "." " " _version_numbers ${BINPAC_VERSION})
separate_arguments(_version_numbers)
list(GET _version_numbers 0 BINPAC_VERSION_MAJOR)
list(GET _version_numbers 1 BINPAC_VERSION_MINOR)
string(REGEX REPLACE "-[0-9]*$" "" BINPAC_VERSION_MINOR ${BINPAC_VERSION_MINOR})

# The SO number shall increase only if binary interface changes.
set(BINPAC_SOVERSION 0)

set(ENABLE_SHARED true)

if (ENABLE_STATIC_ONLY)
    set(ENABLE_STATIC true)
    set(ENABLE_SHARED false)
endif ()

# Set default install paths
include(GNUInstallDirs)

# ##############################################################################
# Dependency Configuration

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

if (MSVC)
    add_compile_options(/J) # Similar to -funsigned-char on other platforms
endif ()

# ##############################################################################
# System Introspection

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

include_directories(BEFORE ${PROJECT_BINARY_DIR})

# ##############################################################################
# Recurse on sub-directories

add_subdirectory(lib)
add_subdirectory(src)

# ##############################################################################
# Build Summary

if (CMAKE_BUILD_TYPE)
    string(TOUPPER ${CMAKE_BUILD_TYPE} BuildType)
endif ()

macro (display test desc summary)
    if (${test})
        set(${summary} ${desc})
    else ()
        set(${summary} no)
    endif ()
endmacro ()

display(ENABLE_SHARED yes shared_summary)
display(ENABLE_STATIC yes static_summary)

message(
    "\n==================|  BinPAC Build Summary  |===================="
    "\nVersion:           ${BINPAC_VERSION}"
    "\nSO version:        ${BINPAC_SOVERSION}"
    "\n"
    "\nBuild Type:        ${CMAKE_BUILD_TYPE}"
    "\nDebug mode:        ${ENABLE_DEBUG}"
    "\nInstall prefix:    ${CMAKE_INSTALL_PREFIX}"
    "\nShared libs:       ${shared_summary}"
    "\nStatic libs:       ${static_summary}"
    "\n"
    "\nCC:                ${CMAKE_C_COMPILER}"
    "\nCFLAGS:            ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BuildType}}"
    "\nCXX:               ${CMAKE_CXX_COMPILER}"
    "\nCXXFLAGS:          ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BuildType}}"
    "\nCPP:               ${CMAKE_CXX_COMPILER}"
    "\n"
    "\n================================================================\n")

include(UserChangedWarning)
