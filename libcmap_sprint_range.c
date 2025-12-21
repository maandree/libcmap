/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_sprint_range(char *buf, const struct libcmap_range *range, const char *endash)
{
	if (!buf)
		return libcmap_snprint_range(NULL, 0, range, endash);
	else if (range->first == range->last)
		return sprintf(buf, SINGLETON_RANGE_FMT, SINGLETON_RANGE_ARGS(range));
	else
		return sprintf(buf, RANGE_FMT, RANGE_ARGS(range, endash));
}
