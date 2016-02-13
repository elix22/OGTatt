LIBS += ../OGTatt/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
    ../OGTatt/Urho3D/include \
    ../OGTatt/Urho3D/include/Urho3D/ThirdParty \

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
*.cpp

HEADERS += \
*.h

OTHER_FILES += \
    Docs/Ideas.txt
