#pragma once

// Define export macro (for shared libraries)
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef JELLY_BUILD_DLL
    #define JELLY_API __declspec(dllexport)
  #else
    #define JELLY_API __declspec(dllimport)
  #endif
#else
  #define JELLY_API __attribute__((visibility("default")))
#endif

// C ABI compatibility
#ifdef __cplusplus
  #define JELLY_API_BEGIN extern "C" {
  #define JELLY_API_END   }
#else
  #define JELLY_API_BEGIN
  #define JELLY_API_END
#endif
