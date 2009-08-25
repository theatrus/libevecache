lenv = Environment(CPPPATH = ['lib/'])
env = Environment(CPPPATH = ['lib/'])
platform = ARGUMENTS.get('OS', Platform())

if platform.name == "win32":
   lenv.Append(CPPFLAGS=["/EHsc", "/MD"])
   env.Append(CPPFLAGS=["/EHsc", "/MD"])
   lenv.Append(CPPDEFINES=["EVECACHE_EXPORT", "WIN32"])
   env.Append(CPPDEFINES=["WIN32"])

lib = lenv.SharedLibrary('evecache', Glob('lib/*cpp'))
if platform.name == "win32":
   env.Program('util/dumper.cpp', LIBS='evecache') # try to link with .lib, not .dll
else:
   env.Program('util/dumper.cpp', LIBS=lib)
