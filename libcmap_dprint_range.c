/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_dprint_range(int fd, const struct libcmap_range *range, const char *endash)
{
	if (range->first == range->last)
		return dprintf(fd, SINGLETON_RANGE_FMT, SINGLETON_RANGE_ARGS(range));
	else
		return dprintf(fd, RANGE_FMT, RANGE_ARGS(range, endash));
}
