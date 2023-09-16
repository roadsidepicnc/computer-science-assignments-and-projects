all : makeFileSystem

makeFileSystem : makeFileSystem.o
	gcc -o makeFileSystem makeFileSystem.o -lm

makeFileSystem.o : makeFileSystem.c
	gcc -c makeFileSystem.c -Wall -Wextra -pedantic

clean:
	rm -f makeFileSystem *.o