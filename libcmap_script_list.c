/* See LICENSE file for copyright and license details. */
#include "common.h"


#include "Scripts.txt.c"

const struct libcmap_script *const libcmap_script_list = list;
const size_t libcmap_script_list_size = sizeof(list) / sizeof(*list);
