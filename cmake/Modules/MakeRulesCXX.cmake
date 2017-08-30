# Make rules for TrackingBlocker

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if(BUILD_TYPE AND NOT "${BUILD_TYPE}" STREQUAL "${CMAKE_BUILD_TYPE}")
  unset(CMAKE_CXX_FLAGS CACHE)
endif()

set(CXX11_FLAGS "-std=c++11")
set(EXTRA_CXX_FLAGS "-Wno-unused-function -Wno-unused-but-set-variable ${USER_CXX_FLAGS}")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CXX11_FLAGS "-std=c++11 -stdlib=libc++")
    set(EXTRA_CXX_FLAGS "${EXTRA_CXX_FLAGS} -Qunused-arguments")
endif()

set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DDEBUG -g -gdwarf-2 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DNDEBUG -O3 -Wall")
set(CMAKE_CXX_FLAGS_MINSIZEREL      "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DNDEBUG -Os -Wall")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DDEBUG -g -gdwarf-2 -O2 -Wall")

if(NOT BUILD_TYPE OR NOT "${BUILD_TYPE}" STREQUAL "${CMAKE_BUILD_TYPE}")
  set(BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Build type" FORCE)
  string(TOUPPER "${BUILD_TYPE}" UPPER_BUILD_TYPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_${UPPER_BUILD_TYPE}}"
      CACHE STRING "Build rules" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")

message(STATUS "Build type : ${BUILD_TYPE}")

