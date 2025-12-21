/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_print_range(const struct libcmap_range *range, const char *endash)
{
	return libcmap_fprint_range(stdout, range, endash);
}
