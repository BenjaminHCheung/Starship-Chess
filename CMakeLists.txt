cmake_minimum_required(VERSION 3.4)
project(Starship-Chess)

FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE( OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)

INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

IF (NOT TARGET object-physics)
    #git submodule add "GitHub path to your Palindrome Library"
    #git submodule update --init --recursive

    add_subdirectory(libs/Stellar)
    INCLUDE_DIRECTORIES(libs/Stellar)
ENDIF()


SET(MYSOURCE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    osgwidget.h
    osgwidget.cpp
    sphereupdatecallback.h
    sphereupdatecallback.cpp
    shipupdatecallback.h
    shipupdatecallback.cpp
    )

set(MYRESOURCES
    TextureMap/TextureMapping.qrc
    )

set( MYRESOURCES TextureMap)

add_executable(${PROJECT_NAME}
    ${MYSOURCE}
    ${MYRESOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}	
    Qt5::Widgets
    Qt5::Gui
    Stellar
)
