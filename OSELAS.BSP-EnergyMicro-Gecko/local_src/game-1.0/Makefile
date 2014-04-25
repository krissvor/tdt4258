#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -g -Wall #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=

# The game must be compiled with -lrt as the last option. This can not be done here, but must be done manually

all: game

game: painter.o sprites.o

clean:
	-rm -f game *.o

install:


.PHONY: all install clean

# end of development makefile
