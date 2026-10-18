/* See LICENSE file for copyright and license details. */
#include "libcmap.h"
#include <errno.h>
#include <limits.h>
#include <string.h>


#define SINGLETON_RANGE_FMT "U+%04lX"
#define SINGLETON_RANGE_ARGS(RANGE)\
	(unsigned long int)((RANGE)->first)

#define RANGE_FMT "U+%04lX%sU+%04lX"
#define RANGE_ARGS(RANGE, ENDASH)\
	(unsigned long int)((RANGE)->first),\
	((ENDASH) ? (ENDASH) : "–"),\
	(unsigned long int)((RANGE)->last)
