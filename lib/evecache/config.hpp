#ifndef _EC_CONFIG_H_
#define _EC_CONFIG_H_

#ifdef WIN32

#if defined(EVECACHE_EXPORT) // inside DLL
#   define EVECACHE_API   __declspec(dllexport)
#else // outside DLL
#   define EVECACHE_API   __declspec(dllimport)
#endif
#else

#define EVECACHE_API

#endif



#endif
