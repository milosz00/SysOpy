cc = gcc -Wall -ggdb

all: sender catcher

sender:
		$(cc) sender.c -o sender

catcher:
		$(cc) catcher.c -o catcher

run_kill:
		@./catcher kill&
		@./sender `pidof catcher` 1000 kill

run_queue:
		@./catcher SIGQUEUE&
		@./sender `pidof catcher` 1000 SIGQUEUE

run_sigrt:
		@./catcher SIGRT&
		@./sender `pidof catcher` 1000 SIGRT

clean:
		rm -f sender catcher