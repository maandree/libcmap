/* See LICENSE file for copyright and license details. */
#include "common.h"


#include "Blocks.txt.c"

const struct libcmap_block *const libcmap_block_list = list;
const size_t libcmap_block_list_size = sizeof(list) / sizeof(*list);
