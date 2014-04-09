#ifndef strUtils_H
#define strUtils_H

#include <string.h>

#ifndef __cplusplus
	#define C_EXTERN extern
#else
	#define C_EXTERN extern "C"
#endif

C_EXTERN void *memMalloc(size_t size);
C_EXTERN char *formatString(const char *format, ...);
C_EXTERN void die(const char *fmt, ...);

#if defined(_MSC_VER) && _MSC_VER <= 1200
#define vsnprintf	_vsnprintf
#endif

#endif /*strUtils_H*/

