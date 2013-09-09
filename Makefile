# makedumpfile

VERSION=1.5.4
DATE=3 Jul 2013

# Honour the environment variable CC
ifeq ($(strip $CC),)
CC	= gcc
endif

CFLAGS = -g -O2 -Wall

ALL_CFLAGS = $(CFLAGS) -D_FILE_OFFSET_BITS=64 \
	  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE \
	  -DVERSION='"$(VERSION)"' -DRELEASE_DATE='"$(DATE)"'
# LDFLAGS = -L/usr/local/lib -I/usr/local/include

# Use TARGET as the target architecture if specified.
# Defaults to uname -m
ifeq ($(strip($TARGET)),)
TARGET := $(shell uname -m)
endif

ARCH := $(shell echo ${TARGET}  | sed -e s/i.86/x86/ -e s/sun4u/sparc64/ \
			       -e s/arm.*/arm/ -e s/sa110/arm/ \
			       -e s/s390x/s390/ -e s/parisc64/parisc/ \
			       -e s/ppc64/powerpc64/ -e s/ppc/powerpc32/)

ALL_CFLAGS += -D__$(ARCH)__

ifeq ($(ARCH), powerpc64)
ALL_CFLAGS += -m64
endif

ifeq ($(ARCH), powerpc32)
ALL_CFLAGS += -m32
endif

HEADERS = makedumpfile.h diskdump_mod.h sadump_mod.h sadump_info.h
SRC =	makedumpfile.c \
	print_info.c dwarf_info.c elf_info.c erase_info.c sadump_info.c cache.c \
	arch/arm.c arch/x86.c arch/x86_64.c arch/ia64.c arch/ppc64.c arch/s390x.c arch/ppc.c
OBJ =	$(SRC:.c=.o)

LIBS = -ldw -lbz2 -lebl -ldl -lelf -lz
ifneq ($(LINKTYPE), dynamic)
LIBS := -static $(LIBS)
endif

ifeq ($(USELZO), on)
LIBS := -llzo2 $(LIBS)
ALL_CFLAGS += -DUSELZO
endif

ifeq ($(USESNAPPY), on)
LIBS := -lsnappy $(LIBS)
ALL_CFLAGS += -DUSESNAPPY
endif

all: makedumpfile

%.o: %.c $(HEADERS)
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

makedumpfile: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -rdynamic -o $@ $^ $(LIBS)
	echo .TH MAKEDUMPFILE 8 \"$(DATE)\" \"makedumpfile v$(VERSION)\" \"Linux System Administrator\'s Manual\" > temp.8
	grep -v "^.TH MAKEDUMPFILE 8" makedumpfile.8 >> temp.8
	mv temp.8 makedumpfile.8
	gzip -c ./makedumpfile.8 > ./makedumpfile.8.gz
	echo .TH MAKEDUMPFILE.CONF 5 \"$(DATE)\" \"makedumpfile v$(VERSION)\" \"Linux System Administrator\'s Manual\" > temp.5
	grep -v "^.TH MAKEDUMPFILE.CONF 5" makedumpfile.conf.5 >> temp.5
	mv temp.5 makedumpfile.conf.5
	gzip -c ./makedumpfile.conf.5 > ./makedumpfile.conf.5.gz

eppic_makedumpfile.so: extension_eppic.c
	$(CC) $(CFLAGS) -shared -rdynamic -o $@ extension_eppic.c -fPIC -leppic -ltinfo

clean:
	rm -f $(OBJ) $(OBJ_PART) $(OBJ_ARCH) makedumpfile makedumpfile.8.gz makedumpfile.conf.5.gz

install:
	cp makedumpfile ${DESTDIR}/bin
	cp makedumpfile-R.pl ${DESTDIR}/bin
	cp makedumpfile.8.gz ${DESTDIR}/usr/share/man/man8
	cp makedumpfile.conf.5.gz ${DESTDIR}/usr/share/man/man5
	cp makedumpfile.conf ${DESTDIR}/etc/makedumpfile.conf.sample
