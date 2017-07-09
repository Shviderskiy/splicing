TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra -Weffc++

QMAKE_LFLAGS += -static

INCLUDEPATH += headers

HEADERS += \
    headers/tools/c++11.hpp \
    headers/tools/keywords_override_protection.hpp \
    headers/tools/endian.hpp \
    headers/tools/spin_lock.hpp \
    headers/tools/static_initialization.hpp \
    headers/api.hpp \
    headers/jump.hpp \
    headers/memory_permissions.hpp \
    headers/errors.hpp

SOURCES += \
    sources/api.cpp \
    sources/memory_permissions.cpp \
    sources/errors.cpp \
    tests/main.cpp
