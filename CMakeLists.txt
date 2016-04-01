cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(TrackingBlocker)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Projects/TrackingBlocker
                      ${PROJECT_SOURCE_DIR}/cmake/Modules
                      ${CMAKE_MODULE_PATH})
include(MakeRules_cxx)

set(MAIN combine_hosts)
include(Packages)
include(Files)
add_subdirectory(Qt)

add_executable( ${MAIN} ${MAIN}.cc ${tools} ${TB})

message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS ${MAIN} DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec)

include(Data)
