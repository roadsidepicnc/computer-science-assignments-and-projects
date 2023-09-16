all: R_i processP

R_i: R_i.c
	gcc -o R_i R_i.c -lm

processP: processP.c
	gcc -o processP processP.c -lm

clean:
	$(RM) processP
