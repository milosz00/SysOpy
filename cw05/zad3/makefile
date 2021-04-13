cc = gcc -Wall -Wextra -pedantic -Werror -std=c11 -g


all: run

consumer:
		$(cc) consumer.c -o consumer

producer:
		$(cc) producer.c -o producer

main:
		$(cc) main.c -o main

prepare_inputs:
		echo -n "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH" > first.txt
		echo -n "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" > second.txt
		echo -n "9999999999999999999999999999999999999999999999999999999999999999999999" > third.txt
		echo -n "######################################################################" > fourth.txt
		echo -n "**********************************************************************" > fifth.txt


compile:
		make -s producer
		make -s consumer
		make -s main
		make -s prepare_inputs

clean:
		rm -f consumer producer main *.txt pipe 

run: compile
		./main
