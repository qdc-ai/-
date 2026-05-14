cmake_minimum_required(VERSION 3.19)
project(spanlei LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# 关键：启用自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)  # 这个必须设置！用于编译 .qrc 文件

qt_add_executable(spanlei
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    cell.h
    cell.cpp
    resource.qrc
    gameconfig.h gameconfig.cpp
    gameborad.h gameborad.cpp gameborad.ui

    # 现在 AUTORCC ON 会正确处理这个文件
)

target_link_libraries(spanlei
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS spanlei
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET spanlei
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})