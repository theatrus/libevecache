CONFIG -= qt
TEMPLATE = app
TARGET = dumper
DEPENDPATH += .
INCLUDEPATH += ../lib
LDPATH += ../lib
LIBS += -L../lib -levecache
# Input
HEADERS += dumper.hpp
SOURCES += dumper.cpp
