# find packages for TrackingBlocker

set(CMAKE_THREAD_PREFER_PTHREADS ON)
find_package(Threads REQUIRED)

option(USE_PSTREAMS "Use pstreams library from command line arguments" OFF)
if(USE_PSTREAMS)
    add_definitions(-DUSE_PSTREAMS)
endif()

set(EXTERNAL_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
