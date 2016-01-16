cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

####################################
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/INSTALL)
set(CMAKE_BUILD_TYPE "Release")

set(MAIN combine_hosts)
project("TrackingBlocker")

INCLUDE_DIRECTORIES($ENV{HOME}/C++/includes/tools)


file(GLOB TB ${PROJECT_SOURCE_DIR}/ModifyTrackingBlocker)
file(GLOB tools $ENV{HOME}/C++/includes/tools/*.hh)
if(XCODE)
    set_source_files_properties(
        ${TB}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")
else(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -O3 -Wall")
endif()

add_executable( ${MAIN} ${MAIN}.cc ${tools} ${TB})

SET(HOST_FILES
    hosts.original
    hosts.ultrasafe
    hosts.safe
    HOSTS
    hosts.txt
    hosts.shc
  )

foreach(_host ${HOST_FILES})
    configure_file(
        ${PROJECT_SOURCE_DIR}/share/${_host}
        ${PROJECT_BINARY_DIR}/share/${_host}
        COPYONLY
    )
endforeach()

configure_file(
        ${PROJECT_SOURCE_DIR}/share/hosts.safe
        ${PROJECT_BINARY_DIR}/hosts.safe
        COPYONLY
    )

install(TARGETS ${MAIN} DESTINATION bin)
#install(FILES ${PROJECT_SOURCE_DIR}/ModifyTrackingBlocker DESTINATION bin)
