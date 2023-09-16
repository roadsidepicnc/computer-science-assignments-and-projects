PART 1:

For testing my homework you need to type:

	lex lexer.l
	yacc -d gpp_interpreter.y
	gcc lex.yy.c y.tab.c -w
	./a.out

	or:

	./a.out		


You can exit program by typing "(exit)"

PART 2:

I used lexer from the previous homework. Lexer returns the tokens. These tokens are used by the parser later.
You can start looking from line 485 where parser part starts. Before line 485 is lexer code from previous homework.

For testing my homework you need to type "clisp gpp_interpreter.lisp" .

First part takes input from user, second part takes input from a text file.
You need to type "quit" in order to exit the first part.
I created a "test.txt" file for testing my homework. You can also type the name of the file you created and test my homework in the second part. 
