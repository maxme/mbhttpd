VERSION	= 0.1
CC      = gcc
WARN	= -W -Wall -Werror -pedantic
OPTI	= -O3 -fomit-frame-pointer # force omit-frame-pointer makes gdb crazy
#DEBUG	= -ggdb -DDEBUG
#LIB	= -lefence
SHFLAGS = -D_I386

PROJNAME= mbhttpd
EXEC	= mbhttpd
CFLAGS	= $(WARN) $(DEBUG) $(OPTI)

# Warning: not portable but useful
SOURCES = $(shell find src -name "*.c")
HEADER_DEPS = src/config.h src/utils.h
OBJECTS = $(SOURCES:.c=.o)
TARBALL = $(PROJNAME)-$(VERSION).tar.gz


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(EXEC)

clean:
	rm -f src/*.o
	rm -f *~ \#*
	rm -f src/*~ src/\#*
	rm -f tests/*~ tests/\#*
	rm -f bench/*~ bench/\#*

distclean: clean
	rm -f $(EXEC) $(TARBALL)

check: $(EXEC)
	cd tests;  sh full_fs_test.sh

dist: distclean
	mkdir -p /tmp/$(PROJNAME)-$(VERSION);
	cp -R * /tmp/$(PROJNAME)-$(VERSION)/
	tar --exclude ".git" -czf $(TARBALL) -C /tmp $(PROJNAME)-$(VERSION)
	rm -rf /tmp/$(PROJNAME)-$(VERSION)/

.PHONY : bench
bench: $(EXEC)
	cd bench; sh ab-bench.sh

# Warning: not portable (gmake only)
%.o: %.c %.h $(HEADER_DEPS)
	$(CC) $(SHFLAGS) $(CFLAGS) -c -o $@ $<
