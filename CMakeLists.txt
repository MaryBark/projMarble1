cmake_minimum_required(VERSION 3.5)

project(ProjackWork2OtusDoctor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# ПОДКЛЮЧАЕМ MARBLE ++++++++++++++++++++++++++++++++++++++++++
# указываем в нашем проекте где лежат дополнительные .cmake файлы
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" "${PROJECT_SOURCE_DIR}/import/cmake")

# выполняем поиск marble-библиотеки, см. выше файл FindMarble.cmake
find_package(Marble)
if (Marble_FOUND)
  message( STATUS "Marble includes: ${Marble_INCLUDE_DIR}")
  message( STATUS "Marble libraries: ${Marble_LIBRARIES}")
else()
  message(WARNING "Not found Marble!")
endif()

# настраиваем includes
include_directories("${Marble_INCLUDE_DIR}")  # заголовочные файлы marble-библиотеки


# ПОДКЛЮЧАЕМ MARBLE ++++++++++++++++++++++++++++++++++++++++++

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        coordinateVectors.h coordinateVectors.cpp
        MapMarbleTrassa.h MapMarbleTrassa.cpp
        orbit.h orbit.cpp definitions.h
        interaction2Database.h interaction2Database.cpp
        perturbationsKA.h perturbationsKA.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProjackWork2OtusDoctor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ProjackWork2OtusDoctor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ProjackWork2OtusDoctor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ProjackWork2OtusDoctor
            ${PROJECT_SOURCES}
        )
    endif()
endif()


# тоже от Marble
#target_link_libraries(ProjackWork2OtusDoctor ${Marble_LIBRARIES})
#_______________________________________________________________


target_link_libraries(ProjackWork2OtusDoctor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Marble_LIBRARIES})



set_target_properties(ProjackWork2OtusDoctor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProjackWork2OtusDoctor)
endif()
