cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

####################################
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/INSTALL)
set(CMAKE_BUILD_TYPE "Release")

####################################################
## ---------------------------------------------- ##
## -                                            - ##
## -            Enable OpenMP Support           - ##
## -                                            - ##
## ---------------------------------------------- ##
####################################################

# Begin configuring OpenMP options
macro(enable_openmp_support DEFAULT)

	OPTION(USE_OPENMP "Enable OpenMP multithreading" ${DEFAULT})
	IF(USE_OPENMP)
		find_package(OpenMP REQUIRED)

		# Add the OpenMP-specific compiler and linker flags
		list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
		list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
		list(APPEND CMAKE_EXE_LINKER_FLAGS ${OpenMP_EXE_LINKER_FLAGS})

	ENDIF(USE_OPENMP)
endmacro(enable_openmp_support)

set(MAIN combine_hosts)
project("TrackingBlocker")

INCLUDE_DIRECTORIES($ENV{HOME}/C++/includes/tools)

enable_openmp_support(ON)

file(GLOB TB ${PROJECT_SOURCE_DIR}/ModifyTrackingBlocker)
if(XCODE)
    set_source_files_properties(
        ${TB}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")
endif()

add_executable( ${MAIN} ${MAIN}.cc ${TB})

SET(HOST_FILES
    hosts.original
    hosts.ultrasafe
    hosts.safe
    HOSTS
    hosts.txt
  )
  
foreach(_host ${HOST_FILES})
    configure_file(
        ${PROJECT_SOURCE_DIR}/share/${_host}
        ${PROJECT_BINARY_DIR}/share/${_host}
        COPY_ONLY
    )
endforeach()

configure_file(
        ${PROJECT_SOURCE_DIR}/ModifyTrackingBlocker
        ${PROJECT_BINARY_DIR}/ModifyTrackingBlocker
        COPY_ONLY
    )
    
install(TARGETS ${MAIN} DESTINATION bin)
#install(FILES ${PROJECT_SOURCE_DIR}/ModifyTrackingBlocker DESTINATION bin)
