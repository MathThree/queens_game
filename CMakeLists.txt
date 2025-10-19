cmake_minimum_required(VERSION 3.16)
project(queens_game LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Detection of Qt 5 or Qt 6 ---
find_package(Qt6 COMPONENTS Widgets LinguistTools QUIET)
if(Qt6_FOUND)
	message(STATUS "Using Qt 6")
	find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
    set(QT_VERSION_MAJOR 6)
else()
    message(STATUS "Using Qt 5")
	find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
    set(QT_VERSION_MAJOR 5)
endif()

# --- Translation and source files ---
set(TS_FILES i18n/queens_game_fr_FR.ts)
set(SRC src/)
set(INC include/)

file(GLOB GAMES "games/*")
add_custom_target(games SOURCES ${GAMES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Qt Ressources ---
if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_add_resources(RESOURCES games/gamesData.qrc)
else()
    qt5_add_resources(RESOURCES games/gamesData.qrc)
endif()

# --- Application files ---
set(APP_SOURCES
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
    ${SRC}gridcontainer.cpp
    ${INC}gridcontainer.h
    ${SRC}gridwidget.cpp
    ${INC}gridwidget.h
    ${SRC}levelselector.cpp
    ${INC}levelselector.h
    ${SRC}levelselector.ui
    ${SRC}levelbutton.cpp
    ${INC}levelbutton.h
    ${TS_FILES}
    ${RESOURCES}
)

# --- Target definition ---
if(${QT_VERSION_MAJOR} EQUAL 6)
	qt_standard_project_setup()
	qt_add_executable(queens_game
		WIN32 MACOSX_BUNDLE
                ${APP_SOURCES}
	)
else()
	add_executable(queens_game
		WIN32 MACOSX_BUNDLE
		${APP_SOURCES}
	)
endif()


# --- Linker ---
if(${QT_VERSION_MAJOR} EQUAL 6)
	target_link_libraries(queens_game PRIVATE Qt::Core Qt::Widgets)
else()
	target_link_libraries(queens_game PRIVATE Qt5::Widgets)
endif()

# --- WebAssembly deployment ---
if(${QT_VERSION_MAJOR} EQUAL 6)
	qt_generate_deploy_app_script(
		TARGET queens_game
		OUTPUT_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/deploy_script
		NO_UNSUPPORTED_PLATFORM_ERROR
		)
endif()

# --- Auto-generated headers ---
target_include_directories(queens_game PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/queens_game_autogen/include
)
