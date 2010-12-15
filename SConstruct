#!/usr/bin/env python
import distutils.sysconfig
import os
import re
import sys

lenv = Environment(CPPPATH = ['lib/'])
env = Environment(CPPPATH = ['lib/'])
pyenv = Environment( SWIGFLAGS=['-c++', '-python', '-classic'], CPPPATH = ['lib/', distutils.sysconfig.get_python_inc()], SHLIBPREFIX="")
platform = ARGUMENTS.get('OS', Platform())


if platform.name == "win32":
   lenv.Append(CPPFLAGS=["/EHsc", "/MD", "/O2", "/GF", "/GL", "/GR", "/arch:SSE"])
   pyenv.Append(CPPFLAGS=["/EHsc", "/MD", "/O2", "/GF", "/GL", "/GR", "/arch:SSE"])
   env.Append(CPPFLAGS=["/EHsc", "/MD", "/O2", "/GF", "/GL", "/GR", "/arch:SSE"])
   lenv.Append(CPPDEFINES=["EVECACHE_EXPORT", "WIN32"])
   env.Append(CPPDEFINES=["WIN32"])
   pyenv.Append(CPPDEFINES=["WIN32"])
   pyenv.Append(LIBPATH=['c:/python26/libs/'])
else:
   lenv.Append(CPPFLAGS=["-g3", "-Wall"])
   env.Append(CPPFLAGS=["-g3", "-Wall"])

lib = lenv.SharedLibrary('evecache', Glob('lib/*cpp'))

# SCONS whacky issue with Windows. I sometimes really dislike scons...
if platform.name == "win32":
   pyext = pyenv.SharedLibrary('_evecache', ['lib/libevecache_wrap.cxx'], LIBS='evecache')
else:
   pyext = pyenv.SharedLibrary('_evecache', ['lib/libevecache.i'], LIBS=[lib, 'python'])

if platform.name == "win32":
   env.Program('util/dumper.cpp', LIBS='evecache') # try to link with .lib, not .dll
else:
   env.Program('util/dumper.cpp', LIBS=lib)
