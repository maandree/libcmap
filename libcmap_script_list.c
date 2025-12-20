/* See LICENSE file for copyright and license details. */
#include "libcmap.h"


static const struct libcmap_script list[] = {
	{"Phony script", NULL, 0}
};


const struct libcmap_script *const libcmap_script_list = list;
const size_t libcmap_script_list_size = sizeof(list) / sizeof(*list);
