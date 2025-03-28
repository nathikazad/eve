#ifdef _MSC_VER
#ifndef FT8XXEMU_INTTYPES_DEFINED_BASE
#define FT8XXEMU_INTTYPES_DEFINED_BASE
typedef unsigned __int8 uint8_t;
typedef signed __int8 int8_t;
typedef unsigned __int16 uint16_t;
typedef signed __int16 int16_t;
typedef unsigned __int32 uint32_t;
typedef signed __int32 int32_t;
typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;
#endif
#else
#include <stdint.h>
#include <stdlib.h>
#endif
#ifndef FT8XXEMU_INTTYPES_DEFINED_COLOR
#define FT8XXEMU_INTTYPES_DEFINED_COLOR
typedef uint32_t argb8888;
#endif
#ifndef FT8XXEMU_INTTYPES_DEFINED_FORCEINLINE
#define FT8XXEMU_INTTYPES_DEFINED_FORCEINLINE
#ifdef _MSC_VER
#	define FT8XXEMU_FORCE_INLINE __forceinline
#else
#	define FT8XXEMU_FORCE_INLINE inline __attribute__((always_inline))
#endif
#endif
