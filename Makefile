IRSSI_DIR=/usr/include/irssi
GLIB2_DIR=/usr/include/glib-2.0
GLIB2_DIR_2=/usr/lib/i386-linux-gnu/glib-2.0
LIB_DIR=/usr/lib

IRSSI_INCLUDES= \
	-I$(IRSSI_DIR) -I$(IRSSI_DIR)/src -I$(IRSSI_DIR)/src/core \
	-I$(IRSSI_DIR)/src/fe-common/core -I$(IRSSI_DIR)/src/irc/core \

GLIB2_INCLUDES= \
	-I$(GLIB2_DIR) -I$(GLIB2_DIR)/glib \
	-I$(GLIB2_DIR_2) -I$(GLIB2_DIR_2)/include \

CC=gcc

# -Wpadded generates from includes!
CFLAGS = \
	-shared \
	-std=c11 -O1 -g -ggdb -pedantic -pedantic-errors \
	-Werror -Wall -Wextra \
	-Wformat-y2k -Wformat-nonliteral -Wformat-security \
	-Wformat=2 \
	-Winit-self \
	-Wmissing-include-dirs -Wswitch-default \
	-Wunused-local-typedefs \
	-Wunused-parameter \
	-Wunused-result \
	-Wunused \
	-Wuninitialized \
	-Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
	-Wbad-function-cast \
	-Wempty-body \
	-Wsign-conversion -Wlogical-op -Waggregate-return \
	-Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
	-Wold-style-definition -Wpacked \
	-Wredundant-decls \
	-Wnested-externs -Winline -Winvalid-pch \
	-Wwrite-strings -Waggregate-return \
	-Wswitch-enum -Wconversion -Wunreachable-code

LINKS=-L$(LIB_DIR)
INCLUDES=-I. $(IRSSI_INCLUDES) $(GLIB2_INCLUDES)

OBJS=module.o
TARGETS=libsslinfo.so

all: $(TARGETS)

libsslinfo.so: $(OBJS)
	$(CC) $(LINKS) $(INCLUDES) $(CFLAGS) $(OBJS) -o $@

module.o: module.c module.h Makefile irssi_includes.h
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGETS) $(OBJS)
