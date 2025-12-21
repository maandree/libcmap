/* See LICENSE file for copyright and license details. */
#include "parse-common.c"


struct range {
	unsigned long int low, high;
};

struct script {
	char *cname;
	char *hname;
	struct range *ranges;
	size_t nranges;
};


static struct script *scripts = NULL;
static size_t nscripts = 0;


static const char *
fixed_script_name(const char *hname)
{
	if (!strcmp(hname, "NKo"))
		return "N'Ko";
	return hname;
}


static struct script *
find_script(char *cname)
{
	size_t i, j;

	for (i = nscripts; i--;)
		if (!strcmp(cname, scripts[i].cname))
			return &scripts[i];

	i = nscripts++;
	scripts = realloc(scripts, nscripts * sizeof(*scripts));
	if (!scripts) {
		fprintf(stderr, "%s: realloc %zu: %s\n", argv0, nscripts * sizeof(*scripts), strerror(errno));
		exit(1);
	}
	scripts[i].cname = strdup(cname);
	if (!scripts[i].cname) {
		fprintf(stderr, "%s: strdup: %s\n", argv0, strerror(errno));
		exit(1);
	}
	scripts[i].ranges = NULL;
	scripts[i].nranges = 0;
	for (j = 0; cname[j]; j++)
		if (cname[j] == '_')
			cname[j] = ' ';
	scripts[i].hname = strdup(fixed_script_name(cname));
	if (!scripts[i].hname) {
		fprintf(stderr, "%s: strdup: %s\n", argv0, strerror(errno));
		exit(1);
	}

	return &scripts[i];
}


static void
parse_line(char *text, size_t lineno)
{
	unsigned long int low, high;
	struct script *script;
	char *name;
	size_t i;

	errno = 0;

	if (!isxdigit(*text)) {
	malformat:
		fprintf(stderr, "%s: line %zu in is malformatted\n", argv0, lineno);
		exit(1);
	}

	high = low = strtoul(text, &text, 16);
	if (errno || low > 0x10FFFFUL)
		goto malformat;
	if (text[0] == '.' && text[1] == '.') {
		if (!isxdigit(text[2]))
			goto malformat;
		high = strtoul(&text[2], &text, 16);
		if (errno || high > 0x10FFFFUL || high < low)
			goto malformat;
	}
	while (isspace(*text))
		text++;

	if (*text++ != ';')
		goto malformat;

	while (isspace(*text))
		text++;
	name = text;
	while (*text && !isspace(*text) && *text != ';')
		text++;
	while (isspace(*text))
		*text++ = '\0';

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

	script = find_script(name);
	i = script->nranges++;
	script->ranges = realloc(script->ranges, script->nranges * sizeof(*script->ranges));
	if (!script->ranges) {
		fprintf(stderr, "%s: realloc %zu: %s\n", argv0, script->nranges * sizeof(*script->ranges), strerror(errno));
		exit(1);
	}
	script->ranges[i].low = low;
	script->ranges[i].high = high;
}


static int
scriptcmp(const void *av, const void *bv)
{
	const struct script *a = av, *b = bv;
	return strcmp(a->hname, b->hname);
}


static int
rangecmp(const void *av, const void *bv)
{
	const struct range *a = av, *b = bv;
	return a->low < b->low ? -1 : a->low > b->low ? +1 : a->high < b->high ? -1 : a->high > b->high;
}


static size_t
join_ranges(struct range *ranges, size_t n)
{
	size_t r, w;

	if (!n)
		abort();

	qsort(ranges, n, sizeof(*ranges), &rangecmp);
	for (r = w = 1U; r < n; r++) {
		if (ranges[r].low == ranges[w - 1U].low)
			ranges[w - 1U].high = ranges[r].high;
		else if (ranges[r].low <= ranges[w - 1U].high + 1U)
			ranges[w - 1U].high = ranges[r].high;
		else
			ranges[w++] = ranges[r];
	}

	return w;
}


static void
range_minus(struct range **rangesp, size_t *np, const struct range *xranges, size_t xn)
{
	struct range *ranges = *rangesp;
	size_t i, j, n = *np;

	for (i = 0, j = 0; i < n && j < xn;) {
		if (xranges[j].high < ranges[i].low) {
			j++;
		} else if (xranges[j].low > ranges[i].high) {
			i++;
		} else if (xranges[j].low <= ranges[i].low && xranges[j].high >= ranges[i].high) {
			memmove(&ranges[i], &ranges[i + 1U], (--n - i) * sizeof(*ranges));
		} else if (xranges[j].low <= ranges[i].low && xranges[j].high < ranges[i].high) {
			ranges[i].low = xranges[j++].high + 1U;
		} else if (xranges[j].high >= ranges[i].high && xranges[j].low > ranges[i].low) {
			ranges[i++].high = xranges[j].low - 1U;
		} else if (xranges[j].low > ranges[i].low && xranges[j].high < ranges[i].high) {
			ranges = realloc(ranges, (n + 1U) * sizeof(*ranges));
			if (!ranges) {
				fprintf(stderr, "%s: realloc %zu: %s\n", argv0, (n + 1U) * sizeof(*ranges), strerror(errno));
				exit(1);
			}
			memmove(&ranges[i + 1U], &ranges[i], (n++ - i) * sizeof(*ranges));
			ranges[i].low = ranges[i + 1U].low;
			ranges[i].high = xranges[j].low - 1U;
			ranges[i + 1U].low = xranges[j].high + 1U;
			i++;
		} else {
			abort();
		}
	}

	*rangesp = ranges;
	*np = n;
}


static int
output(void)
{
	size_t i, j;
	int x = 0;
	const char *prefix;
	struct script *unknown;
	struct range *ranges;
	size_t nranges;

	ranges = malloc(sizeof(*unknown->ranges));
	if (!ranges) {
		fprintf(stderr, "%s: malloc %zu: %s\n", argv0, sizeof(*unknown->ranges), strerror(errno));
		exit(1);
	}
	nranges = 1U;
	ranges[0].low = 0;
	ranges[0].high = 0x10FFFF;

	for (i = 0; i < nscripts; i++) {
		scripts[i].nranges = join_ranges(scripts[i].ranges, scripts[i].nranges);
		range_minus(&ranges, &nranges, scripts[i].ranges, scripts[i].nranges);
	}

	qsort(scripts, nscripts, sizeof(*scripts), &scriptcmp);

	if (nranges) {
		unknown = find_script((char []){"Unknown"});
		unknown->ranges = ranges;
		unknown->nranges = nranges;
	} else {
		free(ranges);
	}

	for (i = 0; i < nscripts; i++) {
		x |= printf("static const struct libcmap_range %s[] = {", scripts[i].cname);
		for (j = 0; j < scripts[i].nranges;) {
			prefix = j % 5U /* no more than 93!! */ == 0U ? "\n\t" : " ";
			x |= printf("%s{0x%04lX, 0x%04lX}", prefix, scripts[i].ranges[j].low, scripts[i].ranges[j].high);
			if (++j < scripts[i].nranges)
				x |= printf(",");
		}
		x |= printf("\n};\n");
	}
	x |= printf("\nstatic const struct libcmap_script list[] = {\n");
	for (i = 0; i < nscripts;) {
		x |= printf("\t{\"%s\", %s, %zu}", scripts[i].hname, scripts[i].cname, scripts[i].nranges);
		free(scripts[i].cname);
		free(scripts[i].hname);
		free(scripts[i].ranges);
		x |= printf("%s\n", ++i < nscripts ? "," : "");
	}
	x |= printf("};\n");
	free(scripts);

	return x;
}
