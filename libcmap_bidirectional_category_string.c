/* See LICENSE file for copyright and license details. */
#include "common.h"


const char *
libcmap_bidirectional_category_string(enum libcmap_bidirectional_category value)
{
	switch ((enum libcmap_bidirectional_supercategory)value) {
	case LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_STRONG: return "strong";
	case LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_WEAK: return "weak";
	case LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_NEUTRAL: return "neutral";
	case LIBCMAP_BIDIRECTIONAL_SUPERCATEGORY_EXPLICIT: return "explicit";
	default: break;
	}

	switch (value) {
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT: return "Left-to-right";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT: return "Right-to-left";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_ARABIC: return "Arabic right-to-left";

	case LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER: return "European number";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER_SEPARATOR: return "European number separator";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_EUROPEAN_NUMBER_TERMINATOR: return "European number terminator";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_ARABIC_NUMBER: return "Arabic number";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_COMMON_SEPARATOR: return "Common separator";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_NONSPACING_MARK: return "Non-spacing mark";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_BOUNDARY_NEUTRAL: return "Boundary neutral";

	case LIBCMAP_BIDIRECTIONAL_CATEGORY_PARAGRAPH_SEPARATOR: return "Paragraph separator";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_SEGMENT_SEPARATOR: return "Segment separator";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_WHITESPACE: return "Whitespace";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_OTHER_NEUTRALS: return "Other neutrals";

	case LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_EMBEDDING: return "Left-to-right embedding";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_OVERRIDE: return "Left-to-right override";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_EMBEDDING: return "Right-to-left embedding";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_OVERRIDE: return "Right-to-left override";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_POP_DIRECTIONAL_FORMAT: return "Pop directional format";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_LEFT_TO_RIGHT_ISOLATE: return "Left-to-right isolate";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_RIGHT_TO_LEFT_ISOLATE: return "Right-to-left isolate";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_FIRST_STRONG_ISOLATE: return "First strong isolate";
	case LIBCMAP_BIDIRECTIONAL_CATEGORY_POP_DIRECTIONAL_ISOLATE: return "Pop directional isolate";

	default: return NULL;
	} 
}
