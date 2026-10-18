/* See LICENSE file for copyright and license details. */
#include "common.h"


const char *
libcmap_decomposition_type_string(enum libcmap_decomposition_type value)
{
	switch (value) {
	case LIBCMAP_DECOMPOSITION_TYPE_CANONICAL: return "Canonical";
	case LIBCMAP_DECOMPOSITION_TYPE_FONT: return "Font";
	case LIBCMAP_DECOMPOSITION_TYPE_NO_BREAK: return "No-break";
	case LIBCMAP_DECOMPOSITION_TYPE_INITIAL: return "Initial";
	case LIBCMAP_DECOMPOSITION_TYPE_MEDIAL: return "Medial";
	case LIBCMAP_DECOMPOSITION_TYPE_FINAL: return "Final";
	case LIBCMAP_DECOMPOSITION_TYPE_ISOLATED: return "Isolated";
	case LIBCMAP_DECOMPOSITION_TYPE_CIRCLE: return "Circle";
	case LIBCMAP_DECOMPOSITION_TYPE_SUPER: return "Superscript";
	case LIBCMAP_DECOMPOSITION_TYPE_SUB: return "Subscript";
	case LIBCMAP_DECOMPOSITION_TYPE_VERTICAL: return "Vertical";
	case LIBCMAP_DECOMPOSITION_TYPE_WIDE: return "Wide";
	case LIBCMAP_DECOMPOSITION_TYPE_NARROW: return "Narrow";
	case LIBCMAP_DECOMPOSITION_TYPE_SMALL: return "Small";
	case LIBCMAP_DECOMPOSITION_TYPE_SQUARE: return "Square";
	case LIBCMAP_DECOMPOSITION_TYPE_FRACTION: return "Fraction";
	case LIBCMAP_DECOMPOSITION_TYPE_COMPAT: return "Compat";
	default: return NULL;
	}
}
