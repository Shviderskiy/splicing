TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra -Weffc++

QMAKE_LFLAGS += -static

INCLUDEPATH += headers

HEADERS += \
    tools/endian.hpp \
    api.hpp \
    tools/keywords_override_protection.hpp \
    jump.hpp \
    memory_permissions.hpp \
    tools/c++11.hpp \
    errors.hpp \
    tools/spin_lock.hpp \
    tools/static_initialization.hpp

SOURCES += \
    api.cpp \
    memory_permissions.cpp \
    errors.cpp \
    main.cpp
