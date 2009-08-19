CONFIG -= qt
TEMPLATE = lib
TARGET = evecache
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += evecache/reader.hpp evecache/parser.hpp evecache/exceptions.hpp evecache/dbtypes.hpp \
           evecache/market.hpp
SOURCES += reader.cpp parser.cpp exceptions.cpp dbtypes.cpp market.cpp
win32:DEFINES += EVECACHE_EXPORT
