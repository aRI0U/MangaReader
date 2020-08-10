QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/ui/MainWindow.cpp \
    src/ui/tabs/Tab.cpp \
    src/ui/tabs/maintab/MainTab.cpp \
    src/ui/tabs/maintab/MangaCover.cpp \
    src/ui/tabs/maintab/MangaEntry.cpp \
    src/ui/tabs/maintab/MangaList.cpp \
    src/ui/tabs/mangatab/MangaTab.cpp \
    src/ui/tabs/mangatab/PixmapLabel.cpp \
    src/ui/tabs/mangatab/Reader.cpp \
    src/utils/download/FileDownloader.cpp \
    src/utils/notifications/NotificationLayout.cpp \
    src/utils/notifications/NotificationManager.cpp \
    src/utils/notifications/NotificationWidget.cpp \
    src/utils/pagegroup/PageGrouper.cpp

HEADERS += \
    src/constants.h \
    src/ui/MainWindow.h \
    src/ui/tabs/Tab.h \
    src/ui/tabs/maintab/MainTab.h \
    src/ui/tabs/maintab/MangaCover.h \
    src/ui/tabs/maintab/MangaEntry.h \
    src/ui/tabs/maintab/MangaList.h \
    src/ui/tabs/mangatab/MangaTab.h \
    src/ui/tabs/mangatab/PixmapLabel.h \
    src/ui/tabs/mangatab/Reader.h \
    src/utils/download/FileDownloader.h \
    src/utils/notifications/NotificationLayout.h \
    src/utils/notifications/NotificationManager.h \
    src/utils/notifications/NotificationWidget.h \
    src/utils/pagegroup/PageGrouper.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
