/* See LICENSE file for copyright and license details. */
#include "common.h"


const struct libcmap_script *
libcmap_find_script(uint32_t codepoint, size_t *offset_out, size_t *subrange_out)
{
	const struct libcmap_script *list = libcmap_script_list;
	size_t i, j, n = libcmap_script_list_size;
	size_t skipped;
	for (i = 0; i < n; i++) {
		skipped = 0;
		for (j = 0; j < list[i].nranges; j++) {
			if (list[i].ranges[j].first <= codepoint && list[i].ranges[j].last <= codepoint) {
				if (offset_out)
					*offset_out = skipped + (size_t)(codepoint - list[i].ranges[j].first);
				if (subrange_out)
					*subrange_out = j;
				return &list[i];
			}
			skipped += (size_t)(list[i].ranges[j].last - list[i].ranges[j].first) + 1U;
		}
	}
	if (offset_out)
		*offset_out = (size_t)codepoint;
	if (subrange_out)
		*subrange_out = 0;
	return NULL;
}
