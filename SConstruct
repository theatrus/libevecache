lenv = Environment(CPPPATH = ['lib/'])
env = Environment(CPPPATH = ['lib/'])
platform = ARGUMENTS.get('OS', Platform())

if platform.name == "win32":
   lenv.Append(CPPFLAGS=["/EHsc"])
   env.Append(CPPFLAGS=["/EHsc"])
   lenv.Append(CPPDEFINES=["EVECACHE_EXPORT"])

lib = lenv.SharedLibrary('evecache', Glob('lib/*cpp'))
env.Program('util/dumper.cpp', LIBS=lib)