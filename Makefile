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


OBJ =\
	libcmap_block_list.o\
	libcmap_script_list.o\
	libcmap_find_block.o\
	libcmap_find_script.o

HDR =\
	libcmap.h

LOBJ = $(OBJ:.o=.lo)


all: libcmap.a libcmap.$(LIBEXT)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)

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

.SUFFIXES:
.SUFFIXES: .lo .o .c

.PHONY: all install uninstall clean
