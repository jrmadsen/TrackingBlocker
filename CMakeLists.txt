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

include(Data)

# install C++ executable
install(TARGETS ${MAIN} DESTINATION ${CMAKE_INSTALL_PREFIX}/libexec)
# install bash scripts
install(FILES ${PROJECT_BINARY_DIR}/run-as-root-wrapper ${PROJECT_BINARY_DIR}/ModifyTrackingBlocker
		DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
		PERMISSIONS
			OWNER_READ OWNER_WRITE OWNER_EXECUTE
			GROUP_READ GROUP_EXECUTE
			WORLD_READ WORLD_EXECUTE		
)
# install python script
install(FILES 
			${PROJECT_BINARY_DIR}/pyscripts/TrackingBlocker.py
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/bin/pyscripts
		PERMISSIONS
			OWNER_READ OWNER_WRITE OWNER_EXECUTE
			GROUP_READ GROUP_EXECUTE
			WORLD_READ WORLD_EXECUTE		
)
# install host files
install(FILES 
			${PROJECT_BINARY_DIR}/share/hosts.original 
			${PROJECT_BINARY_DIR}/hosts.ultrasafe
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
		PERMISSIONS
			OWNER_READ OWNER_WRITE
			GROUP_READ
)
# custom target
add_custom_command(TARGET ${MAIN}
	POST_BUILD
	COMMAND ${PROJECT_BINARY_DIR}/${MAIN} -u -o hosts.ultrasafe -d share/hosts.original -f share/hosts.ultrasafe
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	DEPENDS ${MAIN}
	COMMENT "Generating new hosts.ultrasafe..."
)
#add_dependencies(host ${MAIN})
# create uninstall target
include(CMakeUninstallTarget)

message(STATUS "Install prefix for ${PROJECT_NAME}: ${CMAKE_INSTALL_PREFIX}")

