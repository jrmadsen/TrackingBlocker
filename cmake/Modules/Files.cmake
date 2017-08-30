# files for TrackingBlocker

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)

include_directories(${SOURCE_DIR})

file(GLOB headers ${SOURCE_DIR}/*.hh ${SOURCE_DIR}/*.cc)

file(GLOB DB ${PROJECT_SOURCE_DIR}/scripts/tracking_blocker)

if(XCODE)
    set_source_files_properties(
        ${DB}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

