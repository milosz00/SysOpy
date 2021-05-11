cc = gcc -Wall -pthread -lrt


main:
	$(cc) main.c -o main -lm

test: main
	./main 16 block b.ascii.pgm result

test_block:
	./main 1 block input.ascii.pgm result
	./main 2 block input.ascii.pgm result
	./main 4 block input.ascii.pgm result
	./main 8 block input.ascii.pgm result
	./main 16 block input.ascii.pgm result

test_numbers:
	./main 1 numbers input.ascii.pgm result
	./main 2 numbers input.ascii.pgm result
	./main 4 numbers input.ascii.pgm result
	./main 8 numbers input.ascii.pgm result
	./main 16 numbers input.ascii.pgm result

clean:
	rm -f main result