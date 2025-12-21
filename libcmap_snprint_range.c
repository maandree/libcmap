/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_snprint_range(char *buf, size_t bufsize, const struct libcmap_range *range, const char *endash)
{
	if (range->first == range->last)
		return snprintf(buf, bufsize, SINGLETON_RANGE_FMT, SINGLETON_RANGE_ARGS(range));
	else
		return snprintf(buf, bufsize, RANGE_FMT, RANGE_ARGS(range, endash));
}
