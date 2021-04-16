QT += quick charts

CONFIG += c++17

SOURCES += \
    core/simplewordcounter.cpp \
    core/wordcounterproxy.cpp \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RC_ICONS = icon.ico

RESOURCES += \
    ui/qml.qrc

HEADERS += \
    core/simplewordcounter.h \
    core/wordcounterproxy.h
