CC = gcc
CFLAGS = -g -Wall -lm

build: task1 task2 task3 task4

task1: task1.c
	$(CC) task1.c header.c -o task1 $(CFLAGS)
	
task2: task2.c
	$(CC) task2.c header.c -o task2 $(CFLAGS)
	
task3: task3.c
	$(CC) task3.c header.c -o task3 $(CFLAGS)
	
task4: task4.c
	$(CC) task4.c header.c -o task4 $(CFLAGS)
	
browser: browser
	$(CC) browser.c header.c -o browser -lncurses $(CFLAGS)
	
clean:
	rm -f task1 task2 task3 task4 browser
	rm -f *.out
	rm my_diff
