opt ?= -O0
lib ?= static_lib_tests
cc = gcc -Wall -std=c11 -g $(opt)


all: run_all_config


static_lib: 
		$(cc)  -c lib.c -o lib.o
		ar rcs liblib.a lib.o
		$(cc) main.c -llib -L./ -o main.out

shared_lib:
		$(cc) -fPIC -shared lib.c -o liblib.so
		$(cc) main.c -L.  -Wl,-rpath=. -llib -o main.out


dynamic_lib:
	$(cc) -fPIC -shared lib.c -o liblib.so
	$(cc) main.c -o main.out -ldl -D dynamic


clean:
		rm -f *.out *.so *.o *.a raport3b.txt


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
		# rm -f raport3b.txt
		echo "*** test1 ***" >> raport3b.txt
		make -s test1 >> raport3b.txt
		echo "" >> raport3b.txt
		echo "*** test2 ***" >> raport3b.txt
		make -s test2 >> raport3b.txt
		echo "" >> raport3b.txt
		echo "*** test3 ***" >> raport3b.txt
		make -s test3 >> raport3b.txt
		echo "" >> raport3b.txt
		echo "*** test4 ***" >> raport3b.txt
		make -s test4 >> raport3b.txt
		echo "" >> raport3b.txt



static_lib_tests:
	make static_lib
	echo "    STATIC  LIBRARY TEST " >> raport3b.txt
	echo "----------------" >> raport3b.txt

	make -s all_tests


shared_lib_tests: 
	make clean
	make shared_lib
	echo "    SHARED  LIBRARY TEST " >> raport3b.txt
	echo "----------------" >> raport3b.txt

	make -s all_tests

dynamic_lib_tests:
	make clean
	make dynamic_lib
	echo "    DYNAMIC  LIBRARY TEST " >> raport3b.txt
	echo "----------------" >> raport3a.txt

	make -s all_tests


run_opt_O0:
	echo "--------- opt = O0 ---------" >> raport3b.txt
	echo "" >> raport3b.txt
	make -s $(lib) opt=-O0

run_opt_O2:
	echo "--------- opt = O2 ---------" >> raport3b.txt
	echo "" >> raport3b.txt
	make -s $(lib) opt=-O2

run_opt_Os:
	echo "--------- opt = Os ---------" >> raport3b.txt
	echo "" >> raport3b.txt
	make -s $(lib) opt=-Os

run_all_config:
		rm -f raport3b.txt
		make -s run_opt_O0

		echo "------------------------------------------------" >> raport3b.txt
		echo "" >> raport3b.txt

		make -s run_opt_O2

		echo "------------------------------------------------" >> raport3b.txt
		echo "" >> raport3b.txt

		make -s run_opt_Os

