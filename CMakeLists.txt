cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(TrackingBlocker)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
include(MakeRulesCXX)
include(GNUInstallDirs)
include(Packages)
include(Files)

add_subdirectory(Qt)

add_executable(combine_hosts combine_hosts.cc ${headers} ${DB})

include(Data)

# install C++ executable
install(TARGETS combine_hosts DESTINATION ${CMAKE_INSTALL_LIBEXECDIR})
# install bash scripts
install(
    FILES
        ${PROJECT_BINARY_DIR}/tracking_blocker
    DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
# install python script
install(
    FILES
        ${PROJECT_BINARY_DIR}/pyscripts/TrackingBlocker.py
        ${PROJECT_BINARY_DIR}/pyscripts/remove-ctrl-M.bash
        ${PROJECT_BINARY_DIR}/pyscripts/run_command.py
    DESTINATION
        ${CMAKE_INSTALL_BINDIR}/pyscripts
    PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)
# install host files
install(
    FILES
        ${PROJECT_BINARY_DIR}/share/hosts.original
        ${PROJECT_BINARY_DIR}/hosts.profile.0
    DESTINATION
        ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
)
# custom target
add_custom_command(TARGET combine_hosts
    POST_BUILD
    COMMAND ${PROJECT_BINARY_DIR}/combine_hosts -u -o hosts.profile.0 -d share/hosts.original -f share/hosts.profile.0
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS combine_hosts
    COMMENT "Generating new hosts.profile.0..."
)

# create uninstall target
include(CMakeUninstallTarget)

add_feature(CMAKE_BUILD_TYPE "Project build type")
add_feature(CMAKE_INSTALL_PREFIX "Installation prefix")

print_features()

