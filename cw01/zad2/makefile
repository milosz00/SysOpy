cc = gcc -Wall -std=c11 -g
lib ?= static_lib

all: all_tests


static_lib: 
		$(cc)  -c lib.c -o lib.o
		ar rcs liblib.a lib.o
		$(cc) main.c -llib -L./ -o main.out

shared_lib:
		$(cc) -fPIC -shared lib.c -o liblib.so
		$(cc) main.c -L.  -Wl,-rpath=. -llib -o main.out


clean:
		rm -f *.out *.so *.o *.a raport2.txt


test1:
		./main.out create_table 3 merge_files a.txt:b.txt c.txt:d.txt e.txt:f.txt

test2:
		./main.out create_table 3 merge_files a.txt:f.txt d.txt:e.txt b.txt:c.txt \
		remove_block 0 \
		remove_block 0 \
		remove_block 1

test3:
		./main.out create_table 8 merge_files a.txt:b.txt a.txt:b.txt c.txt:b.txt a.txt:f.txt e.txt:b.txt d.txt:a.txt f.txt:e.txt e.txt:b.txt \
		remove_block 0 \
		remove_row 0 2 \
		remove_row 0 2 \
		remove_row 0 3 \
		remove_row 1 4 \
		remove_block 5 \
		remove_row 2 2

test4:
		./main.out create_table 8 merge_files a.txt:b.txt a.txt:b.txt c.txt:b.txt a.txt:f.txt e.txt:b.txt d.txt:a.txt f.txt:e.txt e.txt:b.txt \
		remove_block 0 \
		remove_row 0 2 \
		remove_row 0 2 \
		remove_row 0 3 \
		remove_row 1 4 \
		remove_block 5 \
		remove_row 2 2 \
		merge_files a.txt:b.txt \
		remove_block 0 \
		merge_files a.txt:b.txt \
		remove_row 0 2 \
		remove_row 0 2 \
		remove_row 0 3 \
		remove_row 0 1 \
		remove_block 0 \
		merge_files e.txt:f.txt 

all_tests:
		make -s $(lib)
		rm -f raport2.txt
		echo "*** test1 ***" >> raport2.txt
		make -s test1 >> raport2.txt
		echo "" >> raport2.txt
		echo "*** test2 ***" >> raport2.txt
		make -s test2 >> raport2.txt
		echo "" >> raport2.txt
		echo "*** test3 ***" >> raport2.txt
		make -s test3 >> raport2.txt
		echo "" >> raport2.txt
		echo "*** test4 ***" >> raport2.txt
		make -s test4 >> raport2.txt
		echo "" >> raport2.txt
