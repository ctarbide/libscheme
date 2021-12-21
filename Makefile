#
# Makefile for libscheme
#

#
# This must be an ANSI C compiler.
#
CC = gcc
CPROTO = cproto

#
# Optimization and debugging flags go here.
#
#CFLAGS = -O
#CFLAGS = -ggdb3 -O0 -I${HOME}/local/include
#CFLAGS = -ggdb3 -O0 -DNO_GC
#CFLAGS = -ggdb3 -O0 -DNO_GC -m32

# -fmax-errors=N: GCC 4.6 and later

CFLAGS = -ggdb3 -O0 -DNO_GC \
    -std=c99 -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow \
    -Wconversion -Wdeclaration-after-statement \
    -Werror -fmax-errors=5

#
# The math library is needed for the numeric functions
# in scheme_number.c.
#LIBS = -L.. -L${HOME}/local/lib64 -L${HOME}/local/lib -lm -lgc
LIBS = -lm

CPROTOFLAGS = -I${HOME}/local/include

#
# If your system needs ranlib, put it here.  Otherwise,
# use a colon.
#
RANLIB=:

OBJS =  scheme_alloc.o \
	scheme_bool.o \
	scheme_char.o \
	scheme_env.o \
	scheme_error.o \
	scheme_eval.o \
	scheme_fun.o \
	scheme_hash.o \
	scheme_list.o \
	scheme_number.o \
	scheme_port.o \
	scheme_print.o \
	scheme_promise.o \
	scheme_read.o \
	scheme_string.o \
	scheme_struct.o \
	scheme_symbol.o \
	scheme_syntax.o \
	scheme_type.o \
	scheme_vector.o

SRCS =  scheme_alloc.c \
	scheme_bool.c \
	scheme_char.c \
	scheme_env.c \
	scheme_error.c \
	scheme_eval.c \
	scheme_fun.c \
	scheme_hash.c \
	scheme_list.c \
	scheme_number.c \
	scheme_port.c \
	scheme_print.c \
	scheme_promise.c \
	scheme_read.c \
	scheme_string.c \
	scheme_struct.c \
	scheme_symbol.c \
	scheme_syntax.c \
	scheme_type.c \
	scheme_vector.c

all: test

scheme_prototypes.h: $(SRCS)
	$(CPROTO) $(CPROTOFLAGS) $(SRCS) > .tmp.scheme_prototypes.h
	mv -f .tmp.scheme_prototypes.h scheme_prototypes.h
	chmod a-w scheme_prototypes.h

libscheme.a: $(OBJS)
	$(AR) rv libscheme.a $(OBJS)
	$(RANLIB) libscheme.a

test: libscheme.a main.o 
	$(CC) $(CFLAGS) -o test main.o libscheme.a $(LIBS)

clean:
	/bin/rm -f $(OBJS) main.o libscheme.a test *~ \
	libscheme.aux libscheme.dvi libscheme.log tmp1 tmp2 tmp3
