/* See LICENSE file for copyright and license details. */
#include "libcmap.h"
#include <stdio.h>


int
libcmap_snprint_range(char *buf, size_t bufsize, const struct libcmap_range *range, const char *endash)
{
        unsigned long int first = (unsigned long int)range->first;
	unsigned long int last = (unsigned long int)range->last;

	if (first == last)
		return snprintf(buf, bufsize, "U+%04lX", first);
	else
		return snprintf(buf, bufsize, "U+%04lX%sU+%04lX", first, endash ? endash : "–", last);
}
