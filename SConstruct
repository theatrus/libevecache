env = Environment(CPPPATH = ['lib/'])

lib = env.SharedLibrary('evecache', Glob('lib/*cpp'))
env.Program('util/dumper.cpp', LIBS=lib)