cmake_minimum_required(VERSION 3.5)

project(queens_game LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES queens_game_fr_FR.ts)

set(SRC src/)
set(INC include/)

file(GLOB GAMES "games/*")
add_custom_target(games SOURCES ${GAMES})

add_custom_target(Doc SOURCES "documentation/Doxyfile")

add_custom_target(Git SOURCES ".gitignore")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(ANDROID)
	add_library(queens_game SHARED
		${SRC}main.cpp
		${SRC}mainwindow.cpp
		${INC}mainwindow.h
		${SRC}mainwindow.ui
		${SRC}gamepresenter.cpp
		${INC}gamepresenter.h
		${SRC}gamemodel.cpp
		${INC}gamemodel.h
		${SRC}cellbutton.cpp
		${INC}cellbutton.h
		${TS_FILES}
	)
else()
	add_executable(queens_game
		${SRC}main.cpp
		${SRC}mainwindow.cpp
		${INC}mainwindow.h
		${SRC}mainwindow.ui
		${SRC}gamepresenter.cpp
		${INC}gamepresenter.h
		${SRC}gamemodel.cpp
		${INC}gamemodel.h
		${SRC}cellbutton.cpp
		${INC}cellbutton.h
		${TS_FILES}
	)
endif()

target_link_libraries(queens_game PRIVATE Qt5::Widgets)

target_include_directories(queens_game PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/queens_game_autogen/include
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
