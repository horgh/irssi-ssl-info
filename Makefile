IRSSI_DIR=/usr/include/irssi
LIB_DIR=/usr/lib

IRSSI_INCLUDES= \
	-I$(IRSSI_DIR) -I$(IRSSI_DIR)/src -I$(IRSSI_DIR)/src/core \
	-I$(IRSSI_DIR)/src/fe-common/core -I$(IRSSI_DIR)/src/irc/core \

GLIB2_INCLUDES=`pkg-config --cflags glib-2.0`

CC=gcc

# NOTE: -Wpadded generates warnings from includes!
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

INCLUDES=-I. $(IRSSI_INCLUDES) $(GLIB2_INCLUDES)

OBJS=module.o
TARGETS=libsslinfo.so

all: $(TARGETS)

libsslinfo.so: $(OBJS)
	$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) -o $@

module.o: module.c module.h Makefile irssi_includes.h
	$(CC) $(INCLUDES) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -rf $(TARGETS) $(OBJS)
