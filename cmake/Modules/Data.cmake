# Data files for TrackingBlocker

SET(HOST_FILES hosts.profile.0)

foreach(_host ${HOST_FILES})
    configure_file(
        ${PROJECT_SOURCE_DIR}/share/${_host}
        ${PROJECT_BINARY_DIR}/share/${_host}
        COPYONLY
    )
endforeach()

configure_file(
    ${PROJECT_SOURCE_DIR}/share/hosts.profile.0
    ${PROJECT_BINARY_DIR}/hosts.profile.0
    COPYONLY
)

if(NOT HOSTNAME)
    set(HOSTNAME $ENV{HOSTNAME})

    if("${HOSTNAME}" STREQUAL "")
        execute_process(
            COMMAND hostname
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE RET
            OUTPUT_VARIABLE HOSTNAME
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
endif()

if(DEFINED HOSTNAME AND NOT "${HOSTNAME}" STREQUAL "")
    configure_file(
        ${PROJECT_SOURCE_DIR}/share/hosts.original.in
        ${PROJECT_BINARY_DIR}/share/hosts.original
        @ONLY
    )
else()
    message(FATAL_ERROR "Hostname not known. Please specify -DHOSTNAME=...")
endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/scripts/tracking_blocker.in
    ${PROJECT_BINARY_DIR}/tracking_blocker
    @ONLY
)

configure_file(
    ${PROJECT_SOURCE_DIR}/scripts/TrackingBlocker.py
    ${PROJECT_BINARY_DIR}/pyscripts/TrackingBlocker.py
    COPYONLY
)

configure_file(
    ${PROJECT_SOURCE_DIR}/scripts/remove-ctrl-M.bash
    ${PROJECT_BINARY_DIR}/pyscripts/remove-ctrl-M.bash
    COPYONLY
)
