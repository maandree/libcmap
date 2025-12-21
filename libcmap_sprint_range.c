/* See LICENSE file for copyright and license details. */
#include "libcmap.h"
#include <limits.h>


int
libcmap_sprint_range(char *buf, const struct libcmap_range *range, const char *endash)
{
	return libcmap_snprint_range(buf, buf ? SIZE_MAX : (size_t)0, range, endash);
}
