/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define ULTIMATE_CODEPOINT 0x10FFFFUL


static const char *argv0;


static void parse_line(char *text, size_t lineno);
static int output(void);


static size_t
parse(char *buf, size_t len)
{
	static int prev_was_cr = 0;
	static size_t lineno = 1U;
	size_t ret = 0;
	size_t off = 0;
	size_t she;

beginning:
	if (prev_was_cr && off < len && buf[off] == '\n')
		off++;
	while (off < len && buf[off] != '\n' && buf[off] != '\r' && isspace(buf[off]))
		off++;
	ret = off;
	if (off == len)
		return ret;
	if (buf[off] == '#') {
		while (off < len && buf[off] != '\n' && buf[off] != '\r')
			off++;
		if (off == len)
			return ret;
		goto newline;
	} else if (buf[off] == '\n' || buf[off] == '\r') {
	newline:
		prev_was_cr = buf[off] == '\r';
		off++;
		lineno++;
		goto beginning;
	}

	for (; off < len; off++) {
		if (buf[off] == '\0') {
			fprintf(stderr, "%s: NUL byte found in <stdin> on line %zu\n", argv0, lineno);
			exit(1);
		} else if (buf[off] == '\n' || buf[off] == '\r' || buf[off] == '#') {
			break;
		}
	}
	if (off == len)
		return ret;

	if (buf[off] == '\n' || buf[off] == '\r') {
		prev_was_cr = buf[off] == '\r';
		buf[off++] = '\0';
	} else if (buf[off] == '#') {
		she = off++;
		while (off < len && buf[off] != '\n' && buf[off] != '\r')
			off++;
		if (off == len)
			return ret;
		buf[she] = '\0';
		prev_was_cr = buf[off++] == '\r';
	} else {
		abort();
	}

	parse_line(&buf[ret], lineno);
	lineno++;
	goto beginning;
}


int
main(int argc, char *argv[])
{
	char *buf = NULL;
	size_t bufsize = 0;
	size_t len = 0;
	size_t parsed;
	ssize_t r;

	argv0 = argv[0];
	(void) argc;

	for (;;) {
		if (len == bufsize) {
			buf = realloc(buf, bufsize += 8192U);
			if (!buf) {
				fprintf(stderr, "%s: realloc %zu: %s\n", argv0, bufsize, strerror(errno));
				exit(1);
			}
		}
		r = read(STDIN_FILENO, &buf[len], bufsize - len);
		if (r <= 0) {
			if (!r)
				break;
			if (errno == EINTR)
				continue;
			fprintf(stderr, "%s: read <stdin>: %s\n", argv0, strerror(errno));
			exit(1);
		}
		len += (size_t)r;
		parsed = parse(buf, len);
		memmove(&buf[0], &buf[parsed], len -= parsed);
	}
	buf[len++] = '\n';
	parse(buf, len);
	free(buf);

	if (output() < 0 || fflush(stdout)) {
		fprintf(stderr, "%s: failed to write to <stdout>\n", argv0);
		exit(1);
	}

	return 0;
}
