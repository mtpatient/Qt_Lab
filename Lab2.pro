QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about_dialog.cpp \
    codeeditor.cpp \
    find_dialog.cpp \
    history_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    replace_dialog.cpp

HEADERS += \
    about_dialog.h \
    codeeditor.h \
    find_dialog.h \
    history_dialog.h \
    mainwindow.h \
    replace_dialog.h

FORMS += \
    about_dialog.ui \
    find_dialog.ui \
    history_dialog.ui \
    mainwindow.ui \
    replace_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    la2_res.qrc

