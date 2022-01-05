include(../../mfds_plugin.pri)

HEADERS += \
    exportdialog.h \
    imageviewerplugin.h \
    imageviewerfactory.h \
    imageviewerfile.h \
    imageviewer.h \
    imageview.h \
    imageviewerconstants.h \
    multiexportdialog.h

SOURCES += \
    exportdialog.cpp \
    imageviewerplugin.cpp \
    imageviewerfactory.cpp \
    imageviewerfile.cpp \
    imageviewer.cpp \
    imageview.cpp \
    multiexportdialog.cpp

!isEmpty(QT.svg.name) {
    QT += svg
    greaterThan(QT_MAJOR_VERSION, 5) : QT += svgwidgets
} else {
    DEFINES += QT_NO_SVG
}

FORMS += \
    imageviewertoolbar.ui
