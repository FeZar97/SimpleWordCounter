QT += quick charts

CONFIG += c++17

SOURCES += \
        main.cpp \
        simplewordcounter.cpp \
        wordcounterproxy.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    simplewordcounter.h \
    wordcounterproxy.h

RC_ICONS = icon.ico
