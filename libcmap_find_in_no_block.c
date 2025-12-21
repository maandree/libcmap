/* See LICENSE file for copyright and license details. */
#include "common.h"


int
libcmap_find_in_no_block(uint32_t codepoint, size_t *offset_out, size_t *subrange_out)
{
	size_t i, skipped = 0;
	for (i = 0; i < libcmap_no_block.nranges; i++) {
		if (libcmap_no_block.ranges[i].first <= codepoint && libcmap_no_block.ranges[i].last <= codepoint) {
			if (offset_out)
				*offset_out = skipped + (size_t)(codepoint - libcmap_no_block.ranges[i].first);
			if (subrange_out)
				*subrange_out = i;
			return 1;
		}
		skipped += (size_t)(libcmap_no_block.ranges[i].last - libcmap_no_block.ranges[i].first) + 1U;
	}
	return 0;
}
