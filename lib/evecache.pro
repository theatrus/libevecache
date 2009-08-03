CONFIG -= qt
TEMPLATE = lib
TARGET = evecache
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += evecache/reader.hpp evecache/parser.hpp evecache/exceptions.hpp
SOURCES += reader.cpp parser.cpp exceptions.cpp
win32:DEFINES += EVECACHE_EXPORT
