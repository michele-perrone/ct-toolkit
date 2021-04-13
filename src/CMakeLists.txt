cmake_minimum_required(VERSION 3.5)

project(CT-TOOLKIT LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 COMPONENTS Widgets LinguistTools REQUIRED) # (Optional) Insert Qt5
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

#set(TS_FILES qt-widgets-test_it_IT.ts)

set(PROJECT_SOURCES
        main.cpp

        app/quadtree.cpp
        app/quadtree.hpp
        app/binarytools.cpp
        app/binarytools.hpp
        app/bmp.cpp
        app/bmp.hpp
        app/imgtools.cpp
        app/imgtools.hpp

        gui/mainwindow.cpp
        gui/mainwindow.h
        gui/mainwindow.ui
        ${TS_FILES}
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CT-TOOLKIT
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(CT-TOOLKIT SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(CT-TOOLKIT
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(CT-TOOLKIT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
