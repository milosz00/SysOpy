cc = gcc -Wall -std=c11 -g


all: static_lib

static_lib: 
		$(cc)  -c lib.c -o lib.o
		ar rcs liblib.a lib.o
		$(cc) main.c -llib -L./ -o main.out

run:
		./main.out create_table 3 a.txt:b.txt d.txt:a.txt c.txt:b.txt

big_files:
		./main.out create_table 67 merge_files e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt \
		e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt e.txt:d.txt 

small_files:
		./main.out create_table 50 merge_files \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt \
		a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt a.txt:b.txt 

diffrent:
		./main.out create_table 100 merge_files \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt 

few_files:
		./main.out create_table 4 merge_files a.txt:b.txt c.txt:e.txt f.txt:e.txt e.txt:f.txt

lot_files:
		./main.out create_table 300 merge_files \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt \
		a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt a.txt:e.txt b.txt:a.txt c.txt:f.txt f.txt:e.txt d.txt:f.txt 

clean:
		rm -f *.out *.so *.o *.a result.txt