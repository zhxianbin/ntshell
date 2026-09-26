# Makefile for the NT-Shell native-ntcli (host) sample.
#
# This sample uses the high level ntcli API, which wraps the ntcmd
# command framework and the ntshell line editor.
#
# Build:
#   make
# Run:
#   ./ntshell_native_ntcli
# Clean:
#   make clean

CC      = gcc
CFLAGS  = -Wall -Wextra -O2
INCLUDES = -I../../../lib/core -I../../../lib/util

TARGET  = ntshell_native_ntcli

CORE_SRCS = \
	../../../lib/core/ntshell.c \
	../../../lib/core/ntcmd.c \
	../../../lib/core/ntcli.c \
	../../../lib/core/text_editor.c \
	../../../lib/core/text_history.c \
	../../../lib/core/vtrecv.c \
	../../../lib/core/vtsend.c \
	../../../lib/core/ntlibc.c

APP_SRCS = \
	main.c \
	usrcmd.c \
	cmd_help.c \
	cmd_info.c \
	cmd_echo.c \
	cmd_exit.c

SRCS = $(APP_SRCS) $(CORE_SRCS)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
