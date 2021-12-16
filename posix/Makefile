INC=-I..
LIB=-L.. -L${HOME}/local/lib64 -L${HOME}/local/lib
CC=cc
CFLAGS=-O $(INC)

OBJS = posix_file.o posix_proc.o

posixscheme: $(OBJS) main.o
	$(CC) $(CFLAGS) -o posixscheme $(OBJS) main.o $(LIB) -lscheme -lm -lgc

clean:
	rm -f $(OBJS) main.o *~ posixscheme

