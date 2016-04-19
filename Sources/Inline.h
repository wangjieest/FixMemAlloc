#ifndef INCLUDE_H
#define INCLUDE_H

#ifndef INLINE
  #if defined(__cplusplus) || ((__STDC_VERSION__) >= 199901L)
    #if ((_MSC_VER) >= 1200)
      #define INLINE __inline
    #else
      #define INLINE inline
    #endif
  #else
    #define INLINE static
  #endif
#endif

#endif
