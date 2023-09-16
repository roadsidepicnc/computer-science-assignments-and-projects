#include <stdio.h>


extern  int yylex();
extern  int yylineno;
extern  char* yytext;

int main(void)
{
	int ntoken;
	
	while(ntoken != 32) 
	{
		ntoken = yylex();
		if(ntoken == 0) printf("KW_AND\n");
		else if(ntoken == 1) printf("KW_OR\n");
		else if(ntoken == 2) printf("KW_NOT\n");
		else if(ntoken == 3) printf("KW_EQUAL\n");
		else if(ntoken == 4) printf("KW_LESS\n");
		else if(ntoken == 5) printf("KW_NIL\n");
		else if(ntoken == 6) printf("KW_LIST\n");
		else if(ntoken == 7) printf("KW_APPEND\n");
		else if(ntoken == 8) printf("KW_CONCAT\n");
		else if(ntoken == 9) printf("KW_SET\n");
		else if(ntoken == 10) printf("KW_DEFFUN\n");
		else if(ntoken == 11) printf("KW_FOR\n");
		else if(ntoken == 12) printf("KW_IF\n");
		else if(ntoken == 13) printf("KW_EXIT\n");
		else if(ntoken == 14) printf("KW_LOAD\n");
		else if(ntoken == 15) printf("KW_DISP\n");
		else if(ntoken == 16) printf("KW_TRUE\n");
		else if(ntoken == 17) printf("KW_FALSE\n");
		else if(ntoken == 18) printf("OP_PLUS\n");
		else if(ntoken == 19) printf("OP_MINUS\n");
		else if(ntoken == 20) printf("OP_DIV\n");
		else if(ntoken == 21) printf("OP_DIV2\n");
		else if(ntoken == 22) printf("OP_MULT\n");
		else if(ntoken == 23) printf("OP_OP\n");
		else if(ntoken == 24) printf("OP_CP\n");
		else if(ntoken == 25) printf("OP_DBLMULT\n");
		else if(ntoken == 26) printf("OP_OC\n");
		else if(ntoken == 27) printf("OP_CC\n");
		else if(ntoken == 28) printf("OP_COMMA\n");
		else if(ntoken == 29) printf("COMMENT\n");
		else if(ntoken == 30) printf("VALUE\n");
		else if(ntoken == 31) printf("IDENTIFIER\n");
		else if(ntoken == 32) printf("SYNTAX ERROR\n");

	}
	return 0;
}