all : fileSystemOper

fileSystemOper : fileSystemOper.o
	gcc -o fileSystemOper fileSystemOper.o -lm

fileSystemOper.o : fileSystemOper.c
	gcc -c fileSystemOper.c -Wall -Wextra -pedantic

clean:
	rm -f fileSystemOper *.o