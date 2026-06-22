find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Svg Concurrent DBus REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(dde-splash
    main.cpp
    splashwidget.h
    splashwidget.cpp
)

target_link_libraries(dde-splash
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::DBus
)

install(TARGETS dde-splash DESTINATION ${CMAKE_INSTALL_BINDIR})
