/* See LICENSE file for copyright and license details. */
#include "common.h"


const char *
libcmap_numeric_type_string(enum libcmap_numeric_type value)
{
	switch (value) {
	case LIBCMAP_NUMERIC_TYPE_NONE: return "None";
	case LIBCMAP_NUMERIC_TYPE_DECIMAL: return "Decimal";
	case LIBCMAP_NUMERIC_TYPE_DIGIT: return "Digit";
	case LIBCMAP_NUMERIC_TYPE_NUMERIC: return "Numeric";
	default: return NULL;
	}
}
