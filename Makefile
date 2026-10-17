.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

OS = linux
# Linux:   linux
# Mac OS:  macos
# Windows: windows
include mk/$(OS).mk


LIB_MAJOR = 1
LIB_MINOR = 0
LIB_VERSION = $(LIB_MAJOR).$(LIB_MINOR)
LIB_NAME = cmap

UNICODE_VERSION = 18.0.0
# Before updating, remember to check downloaded files
# changes in how unlisted codepoints should be treated


OBJ =\
	libcmap_unicode_version.o\
	libcmap_block_list.o\
	libcmap_script_list.o\
	libcmap_codepoint_list.o\
	libcmap_find_in_no_block.o\
	libcmap_find_block.o\
	libcmap_find_script.o\
	libcmap_find_codepoint_data.o\
	libcmap_sprint_range.o\
	libcmap_snprint_range.o\
	libcmap_fprint_range.o\
	libcmap_print_range.o\
	libcmap_dprint_range.o

HDR =\
	libcmap.h\
	common.h

LOBJ = $(OBJ:.o=.lo)

PARSERS =\
	Scripts.parse\
	Blocks.parse\
	UnicodeData.parse


all: libcmap.a libcmap.$(LIBEXT)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)
$(PARSERS:=.o): parse-common.c
$(PARSERS):

libcmap_script_list.o: Scripts.txt.c
libcmap_block_list.o: Blocks.txt.c
libcmap_codepoint_list.o: UnicodeData.txt.c

libcmap_unicode_version.o: Makefile

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) -D'UNICODE_VERSION="$(UNICODE_VERSION)"'

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS) -D'UNICODE_VERSION="$(UNICODE_VERSION)"'

.o:
	$(CC) -o $@ $<

libcmap.a: $(OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(OBJ)
	$(AR) ts $@ > /dev/null

libcmap.$(LIBEXT): $(LOBJ)
	$(CC) $(LIBFLAGS) -o $@ $(LOBJ) $(LDFLAGS)

Scripts.$(UNICODE_VERSION).txt:
	$(DOWNLOAD) 'https://www.unicode.org/Public/$(UNICODE_VERSION)/ucd/Scripts.txt' > $@

Blocks.$(UNICODE_VERSION).txt:
	$(DOWNLOAD) 'https://www.unicode.org/Public/$(UNICODE_VERSION)/ucd/Blocks.txt' > $@

UnicodeData.$(UNICODE_VERSION).txt:
	$(DOWNLOAD) 'https://www.unicode.org/Public/$(UNICODE_VERSION)/ucd/UnicodeData.txt' > $@

Scripts.txt.c: Scripts.$(UNICODE_VERSION).txt Scripts.parse
	./Scripts.parse < $(@:.txt.c=).$(UNICODE_VERSION).txt > $@ || (rm -f -- "$@"; ! :)

Blocks.txt.c: Blocks.$(UNICODE_VERSION).txt Blocks.parse
	./Blocks.parse < $(@:.txt.c=).$(UNICODE_VERSION).txt > $@ || (rm -f -- "$@"; ! :)

UnicodeData.txt.c: UnicodeData.$(UNICODE_VERSION).txt UnicodeData.parse
	./UnicodeData.parse < $(@:.txt.c=).$(UNICODE_VERSION).txt > $@ || (rm -f -- "$@"; ! :)

install: libcmap.a libcmap.$(LIBEXT)
	mkdir -p -- "$(DESTDIR)$(PREFIX)/lib"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/include"
	cp -- libcmap.a "$(DESTDIR)$(PREFIX)/lib/"
	cp -- libcmap.$(LIBEXT) "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBMINOREXT)"
	$(FIX_INSTALL_NAME) "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBMINOREXT)"
	ln -sf -- libcmap.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBMAJOREXT)"
	ln -sf -- libcmap.$(LIBMAJOREXT) "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBEXT)"
	cp -- libcmap.h "$(DESTDIR)$(PREFIX)/include/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libcmap.a"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBMAJOREXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBMINOREXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libcmap.$(LIBEXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/include/libcmap.h"

clean:
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.dll *.dylib
	-rm -f -- *.gch *.gcov *.gcno *.gcda *.$(LIBEXT)
	-rm -f -- *.txt.c *.parse

clean-downloads:
	-rm -f -- *.txt

clean-all: clean clean-downloads

download-license:
	$(DOWNLOAD) 'https://www.unicode.org/license.txt' > LICENSE.UNICODE

.SUFFIXES:
.SUFFIXES: .lo .o .c

.PHONY: all install uninstall clean clean-downloads clean-all download-license
