/* See LICENSE file for copyright and license details. */
#include "libcmap.h"


static const struct libcmap_block list[] = {
	{"Phony block", {0, 0}}
};


const struct libcmap_block *const libcmap_block_list = list;
const size_t libcmap_block_list_size = sizeof(list) / sizeof(*list);
