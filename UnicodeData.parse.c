/* See LICENSE file for copyright and license details. */
#include "parse-common.c"


struct segment {
	uint32_t first, last;
};


static struct segment *segments = NULL;
static size_t nsegments = 0u;
static int ret = 0;


static char *
split(char **text)
{
	char *field = *text;
	if (field) {
		*text = strchr(*text, ';');
		if (*text)
			*(*text)++ = '\0';
	}
	return field;
}


static int
output_character_data(char *text)
{
	char *field;

	if (!(field = split(&text))) return 0; /* codepoint */

	if (!(field = split(&text))) return 0;
	ret |= printf(".name = \"%s\"", field);

	if (!(field = split(&text))) return 0;
	ret |= printf(", .general_category = GENERAL_CATEGORY_%s", field);

	if (!(field = split(&text))) return 0;
	ret |= printf(", .canonical_combining_classes = %su", field);

	if (!(field = split(&text))) return 0;
	ret |= printf(", .bidirectional_category = BIDIRECTIONAL_CATEGORY_%s", field);

	if (!(field = split(&text))) return 0; /* TODO Character Decomposition Mapping */
	if (!(field = split(&text))) return 0; /* TODO Decimal digit value */
	if (!(field = split(&text))) return 0; /* TODO Digit value */
	if (!(field = split(&text))) return 0; /* TODO Numeric value */

	if (!(field = split(&text))) return 0;
	ret |= printf(", .bidirectional_mirrored = BIDIRECTIONAL_MIRRORED_%s", field);

	if (!(field = split(&text))) return 0;
	ret |= printf(", .unicode_1_name = \"%s\"", field);

	if (!(field = split(&text))) return 0; /* ISO 10646 comment field -- always empty as of Unicode 5.2.0 */

	if (!(field = split(&text))) return 0; /* TODO Uppercase Mapping */
	if (!(field = split(&text))) return 0; /* TODO Lowercase Mapping */
	if (!(field = split(&text))) return 0; /* TODO Titlecase Mapping */

	return 1;
}


static void
parse_line(char *text, size_t lineno)
{
	unsigned long int codepoint;
	char *end;

	if (!isxdigit(*text)) {
	malformat:
		fprintf(stderr, "%s: line %zu in is malformatted\n", argv0, lineno);
		exit(1);
	}

	errno = 0;

	codepoint = strtoul(text, &end, 16);
	if (errno || codepoint > ULTIMATE_CODEPOINT || *end != ';')
		goto malformat;

	if (!nsegments) {
	new_segment:
		ret |= printf("static const struct libcmap_codepoint_data data_from_%04lx[] = {\n", codepoint);
		segments = realloc(segments, ++nsegments * sizeof(*segments));
		if (!segments) {
			fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nsegments * sizeof(*segments), strerror(errno));
			exit(1);
		}
		segments[nsegments - 1u].first = (uint32_t)codepoint;
		goto new_codepoint;
	} else if (segments[nsegments - 1u].last + 1u == (uint32_t)codepoint) {
		ret |= printf(",\n");
	new_codepoint:
		segments[nsegments - 1u].last = (uint32_t)codepoint;
		ret |= printf("\t{");
		if (!output_character_data(text))
			goto malformat;
		ret |= printf("}");
	} else {
		if ((uint32_t)codepoint < segments[nsegments - 1u].last) {
			fprintf(stderr, "%s: UnicodeData is not sorted\n", argv0);
			exit(1);
		}
		ret |= printf("\n};\n");
		goto new_segment;
	}
}


static int
output(void)
{
	size_t i;

	if (!nsegments) {
		fprintf(stderr, "%s: character list is empty\n", argv0);
		exit(1);
	}

	ret |= printf("\n};\n");

	ret |= printf("static const struct libcmap_codepoint_segment list[] = {\n");
	for (i = 0u; i < nsegments; i++) {
		if (i)
			ret |= printf(",\n");
		ret |= printf("\t{.range = {0x%04"PRIX32", 0x%04"PRIX32"}, .data = data_from_%04"PRIx32"}",
		              segments[i].first, segments[i].last, segments[i].first);
	}
	ret |= printf("\n};\n");

	free(segments);

	return ret;
}
