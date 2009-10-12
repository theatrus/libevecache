import distutils.sysconfig

lenv = Environment(CPPPATH = ['lib/'])
env = Environment(CPPPATH = ['lib/'])
pyenv = Environment(SWIGFLAGS=['-c++', '-python', '-classic'], CPPPATH = ['lib/', distutils.sysconfig.get_python_inc()], SHLIBPREFIX="")
platform = ARGUMENTS.get('OS', Platform())

if platform.name == "win32":
   lenv.Append(CPPFLAGS=["/EHsc", "/MD", "/O2", "/GF", "/GL", "/GR", "/arch:SSE"])
   env.Append(CPPFLAGS=["/EHsc", "/MD", "/O2", "/GF", "/GL", "/GR", "/arch:SSE"])
   lenv.Append(CPPDEFINES=["EVECACHE_EXPORT", "WIN32"])
   env.Append(CPPDEFINES=["WIN32"])
else:
   lenv.Append(CPPFLAGS=["-g3"])
   env.Append(CPPFLAGS=["-g3"])

lib = lenv.SharedLibrary('evecache', Glob('lib/*cpp'))
pyext = pyenv.SharedLibrary('_evecache.so', ['lib/libevecache.i'], LIBS=lib)

if platform.name == "win32":
   env.Program('util/dumper.cpp', LIBS='evecache') # try to link with .lib, not .dll
else:
   env.Program('util/dumper.cpp', LIBS=lib)
