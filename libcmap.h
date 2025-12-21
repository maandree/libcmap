/* See LICENSE file for copyright and license details. */
#ifndef LIBCMAP_H
#define LIBCMAP_H

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__)
# define LIBCMAP_PURE_ __attribute__((__pure__))
# define LIBCMAP_CONST_ __attribute__((__const__))
#else
# define LIBCMAP_PURE_
# define LIBCMAP_CONST_
#endif


#define LIBCMAP_ULTIMATE_CODEPOINT UINT32_C(0x10FFFF)
#define LIBCMAP_UNIVERSE_RANGE {0, LIBCMAP_ULTIMATE_CODEPOINT}

#define LIBCMAP_RANGE_SIZE(RANGE) ((size_t)((RANGE)->last - (RANGE)->first + 1U))


struct libcmap_range {
	uint32_t first;
	uint32_t last;
};

struct libcmap_block {
	const char *name;
	struct libcmap_range range;
};

struct libcmap_script {
	const char *name;
	const struct libcmap_range *ranges;
	size_t nranges;
};


extern const struct libcmap_script libcmap_no_block;
extern const struct libcmap_block *const libcmap_block_list;
extern const size_t libcmap_block_list_size;

extern const struct libcmap_script *const libcmap_script_list;
extern const size_t libcmap_script_list_size;


int libcmap_find_in_no_block(uint32_t codepoint, size_t *offset_out, size_t *subrange_out);
const struct libcmap_block *libcmap_find_block(uint32_t codepoint, size_t *offset_out);
const struct libcmap_script *libcmap_find_script(uint32_t codepoint, size_t *offset_out, size_t *subrange_out);

int libcmap_sprint_range(char *buf, const struct libcmap_range *range, const char *endash);
int libcmap_snprint_range(char *buf, size_t bufsize, const struct libcmap_range *range, const char *endash);


#undef LIBCMAP_PURE_
#undef LIBCMAP_CONST_
#endif
