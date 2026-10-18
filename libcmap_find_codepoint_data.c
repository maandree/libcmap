/* See LICENSE file for copyright and license details. */
#include "common.h"


const struct libcmap_codepoint_data *
libcmap_find_codepoint_data(uint32_t codepoint)
{
	size_t low = 0u, high = libcmap_codepoint_list_size - 1u;
	size_t i;

	while (low <= high) {
		i = low + (high - low) / 2u;
		if (libcmap_codepoint_list[i].range.last < codepoint) {
			low = i + 1u;
		} else if (libcmap_codepoint_list[i].range.first > codepoint) {
			if (!high)
				break;
			high = i - 1u;
		} else {
			return &libcmap_codepoint_list[i].data[codepoint - libcmap_codepoint_list[i].range.first];
		}
	}

	return NULL;
}
