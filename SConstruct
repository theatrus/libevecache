env = Environment(CPPPATH = ['lib/'])
platform = ARGUMENTS.get('OS', Platform())

if platform.name == "win32":
   env.Append(CPPFLAGS=["/EHsc"])

lib = env.SharedLibrary('evecache', Glob('lib/*cpp'))
env.Program('util/dumper.cpp', LIBS=lib)