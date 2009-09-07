Welcome to libevecache, the EVE Cache Reader and Decoder Library!


Compiling
=========

There are two sets of build scripts available, SCons and qmake.

To build with scons, simply type in "scons" at the command line
(assuming scons is in your PATH). This will use your system compiler
to build a shared library (libevecache.so or evecache.dll) and the
"dumper" utility which is a starting ground for integrating
libevecache in your project.

The qmake scripts are divided between the "lib/" and "util/"
directory. They can be invoked by running "qmake", followed by the
make tool of your choosing (nmake, make, etc).

Compilers supported:
          - Microsoft Visual C++ 2008 (Express, Pro), 32bit
          - gcc/g++ 4.3.3, 32bit and 64bit, on Linux

Trying out Dumper
=================


After compiling both dumper and libevecache, the following commands
should work:

util/dumper "tests/Lonetrek-Armageddon-2009.07.30 062647.cache"
util/dumper --market "tests/Lonetrek-Armageddon-2009.07.30 062647.cache"

If you can't find the shared library under Linux, set your
LD_LIBRARY_PATH environment variable.


Integrating
==========

libevecache presents a C++ interface to the library. On Windows, there
is no C COM API available (sorry, contributions welcome).

Everything is in the "EveCache" namespace. libevecache depends on your
platform STL for classes such as vector, string, and
stringstream.

For examples of how to use the library in the absense of other
documents, refer to the dumper utility located in "util/".


Support
=======

E-mail  : atrus at stackworks.net
IRC     : #evecentral on irc.freenode.org or #eve-dev on irc.coldfont.net
Code    : http://gitorious.org/libevecache
Web     : http://dev.eve-central.com/libevecache/
