QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 你可以在此修改你的项目名称
TARGET = ScenicManagementSystem
TEMPLATE = app

# 源文件和头文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scenicspot.cpp \
    scenicareagraph.cpp \
    scenicmapwidget.cpp

HEADERS += \
    mainwindow.h \
    scenicspot.h \
    scenicareagraph.h \
    scenicmapwidget.h

FORMS += \
    mainwindow.ui

# 默认规则
DISTFILES +=

# 发布设置
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/
else:unix: LIBS += -L$$OUT_PWD/

# 启用调试信息
CONFIG += debug
QMAKE_CXXFLAGS += -std=c++17

