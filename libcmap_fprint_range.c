/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_fprint_range(FILE *fp, const struct libcmap_range *range, const char *endash)
{
	if (range->first == range->last)
		return fprintf(fp, SINGLETON_RANGE_FMT, SINGLETON_RANGE_ARGS(range));
	else
		return fprintf(fp, RANGE_FMT, RANGE_ARGS(range, endash));
}
