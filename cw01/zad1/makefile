cc = gcc -Wall -std=c11 -g

all: static_lib shared_lib

static_lib: 
		$(cc) -c lib.c -o lib.o
		ar rcs lib.a lib.o

shared_lib: lib.c
		$(cc) -c -fPIC lib.c -o lib.o
		$(cc) -shared -fPIC -o libmerge.so lib.o

clean:
		rm -Rf build *.out *.so *.o *.a
