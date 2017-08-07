
include(MacroUtilities)
include(ExternalProject)
include(GNUInstallDirs)

option(BUILD_GUI "Build Qt interface" OFF)
add_feature(BUILD_GUI "Build Qt interface")

if(BUILD_GUI)
  option(USE_QT_QMAKE
         "Build the Qt interface using QMake externally instead of CMake" OFF)
  add_subfeature(BUILD_GUI USE_QT_QMAKE
                 "Build the Qt interface using QMake externally instead of CMake")

  set(new_include_dirs )
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
               PROPERTY INCLUDE_DIRECTORIES ${new_include_dirs})
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/source
      ${PROJECT_SOURCE_DIR}/tools)

  ConfigureRootSearchPath(QT)
  ConfigureRootSearchPath(QT4)
  ConfigureRootSearchPath(QT5)

  set(name QtTrackingBlocker)
  set(path ${PROJECT_SOURCE_DIR}/Qt)

  file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.hh)
  file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${name}.pro.in
                 ${CMAKE_CURRENT_SOURCE_DIR}/${name}.pro
                 @ONLY )

  if(NOT USE_QT_QMAKE AND NOT CMAKE_VERSION VERSION_LESS 3.0)
    set(CMAKE_AUTOMOC ON)
    option(USE_QT4 "Force use of Qt4 always" OFF)
    add_subfeature(BUILD_GUI USE_QT4
                   "Force explicit use of Qt4 (don't search for Qt5)")

    if(NOT USE_QT4)
      find_package(Qt5Core QUIET)
      find_package(Qt5Gui QUIET)
      find_package(Qt5OpenGL QUIET)
      find_package(Qt5PrintSupport QUIET)
    endif()

    if(NOT USE_QT4
        AND Qt5Core_FOUND
        AND Qt5Gui_FOUND
        AND Qt5OpenGL_FOUND
        AND Qt5PrintSupport_FOUND)
      INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS}
                          ${Qt5Gui_INCLUDE_DIRS}
                          ${Qt5OpenGL_INCLUDE_DIRS}
                          ${Qt5PrintSupport_INCLUDE_DIRS} )
      add_executable(${name} ${path}/${name}.cc ${HEADERS} ${SOURCES})
      target_link_libraries(${name}
                            ${Qt5Core_LIBRARIES}
                            ${Qt5Gui_LIBRARIES}
                            ${Qt5OpenGL_LIBRARIES}
                            ${Qt5PrintSupport_LIBRARIES}
                            ${EXTERNAL_LIBRARIES})
      add_custom_target(vis DEPENDS ${name})
    else()
      unset(Qt5Core_DIR CACHE)
      unset(Qt5Gui_DIR CACHE)
      unset(Qt5OpenGL_DIR CACHE)
      unset(Qt5PrintSupport_DIR CACHE)
      find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL)
      INCLUDE_DIRECTORIES(${QT_QTCORE_INCLUDE_DIR}
                          ${QT_QTGUI_INCLUDE_DIR}
                          ${QT_QTOPENGL_INCLUDE_DIR}
                          ${QT_QTXML_INCLUDE_DIR} )
      add_executable(${name} ${name}.cc ${HEADERS} ${SOURCES})
      target_link_libraries(${name}
                            Qt4::QtCore
                            Qt4::QtGui
                            Qt4::QtOpenGL
                            ${EXTERNAL_LIBRARIES})
      add_custom_target(vis DEPENDS ${name})
    endif()
    install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_BINDIR})
  else()
    if(NOT QMAKE_EXE)
      set(QMAKE_EXE "QMAKE_EXE-NOTFOUND")
    endif()
    # look for Qt5-qmake
    find_package(Qt5Core QUIET)
    if(Qt5Core_FOUND)
      set(QMAKE_EXE ${Qt5Core_QMAKE_EXECUTABLE} CACHE STRING "QMake executable" FORCE)
    endif()
    # if not Qt5-qmake, search for Qt4
    if("${QMAKE_EXE}" STREQUAL "QMAKE_EXE-NOTFOUND")
      find_package(Qt4)
      set(QMAKE_EXE ${QT_QMAKE_EXECUTABLE} CACHE STRING "QMake executable" FORCE)
    endif()
    # if not Qt5-qmake and not Qt4, look for qmake explicitly
    if("${QMAKE_EXE}" STREQUAL "QMAKE_EXE-NOTFOUND")
      find_program(QMAKE_EXE
                   NAMES
                     qmake-qt4 qmake
                   DOC
                     "QMake executable")
    endif()

    add_subfeature(BUILD_GUI QMAKE_EXE "QMake executable")

    if(NOT "${QMAKE_EXE}" STREQUAL "QMAKE_EXE-NOTFOUND")
        add_custom_target(${name}_config
                          COMMAND ${QMAKE_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/${name}.pro
                          COMMENT "Generating ${name}.pro ..."
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${path})
        add_custom_target(${name} ALL
                          COMMAND ${CMAKE_MAKE_PROGRAM} -j8
                          COMMENT "Building ${name}..."
                          DEPENDS ${name}_config
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${path})

        if(APPLE)
            add_custom_command(TARGET ${name}
                               POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${path}/${name}.app/Contents/MacOS/${name}
                                ${CMAKE_BINARY_DIR}/)
        else()
            add_custom_command(TARGET ${name}
                               POST_BUILD
                               COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${path}/${name} ${CMAKE_BINARY_DIR}/)
        endif()
        add_custom_target(vis DEPENDS ${name})
    endif()
  endif()

endif()
