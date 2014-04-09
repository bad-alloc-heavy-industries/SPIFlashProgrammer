#include <stdarg.h>
#include <stdlib.h>
#include "strUtils.h"

void *memMalloc(size_t size)
{
	void *ret = malloc(size);
	if (ret == NULL)
		die("Could not allocate enough memory");
	memset(ret, 0, size);
	return ret;
}

char *formatString(const char *format, ...)
{
	int len;
	char *ret;
	va_list args;
	va_start(args, format);
	len = vsnprintf(NULL, 0, format, args);
	va_end(args);
	ret = (char *)memMalloc(len + 1);
	va_start(args, format);
	vsprintf(ret, format, args);
	va_end(args);
	return ret;
}

void die(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	exit(1);
}

