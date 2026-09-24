# Makefile for the NT-Shell native (host) sample.
#
# Build:
#   make
# Run:
#   ./ntshell_native
# Clean:
#   make clean

CC      = gcc
CFLAGS  = -Wall -Wextra -O2
INCLUDES = -I../../../lib/core -I../../../lib/util

TARGET  = ntshell_native

CORE_SRCS = \
	../../../lib/core/ntshell.c \
	../../../lib/core/text_editor.c \
	../../../lib/core/text_history.c \
	../../../lib/core/vtrecv.c \
	../../../lib/core/vtsend.c \
	../../../lib/core/ntlibc.c

UTIL_SRCS = \
	../../../lib/util/ntopt.c

APP_SRCS = \
	main.c \
	usrcmd.c

SRCS = $(APP_SRCS) $(CORE_SRCS) $(UTIL_SRCS)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
