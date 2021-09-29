TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra -Weffc++

QMAKE_LFLAGS += -static

INCLUDEPATH += include

HEADERS += \
    include/tools/c++11.hpp \
    include/tools/keywords_override_protection.hpp \
    include/tools/endian.hpp \
    include/tools/spin_lock.hpp \
    include/tools/static_initialization.hpp \
    include/api.hpp \
    include/jump.hpp \
    include/memory_permissions.hpp \
    include/errors.hpp

SOURCES += \
    srs/api.cpp \
    srs/memory_permissions.cpp \
    srs/errors.cpp \
    tests/main.cpp
