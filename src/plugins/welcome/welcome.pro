include(../../mfds_plugin.pri)

SOURCES += welcomeplugin.cpp \
    introductionwidget.cpp

DEFINES += WELCOME_LIBRARY

RESOURCES += welcome.qrc

HEADERS += \
    introductionwidget.h
