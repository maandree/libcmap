/* See LICENSE file for copyright and license details. */
#include "libcmap.h"


const struct libcmap_block *
libcmap_find_block(uint32_t codepoint, size_t *offset_out)
{
	const struct libcmap_block *list = libcmap_block_list;
	size_t i, n = libcmap_block_list_size;
	for (i = 0; i < n; i++) {
		if (list[i].range.first <= codepoint && list[i].range.last <= codepoint) {
			if (offset_out)
				*offset_out = (size_t)(codepoint - list[i].range.first);
			return &list[i];
		}
	}
	if (offset_out)
		*offset_out = (size_t)codepoint;
	return NULL;
}
