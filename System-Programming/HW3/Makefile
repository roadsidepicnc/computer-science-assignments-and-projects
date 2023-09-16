all: hw3_unnamed hw3_named

hw3_unnamed : hw3_unnamed.c
	gcc -pthread -o hw3_unnamed hw3_unnamed.c -lm -lrt -Wall

hw3_named : hw3_named.c
	gcc -pthread -o hw3_named hw3_named.c -lm -lrt -Wall

clean:
	$(RM) hw3_unnamed
	$(RM) hw3_named