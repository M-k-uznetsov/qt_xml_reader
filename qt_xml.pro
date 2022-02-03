QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    TreeNode.cpp \
    parser.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    TreeNode.h \
    parser.h \
    mainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
