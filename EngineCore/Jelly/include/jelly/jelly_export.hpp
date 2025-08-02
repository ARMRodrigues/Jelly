#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef JELLY_BUILD_DLL
    #define JELLY_EXPORT __declspec(dllexport)
  #else
    #define JELLY_EXPORT __declspec(dllimport)
  #endif
#else
  #define JELLY_EXPORT __attribute__((visibility("default")))
#endif