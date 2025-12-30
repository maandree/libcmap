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

UNICODE_VERSION = 17.0.0
# Before updating, remember to check downloaded files
# changes in how unlisted codepoints should be treated


OBJ =\
	libcmap_block_list.o\
	libcmap_script_list.o\
	libcmap_find_in_no_block.o\
	libcmap_find_block.o\
	libcmap_find_script.o\
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
	Blocks.parse


all: libcmap.a libcmap.$(LIBEXT)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)
$(PARSERS:=.o): parse-common.c

libcmap_script_list.o: Scripts.txt.c
libcmap_block_list.o: Blocks.txt.c

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

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

Scripts.txt.c: Scripts.$(UNICODE_VERSION).txt Scripts.parse
	./Scripts.parse < $(@:.txt.c=).$(UNICODE_VERSION).txt > $@

Blocks.txt.c: Blocks.$(UNICODE_VERSION).txt Blocks.parse
	./Blocks.parse < $(@:.txt.c=).$(UNICODE_VERSION).txt > $@

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

.SUFFIXES:
.SUFFIXES: .lo .o .c

.PHONY: all install uninstall clean clean-downloads clean-all
