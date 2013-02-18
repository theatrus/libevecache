#ifndef _EC_CONFIG_H_
#define _EC_CONFIG_H_

#if defined(WIN32) && defined(EVECACHE_DLL)

#if defined(EVECACHE_EXPORT) // inside DLL
#   define EVECACHE_API   __declspec(dllexport)
#else // outside DLL
#   define EVECACHE_API   __declspec(dllimport)
#endif
#else

#define EVECACHE_API

#endif



#endif
