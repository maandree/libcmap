/* See LICENSE file for copyright and license details. */
#include "parse-common.c"

/* As of Unicode 18.0.0, splitting decomposition_mapping into 32-bit and 16-bit
 * only saves just shy of 11 KB, therefore it has been decided to keep the API
 * simple by always using 32 */


#define LONGEST_DECOMPOSITION 18u /* Unicode makes this guarantee */


struct segment {
	uint32_t first, last;
};

struct sequence {
	char *string;
	size_t len;
};


static struct segment *segments = NULL;
static size_t nsegments = 0u;
static int ret = 0;
static char **lines = NULL;
static size_t nlines = 0u;
static struct sequence *sequences = NULL;
static size_t nsequences = 0u;


#if defined(__GNUC__)
__attribute__((__format__(gnu_printf, 1, 2)))
#endif
static void
add_linef(const char *fmt, ...)
{
	va_list args1, args2;
	int len, len2;
	char *text;
	va_start(args1, fmt);
	va_copy(args2, args1);
	len = vsnprintf(NULL, 0u, fmt, args2);
	if (len < 0) {
		fprintf(stderr, "%s: snprintf: %s\n", argv0, strerror(errno));
		exit(1);
	}
	va_end(args2);
	text = malloc((size_t)len + 1u);
	if (!text) {
		fprintf(stderr, "%s: malloc %zu: %s\n", argv0, (size_t)len + 1u, strerror(errno));
		exit(1);
	}
	len2 = vsnprintf(text, (size_t)len + 1u, fmt, args1);
	if (len2 < 0) {
		fprintf(stderr, "%s: snprintf: %s\n", argv0, strerror(errno));
		exit(1);
	} else if (len2 != len) {
		fprintf(stderr, "abort on line %i\n", __LINE__);
		abort();
	}
	va_end(args1);
	lines = realloc(lines, ++nlines * sizeof(*lines));
	if (!lines) {
		fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nlines * sizeof(*lines), strerror(errno));
		exit(1);
	}
	lines[nlines - 1u] = text;
}


static void
add_sequence(const char *sequence, size_t len)
{
	sequences = realloc(sequences, ++nsequences * sizeof(*sequences));
	if (!sequences) {
		fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nsequences * sizeof(*sequences), strerror(errno));
		exit(1);
	}
	sequences[nsequences - 1u].string = strdup(sequence);
	if (!sequences[nsequences - 1u].string) {
		fprintf(stderr, "%s: strdup: %s\n", argv0, strerror(errno));
		exit(1);
	}
	sequences[nsequences - 1u].len = len;
}


static const char *
get_decomposition_type(const char *text)
{
	static char buf[64];
	size_t i;

	if (*text++ != '<') return NULL;
	for (i = 0u; *text && *text != '>'; i++, text++) {
		if (i == sizeof(buf)) return NULL;
		if ('A' <= *text && *text <= 'Z') {
			buf[i++] = '_';
			if (i == sizeof(buf)) return NULL;
			buf[i] = *text;
		} else if ('a' <= *text && *text <= 'z') {
			buf[i] = (char)(*text + ('A' - 'a'));
		} else {
			return NULL;
		}
	}

	if (*text != '>') return NULL;
	if (i == sizeof(buf)) return NULL;
	buf[i] = '\0';
	return buf;
}


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
	char *field, *numeric_value = NULL, *simple_uppercase_mapping;
	const char *formatted;
	size_t n, i;

	if (!(field = split(&text))) return 0; /* codepoint */

	if (!(field = split(&text))) return 0;
	add_linef(".name = \"%s\"", field);

	if (!(field = split(&text))) return 0;
	add_linef(", .general_category = GENERAL_CATEGORY_%s", field);

	if (!(field = split(&text))) return 0;
	add_linef(", .canonical_combining_classes = %su", field);

	if (!(field = split(&text))) return 0;
	add_linef(", .bidirectional_category = BIDIRECTIONAL_CATEGORY_%s", field);

	if (!(field = split(&text))) return 0;
	if (!*field) {
		add_linef(", .decomposition_type = LIBCMAP_DECOMPOSITION_TYPE_CANONICAL");
		add_linef(", .decomposition_mapping_size = 0u");
		add_linef(", .decomposition_mapping = NULL");
	} else if (*field == '<') {
		formatted = get_decomposition_type(field);
		if (!formatted) return 0;
		add_linef(", .decomposition_type = LIBCMAP_DECOMPOSITION_TYPE_%s", formatted);
		field = strchr(field, '>');
		if (!field++) return 0;
		if (*field++ != ' ') return 0;
		goto decomposition_mapping;
	} else {
		add_linef(", .decomposition_type = LIBCMAP_DECOMPOSITION_TYPE_CANONICAL");
	decomposition_mapping:
		n = 0u;
		for (i = 0u;;) {
			if (('0' > field[i] || field[i] > '9') && ('A' > field[i] || field[i] > 'F')) return 0;
			while (('0' <= field[i] && field[i] <= '9') || ('A' <= field[i] && field[i] <= 'F')) i++;
			n += 1u;
			if (!field[i]) break;
			if (field[i] != ' ') return 0;
			field[i++] = '_';
		}
		if (n > LONGEST_DECOMPOSITION) return 0;
		add_linef(", .decomposition_mapping_size = %zuu", n);
		add_linef(", .decomposition_mapping = &sequences[DECOMPOSITION_%s]", field);
		add_sequence(field, n);
	}

	if (!(field = split(&text))) return 0;
	if (*field && !numeric_value) {
		numeric_value = field;
		add_linef(", .numeric_type = LIBCMAP_NUMERIC_TYPE_DECIMAL");
	}

	if (!(field = split(&text))) return 0;
	if (*field && !numeric_value) {
		numeric_value = field;
		add_linef(", .numeric_type = LIBCMAP_NUMERIC_TYPE_DIGIT");
	}

	if (!(field = split(&text))) return 0;
	if (*field && !numeric_value) {
		numeric_value = field;
		add_linef(", .numeric_type = LIBCMAP_NUMERIC_TYPE_NUMERIC");
	}

	if (numeric_value) {
		add_linef(", .numeric_value = \"%s\"", numeric_value);
	} else {
		add_linef(", .numeric_type = LIBCMAP_NUMERIC_TYPE_NONE");
		add_linef(", .numeric_value = NULL");
	}

	if (!(field = split(&text))) return 0;
	add_linef(", .bidirectional_mirrored = BIDIRECTIONAL_MIRRORED_%s", field);

	if (!(field = split(&text))) return 0;
	add_linef(", .unicode_1_name = \"%s\"", field);

	if (!(field = split(&text))) return 0; /* ISO 10646 comment field -- always empty as of Unicode 5.2.0 */

	if (!(field = split(&text))) return 0;
	simple_uppercase_mapping = field;
	if (*field)
		add_linef(", .simple_uppercase_mapping = UINT32_C(0x%s)", field);
	else
		add_linef(", .simple_uppercase_mapping = 0u");

	if (!(field = split(&text))) return 0;
	if (*field)
		add_linef(", .simple_lowercase_mapping = UINT32_C(0x%s)", field);
	else
		add_linef(", .simple_lowercase_mapping = 0u");

	if (!(field = split(&text))) return 0;
	if (*field)
		add_linef(", .simple_titlecase_mapping = UINT32_C(0x%s)", field);
	else if (*simple_uppercase_mapping)
		add_linef(", .simple_titlecase_mapping = UINT32_C(0x%s)", simple_uppercase_mapping);
	else
		add_linef(", .simple_titlecase_mapping = 0u");

	return 1;
}


static void
parse_line(char *text, size_t lineno)
{
	unsigned long int codepoint;
	char *end;

	if (!isxdigit(*text)) {
	malformat:
		fprintf(stderr, "%s: line %zu is malformatted\n", argv0, lineno);
		exit(1);
	}

	errno = 0;

	codepoint = strtoul(text, &end, 16);
	if (errno || codepoint > ULTIMATE_CODEPOINT || *end != ';')
		goto malformat;

	if (!nsegments) {
	new_segment:
		add_linef("static const struct libcmap_codepoint_data data_from_%04lx[] = {\n", codepoint);
		segments = realloc(segments, ++nsegments * sizeof(*segments));
		if (!segments) {
			fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nsegments * sizeof(*segments), strerror(errno));
			exit(1);
		}
		segments[nsegments - 1u].first = (uint32_t)codepoint;
		goto new_codepoint;
	} else if (segments[nsegments - 1u].last + 1u == (uint32_t)codepoint) {
		add_linef(",\n");
	new_codepoint:
		segments[nsegments - 1u].last = (uint32_t)codepoint;
		add_linef("\t{");
		if (!output_character_data(text))
			goto malformat;
		add_linef("}");
	} else {
		if ((uint32_t)codepoint < segments[nsegments - 1u].last) {
			fprintf(stderr, "%s: UnicodeData is not sorted\n", argv0);
			exit(1);
		}
		add_linef("\n};\n");
		goto new_segment;
	}
}


static int
sequence_cmp(const void *a_, const void *b_)
{
	const struct sequence *a = a_, *b = b_;
	return a->len > b->len ? -1 : a->len < b->len; /* longest first */
}


static int
is_same_sequence(const uint32_t *a, const uint32_t *b, size_t n)
{
	while (n--)
		if (a[n] != b[n])
			return 0;
	return 1;
}


static size_t
find_subsequece(const uint32_t *seq, size_t seqlen, struct sequence *subseq, uint32_t decomp[LONGEST_DECOMPOSITION])
{
	uint32_t cp;
	size_t i, n = 0u;
	size_t off = 0u;

	if (subseq->len > LONGEST_DECOMPOSITION) {
		fprintf(stderr, "abort on line %i\n", __LINE__);
		abort();
	}
	if (!subseq->len) {
		fprintf(stderr, "abort on line %i\n", __LINE__);
		abort();
	}

	cp = 0u;
	for (i = 0;; i++) {
		if ('0' <= subseq->string[i] && subseq->string[i] <= '9') {
			cp = cp * 16u + (uint32_t)(subseq->string[i] - '0');
		} else if ('A' <= subseq->string[i] && subseq->string[i] <= 'F') {
			cp = cp * 16u + (uint32_t)(subseq->string[i] - 'A') + 10u;
		} else if (subseq->string[i] == '_' || !subseq->string[i]) {
			if (!cp) {
				fprintf(stderr, "%s: a character U+0000 in its decomposition mapping\n", argv0);
				exit(1);
			}
			decomp[n++] = cp;
			cp = 0u;
			if (!subseq->string[i])
				break;
		} else {
			fprintf(stderr, "abort on line %i\n", __LINE__);
			abort();
		}
	}

	if (n != subseq->len) {
		fprintf(stderr, "abort on line %i\n", __LINE__);
		abort();
	}

	for (; off < seqlen; off++) {
		n = subseq->len < seqlen - off ? subseq->len : seqlen - off;
		if (is_same_sequence(decomp, &seq[off], n))
			break;
	}

	return off;
}


static int
output(void)
{
	uint32_t decomp[LONGEST_DECOMPOSITION];
	uint32_t *seq = NULL;
	size_t i, seqlen = 0u, off, end, ext;

	if (!nsegments) {
		fprintf(stderr, "%s: character list is empty\n", argv0);
		exit(1);
	}
	if (!nsequences) {
		fprintf(stderr, "%s: no character has a decomposition mapping\n", argv0);
		exit(1);
	}

	qsort(sequences, nsequences, sizeof(*sequences), &sequence_cmp);
	for (i = 0u; i < nsequences; i++) {
		off = find_subsequece(seq, seqlen, &sequences[i], decomp);
		ret |= printf("#define DECOMPOSITION_%s %zuu\n", sequences[i].string, off);
		end = off + sequences[i].len;
		ext = end > seqlen ? end - seqlen : 0u;
		off = sequences[i].len - ext;
		if (ext) {
			seq = realloc(seq, (seqlen + ext) * sizeof(*seq));
			if (!seq) {
				fprintf(stderr, "%s: realloc %zu: %s\n", argv0, (seqlen + ext) * sizeof(*seq), strerror(errno));
				exit(1);
			}
			memcpy(&seq[seqlen], &decomp[off], ext * sizeof(*seq));
			seqlen += ext;
		}
		free(sequences[i].string);
	}
	free(sequences);
	ret |= printf("static const uint32_t sequences[] = {\n");
	for (i = 0u; i < seqlen; i++)
		ret |= printf("\tUINT32_C(0x%04lX)%s\n", (unsigned long int)seq[i], i + 1u == seqlen ? "" : ",");
	ret |= printf("};\n");

	for (i = 0u; i < nlines; i++) {
		ret |= printf("%s", lines[i]);
		free(lines[i]);
	}
	free(lines);

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
