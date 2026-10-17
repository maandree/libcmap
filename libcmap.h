/* See LICENSE file for copyright and license details. */
#ifndef LIBCMAP_H
#define LIBCMAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


#if defined(__GNUC__)
# define LIBCMAP_PURE_ __attribute__((__pure__))
# define LIBCMAP_CONST_ __attribute__((__const__))
#else
# define LIBCMAP_PURE_
# define LIBCMAP_CONST_
#endif


/**
 * The last valid Unicode codepoint
 */
#define LIBCMAP_ULTIMATE_CODEPOINT UINT32_C(0x10FFFF)

/**
 * The range of all valid Unicode codepoints, include
 * assigned, unassigned, reserved, and surrogates.
 */
#define LIBCMAP_UNIVERSE_RANGE {0, LIBCMAP_ULTIMATE_CODEPOINT}

/**
 * Calculate the number of Unicode codepoints in a range
 * 
 * @param   RANGE:const libcmap_range *  The codepoint range
 * @return  :size_t                      The number of covered codepoints
 */
#define LIBCMAP_RANGE_SIZE(RANGE) ((size_t)((RANGE)->last - (RANGE)->first + 1u))


enum libcmap_general_supercategory {
	LIBCMAP_GENERAL_SUPERCATEGORY_LETTER = 0x0100u,
	LIBCMAP_GENERAL_SUPERCATEGORY_MARK = 0x0200u,
	LIBCMAP_GENERAL_SUPERCATEGORY_NUMBER = 0x0300u,
	LIBCMAP_GENERAL_SUPERCATEGORY_PUNCTUATION = 0x0400u,
	LIBCMAP_GENERAL_SUPERCATEGORY_SYMBOL = 0x0500u,
	LIBCMAP_GENERAL_SUPERCATEGORY_SEPARATOR = 0x0600u,
	LIBCMAP_GENERAL_SUPERCATEGORY_OTHER = 0x0700u
};

enum libcmap_general_category {
	LIBCMAP_GENERAL_CATEGORY_LETTER_UPPERCASE = LIBCMAP_GENERAL_SUPERCATEGORY_LETTER + 1u,
	LIBCMAP_GENERAL_CATEGORY_LETTER_LOWERCASE,
	LIBCMAP_GENERAL_CATEGORY_LETTER_TITLECASE,
	LIBCMAP_GENERAL_CATEGORY_LETTER_MODIFIER,
	LIBCMAP_GENERAL_CATEGORY_LETTER_OTHER,
	LIBCMAP_GENERAL_CATEGORY_MARK_NONSPACING = LIBCMAP_GENERAL_SUPERCATEGORY_MARK + 1u,
	LIBCMAP_GENERAL_CATEGORY_MARK_SPACING_COMBINING,
	LIBCMAP_GENERAL_CATEGORY_MARK_ENCODING,
	LIBCMAP_GENERAL_CATEGORY_NUMBER_DECIMAL_DIGIT = LIBCMAP_GENERAL_SUPERCATEGORY_NUMBER + 1u,
	LIBCMAP_GENERAL_CATEGORY_NUMBER_LETTER,
	LIBCMAP_GENERAL_CATEGORY_NUMBER_OTHER,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_CONNECTOR = LIBCMAP_GENERAL_SUPERCATEGORY_PUNCTUATION + 1u,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_DASH,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_OPEN,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_CLOSE,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_INITIAL_QUOTE,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_FINAL_QUOTE,
	LIBCMAP_GENERAL_CATEGORY_PUNCTUATION_OTHER,
	LIBCMAP_GENERAL_CATEGORY_SYMBOL_MATH = LIBCMAP_GENERAL_SUPERCATEGORY_SYMBOL + 1u,
	LIBCMAP_GENERAL_CATEGORY_SYMBOL_CURRENCY,
	LIBCMAP_GENERAL_CATEGORY_SYMBOL_MODIFIER,
	LIBCMAP_GENERAL_CATEGORY_SYMBOL_OTHER,
	LIBCMAP_GENERAL_CATEGORY_SEPARATOR_SPACE = LIBCMAP_GENERAL_SUPERCATEGORY_SEPARATOR + 1u,
	LIBCMAP_GENERAL_CATEGORY_SEPARATOR_LINE,
	LIBCMAP_GENERAL_CATEGORY_SEPARATOR_PARAGRAPH,
	LIBCMAP_GENERAL_CATEGORY_OTHER_CONTROL = LIBCMAP_GENERAL_SUPERCATEGORY_OTHER + 1u,
	LIBCMAP_GENERAL_CATEGORY_OTHER_FORMAT,
	LIBCMAP_GENERAL_CATEGORY_OTHER_SURROGATE,
	LIBCMAP_GENERAL_CATEGORY_OTHER_PRIVATE_USE,

	/**
	 * This category has two subcategories: noncharacter and reserved
	 * If .noncharacter it is not a character, otherwise it is reserved
	 */
	LIBCMAP_GENERAL_CATEGORY_OTHER_NOT_ASSIGNED
};

#define LIBCMAP_GENERAL_SUPERCATEGORY_MASK 0x7F00u


enum libcmap_bidirectional_supercategory {
	LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_STRONG = 0x0100u,
	LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_WEAK = 0x0200u,
	LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_NEUTRAL = 0x0300u,
	LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_EXPLICIT = 0x0400u
};

enum libcmap_bidirectional_category {
	LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT = LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_STRONG + 1u,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_ARABIC,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER = LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_WEAK + 1u,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER_SEPARATOR,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER_TERMINATOR,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_ARABIC_NUMBER,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_COMMON_SEPARATOR,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_NONSPACING_MARK,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_BOUNDARY_NEUTRAL,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_PARAGRAPH_SEPARATOR = LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_NEUTRAL + 1u,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_SEGMENT_SEPARATOR,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_WHITESPACE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_OTHER_NEUTRALS,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_EMBEDDING = LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_EXPLICIT + 1u,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_OVERRIDE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_EMBEDDING,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_OVERRIDE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_POP_DIRECTIONAL_FORMAT,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_ISOLATE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_ISOLATE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_FIRST_STRONG_ISOLATE,
	LIBCMAP_BIDIRECTIONAL_CATEGORY_POP_DIRECTIONAL_ISOLATE
};

#define LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_MASK 0x7F00u

enum libcmap_numeric_type {
	LIBCMAP_NUMERIC_TYPE_NONE,
	LIBCMAP_NUMERIC_TYPE_DECIMAL,
	LIBCMAP_NUMERIC_TYPE_DIGIT,
	LIBCMAP_NUMERIC_TYPE_NUMERIC
};

enum libcmap_decomposition_type {
	LIBCMAP_DECOMPOSITION_TYPE_CANONICAL,
	LIBCMAP_DECOMPOSITION_TYPE_FONT,
	LIBCMAP_DECOMPOSITION_TYPE_NO_BREAK,
	LIBCMAP_DECOMPOSITION_TYPE_INITIAL,
	LIBCMAP_DECOMPOSITION_TYPE_MEDIAL,
	LIBCMAP_DECOMPOSITION_TYPE_FINAL,
	LIBCMAP_DECOMPOSITION_TYPE_ISOLATED,
	LIBCMAP_DECOMPOSITION_TYPE_CIRCLE,
	LIBCMAP_DECOMPOSITION_TYPE_SUPER,
	LIBCMAP_DECOMPOSITION_TYPE_SUB,
	LIBCMAP_DECOMPOSITION_TYPE_VERTICAL,
	LIBCMAP_DECOMPOSITION_TYPE_WIDE,
	LIBCMAP_DECOMPOSITION_TYPE_NARROW,
	LIBCMAP_DECOMPOSITION_TYPE_SMALL,
	LIBCMAP_DECOMPOSITION_TYPE_SQUARE,
	LIBCMAP_DECOMPOSITION_TYPE_FRACTION,
	LIBCMAP_DECOMPOSITION_TYPE_COMPAT
};


/**
 * Contiguous range of Unicode codepoints
 *
 * @seealso  LIBCMAP_RANGE_SIZE
 * @seealso  libcmap_sprint_range
 */
struct libcmap_range {
	/**
	 * The lowest codepoint within the range
	 */
	uint32_t first;

	/**
	 * The highest codepoint within the range
	 */
	uint32_t last;
};

/**
 * Unicode character block
 * 
 * @seealso  libcmap_block_list
 * @seealso  libcmap_find_block
 */
struct libcmap_block {
	/**
	 * The name of the block
	 */
	const char *name;

	/**
	 * The range of codepoints the block covers
	 */
	struct libcmap_range range;
};

/**
 * Script and the codepoints used for it
 * 
 * @seealso  libcmap_script_list
 * @seealso  libcmap_find_script
 */
struct libcmap_script {
	/**
	 * The name of the script
	 *
	 * Replace any space with an underscore and remove
	 * all other non-alphanumeric characters to get
	 * the name as declared by Unicode
	 */
	const char *name;

	/**
	 * Sorted list of disjoint ranges of codepoints
	 * used for the script
	 * 
	 * The list is in ascending order
	 * 
	 * The is a non-zero gap of codepoints between
	 * each range
	 */
	const struct libcmap_range *ranges;

	/**
	 * The number of elements in `.ranges`
	 */
	size_t nranges;
};


/**
 * Information about a specific codepoint
 */
struct libcmap_codepoint_data {
	/**
	 * The name as published in the Unicode Standard code charts
	 *
	 * It uniquely identifies the character unless it starts with '<'
	 */
	const char *name;

	/* TODO documentation is needed */
	const char *unicode_1_name;
	const char *numeric_value;
	const uint32_t *decomposition_mapping;
	enum libcmap_general_category general_category : 16;
	enum libcmap_bidirectional_category bidirectional_category : 16;
	uint32_t simple_uppercase_mapping : 21;
	enum libcmap_numeric_type numeric_type : 4;
	enum libcmap_decomposition_type decomposition_type : 7;
	uint32_t simple_lowercase_mapping : 21;
	uint32_t canonical_combining_classes : 11;
	uint32_t simple_titlecase_mapping : 21;
	uint32_t decomposition_mapping_size : 6;
	uint32_t bidirectional_mirrored : 1;
	uint32_t noncharacter : 1;
	uint32_t : 3;
};


/**
 * Contiguous block of codepoints with
 * data for each codepoint
 */
struct libcmap_codepoint_segment {
	/**
	 * The range of codepoints the segment covers
	 */
	struct libcmap_range range;

	/**
	 * data for each codepoint in the segment,
	 *
	 * `.data[codepoint - .range.first]` gives the entry for
	 * a specific codepoint without [`.range.first, `.range.last`]
	 */
	const struct libcmap_codepoint_data *data;
};


/**
 * The version of the Unicode Standard that the library is
 * compiled with data for
 */
extern const char *const libcmap_unicode_version;

/**
 * Listing of all Unicode codepoints not assigned any block
 * 
 * This listing can potentially be empty
 * 
 * The name of this listing is called "No Block", in accordance
 * with the Unicode specification of it being called "No_Block"
 * 
 * @seealso  libcmap_block_list
 * @seealso  libcmap_find_in_no_block
 */
extern const struct libcmap_script libcmap_no_block;

/**
 * Listing of all Unicode blocks
 * 
 * The blocks are sorted, in ascending order by name
 * 
 * @seealso  libcmap_block_list_size
 * @seealso  libcmap_no_block
 * @seealso  libcmap_find_block
 * @seealso  libcmap_script_list
 */
extern const struct libcmap_block *const libcmap_block_list;

/**
 * The number of elements in `libcmap_block_list`
 */
extern const size_t libcmap_block_list_size;

/**
 * Listing of all so called scripts as divided by Unicode
 * 
 * Each codepoint only appears once: under their primary
 * script as assigned by Unicode
 * 
 * The scripts are sorted, in ascending order by name,
 * except "Unknown" is added as the last script
 * 
 * It contains three special ”scripts”: "Common", "Inherited",
 * and "Unknown". "Unknown" is an implicitly listed script
 * in Unicode, assigned to any codepoint that is not covered
 * by the other so called scripts; therefore it has been added
 * to the end of the list; however future versions of this
 * library may include it in order.
 * 
 * @seealso  libcmap_script_list_size
 * @seealso  libcmap_script_block
 * @seealso  libcmap_block_list
 */
extern const struct libcmap_script *const libcmap_script_list;

/**
 * The number of elements in `libcmap_script_list`
 */
extern const size_t libcmap_script_list_size;

/**
 * Data about codepoints
 *
 * There are gaps, hence the list is segmented
 *
 * The list is sorted in ascending order
 *
 * @seealso  libcmap_codepoint_list_size
 * @seealso  libcmap_find_codepoint_data
 */
extern const struct libcmap_codepoint_segment *const libcmap_codepoint_list;

/**
 * The number of elements in `libcmap_codepoint_list`
 */
extern const size_t libcmap_codepoint_list_size;


/**
 * Find a codepoint within `libcmap_no_block`
 * 
 * @param   codepoint     The codepoint to locate
 * @param   offset_out    Output parameter for the codepoints index within
 *                        `libcmap_no_block`; which is sum of the number
 *                        of codepoints in all ranges with `libcmap_no_block`
 *                        up to but excluding `*subrange_out`, plus
 *                        `codepoint - libcmap_no_block->ranges[*subrange_out].first`;
 *                        may be `NULL`
 * @param   subrange_out  Output parameter for the index of the range in
 *                        `libcmap_no_block` which covers the codepoint;
 *                        may be `NULL`
 * @return                1 if `libcmap_no_block` covers the codepoint,
 *                        0 otherwise
 * 
 * If 0 is returned, `*offset_out` and `*subrange_out` will not be set
 * 
 * @seealso  libcmap_find_block
 * @seealso  libcmap_find_script
 */
int libcmap_find_in_no_block(uint32_t codepoint, size_t *offset_out, size_t *subrange_out);

/**
 * Find the Unicode block that covers a specified codepoint
 * 
 * @param   codepoint   The codepoint to find the block for
 * @param   offset_out  Output parameter for the codepoints index within
 *                      the block; that is, the offset from the block's
 *                      lowest codepoint; may be `NULL`
 * @return              The block containing the codepoint,
 *                      `NULL` if not found
 * 
 * If `NULL` is returned, *offset_out` will not be set
 * 
 * The returned pointer (unless `NULL`) is `&libcmap_block_list[i]`
 * for some non-negative `i` less than `libcmap_block_list_size`
 * 
 * @seealso  libcmap_find_in_no_block
 * @seealso  libcmap_find_script
 */
const struct libcmap_block *libcmap_find_block(uint32_t codepoint, size_t *offset_out);

/**
 * Find the primary script, as assigned and divided by Unicode,
 * of a specified codepoint
 * 
 * @param   codepoint     The codepoint to find the primary script for
 * @param   offset_out    Output parameter for the codepoints index within
 *                        the script; which is sum of the number of 
 *                        codepoints in all ranges with script up to but
 *                        excluding `*subrange_out`, plus
 *                        `codepoint - script->ranges[*subrange_out].first`,
 *                        where `script` is the returned script;
 *                        may be `NULL`
 * @param   subrange_out  Output parameter for the index of the range in
 *                        the returned script which covers the codepoint;
 *                        may be `NULL`
 * @return                The codepoints primary script, `NULL` if not found
 *                        (should only be possible if the codepoint is
 *                        greater than `LIBCMAP_ULTIMATE_CODEPOINT`)
 * 
 * If 0 is returned, `*offset_out` and `*subrange_out` will not be set
 * 
 * The returned pointer (unless `NULL`) is `&libcmap_script_list[i]`
 * for some non-negative `i` less than `libcmap_script_list_size`
 * 
 * @seealso  libcmap_find_block
 */
const struct libcmap_script *libcmap_find_script(uint32_t codepoint, size_t *offset_out, size_t *subrange_out);


/**
 * Find data about a specified codepoint
 *
 * @param   codepoint  The codepoint
 * @return             Information from UnicodeData about the codepoint,
 *                     `NULL` if not found
 *
 * `NULL`-return doesn't necessary mean it is unassigned,
 * but rather just that it doesn't appear in UnicodeData,
 * which is common for ideographs
 * 
 * @seealso  libcmap_codepoint_list
 */
LIBCMAP_PURE_
const struct libcmap_codepoint_data *libcmap_find_codepoint_data(uint32_t codepoint);


/**
 * Print a string representing a Unicode codepoint range
 * 
 * @param   buf     Output buffer to print to, must be sufficiently large;
 *                  or `NULL` to only measure the length of the string
 * @param   range   The range to print a representation of
 * @param   endash  Desired delimiter between the first and the last
 *                  codepoint, or `NULL` for the default (not ASCII)
 * @return          The number of bytes in the string, excluding the
 *                  terminating NUL byte, -1 on failure
 * 
 * @throws  Any error specified for sprintf(3)
 * 
 * A NUL byte will added to the end of the string, but this byte
 * will not be counted in the return value
 */
int libcmap_sprint_range(char *buf, const struct libcmap_range *range, const char *endash);

/**
 * Print a string representing a Unicode codepoint range
 * 
 * @param   buf      Output buffer to print to
 * @param   bufsize  The maximum number of bytes to write to `buf`,
 *                   including the terminal NUL byte
 * @param   range    The range to print a representation of
 * @param   endash   Desired delimiter between the first and the last
 *                   codepoint, or `NULL` for the default (not ASCII)
 * @return           The number of bytes in the string, excluding the
 *                   terminating NUL byte, -1 on failure
 * 
 * @throws  Any error specified for snprintf(3)
 * 
 * A NUL byte will added to the end of the string, but this byte
 * will not be counted in the return value
 * 
 * If `bufsize` is not sufficiently large, string printed to
 * `buf` will be truncated to `bufsize - 1u` bytes, a NUL
 * byte will be added to the end (at `buf[bufsize - 1u`);
 * however if `bufsize` is 0, nothing is written to `buf`.
 * The function will always return the size the full string,
 * even if it was truncated; therefore the return value can
 * be equal to or exceed `bufsize`.
 * 
 * NB! if `buf` is `NULL`, `bufsize' shall be 0, as the function
 * does not check if `buf` is `NULL`
 */
int libcmap_snprint_range(char *buf, size_t bufsize, const struct libcmap_range *range, const char *endash);

/**
 * Print a string representing a Unicode codepoint range
 * 
 * @param   fp      The file to write to
 * @param   range   The range to print a representation of
 * @param   endash  Desired delimiter between the first and the last
 *                  codepoint, or `NULL` for the default (not ASCII)
 * @return          The number of bytes in the string, -1 on failure
 * 
 * @throws  Any error specified for fprintf(3)
 */
int libcmap_fprint_range(FILE *fp, const struct libcmap_range *range, const char *endash);

/**
 * Print, to standard output, a string representing a
 * Unicode codepoint range
 * 
 * @param   range   The range to print a representation of
 * @param   endash  Desired delimiter between the first and the last
 *                  codepoint, or `NULL` for the default (not ASCII)
 * @return          The number of bytes in the string, -1 on failure
 * 
 * @throws  Any error specified for fprintf(3)
 */
int libcmap_print_range(const struct libcmap_range *range, const char *endash);

/**
 * Print a string representing a Unicode codepoint range
 * 
 * @param   fd      The file descriptor
 * @param   range   The range to print a representation of
 * @param   endash  Desired delimiter between the first and the last
 *                  codepoint, or `NULL` for the default (not ASCII)
 * @return          The number of bytes in the string, -1 on failure
 * 
 * @throws  Any error specified for dprintf(3)
 */
int libcmap_dprint_range(int fd, const struct libcmap_range *range, const char *endash);


#undef LIBCMAP_PURE_
#undef LIBCMAP_CONST_
#endif
