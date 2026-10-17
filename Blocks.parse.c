/* See LICENSE file for copyright and license details. */
#include "parse-common.c"

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wunsafe-buffer-usage" /* broken */
# pragma clang diagnostic ignored "-Wimplicit-void-ptr-cast" /* C++ */
# pragma clang diagnostic ignored "-Wpre-c11-compat" /* C++ */
# pragma clang diagnostic ignored "-Wdisabled-macro-expansion" /* libc */
#endif


struct block {
	char *name;
	unsigned long int low, high;
};


static struct block *blocks = NULL;
static size_t nblocks = 0;


static void
parse_line(char *text, size_t lineno)
{
	unsigned long int low, high;
	char *name;
	size_t i;

	errno = 0;

	if (!isxdigit(*text)) {
	malformat:
		fprintf(stderr, "%s: line %zu is malformatted\n", argv0, lineno);
		exit(1);
	}

	high = low = strtoul(text, &text, 16);
	if (errno || low > ULTIMATE_CODEPOINT)
		goto malformat;
	if (text[0] == '.' && text[1] == '.') {
		if (!isxdigit(text[2]))
			goto malformat;
		high = strtoul(&text[2], &text, 16);
		if (errno || high > ULTIMATE_CODEPOINT || high < low)
			goto malformat;
	}
	while (isspace(*text))
		text++;

	if (*text++ != ';')
		goto malformat;

	while (isspace(*text))
		text++;
	name = text;
	while (*text && *text != ';')
		text++;
	for (i = 1u; isspace(*(text - i)); i++)
		*(text - i) = '\0';

	if (*text == ';') {
		static int warned = 0;
		if (!warned) {
			warned = 1;
			fprintf(stderr, "%s: unrecognised column detected in <stdin>\n", argv0);
		}
		*text++ = '\0';
	} else if (*text) {
		goto malformat;
	}

	i = nblocks++;
	blocks = realloc(blocks, nblocks * sizeof(*blocks));
	if (!blocks) {
		fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nblocks * sizeof(*blocks), strerror(errno));
		exit(1);
	}
	blocks[i].name = strdup(name);
	if (!blocks[i].name) {
		fprintf(stderr, "%s: strdup: %s\n", argv0, strerror(errno));
		exit(1);
	}
	blocks[i].low = low;
	blocks[i].high = high;
}


static int
blockcmp_name(const void *av, const void *bv)
{
	const struct block *a = av, *b = bv;
	return strcmp(a->name, b->name);
}


static int
blockcmp_range(const void *av, const void *bv)
{
	const struct block *a = av, *b = bv;
	return a->low < b->low ? -1 : +1;
}


static int
output(void)
{
	size_t i;
	int x = 0;

	qsort(blocks, nblocks, sizeof(*blocks), &blockcmp_name);

	x |= printf("static const struct libcmap_block list[] = {\n");
	for (i = 0; i < nblocks;) {
		x |= printf("\t{\"%s\", {0x%04lX, 0x%04lX}}", blocks[i].name, blocks[i].low, blocks[i].high);
		free(blocks[i].name);
		x |= printf("%s\n", ++i < nblocks ? "," : "");
	}
	x |= printf("};\n\n");

	qsort(blocks, nblocks, sizeof(*blocks), &blockcmp_range);
	if (!nblocks || blocks[0].low)
		abort();
	for (i = 1u; i < nblocks; i++)
		if (blocks[i].low > blocks[i - 1u].high + 1u)
			break;
	if (i == nblocks && blocks[i - 1u].high == ULTIMATE_CODEPOINT) {
		x |= printf("const struct libcmap_script libcmap_no_block = {\"No Block\", NULL, 0};\n");
	} else {
		x |= printf("static const struct libcmap_range No_Block[] = {\n");
		x |= printf("\t{0x%04lX, 0x%04lX}", blocks[i - 1u].high + 1u, blocks[i].low - 1u);
		for (i++; i < nblocks; i++)
			if (blocks[i].low > blocks[i - 1u].high + 1u)
				x |= printf(",\n\t{0x%04lX, 0x%04lX}", blocks[i - 1u].high + 1u, blocks[i].low - 1u);
		if (blocks[i - 1u].high < ULTIMATE_CODEPOINT)
			x |= printf(",\n\t{0x%04lX, LIBCMAP_ULTIMATE_CODEPOINT}", blocks[i - 1u].high + 1u);
		x |= printf("\n};\n");
		x |= printf("const struct libcmap_script libcmap_no_block = ");
		x |= printf("{\"No Block\", No_Block, sizeof(No_Block) / sizeof(*No_Block)};\n");
	}

	free(blocks);
	return x;
}
