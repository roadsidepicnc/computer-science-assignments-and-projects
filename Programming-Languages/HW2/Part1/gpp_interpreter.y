%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#include <string.h>
	extern int yylex();
	extern  int yylineno;
	extern  char* yytext;

	struct variable
	{ 
		int type;
		char name[100];
		int val;
		int arr[1000];
		int arrSize;
	};

	struct function
	{ 
		int type;
		int argNum;
		char name[100];
	};

	struct function functions[1000];
	struct variable variables[1000];
	int variablesSize = 0;

	int funcCheck = 0;

	int tempArr[1000];
	int tempArrSize = 0;

	int counter = 0;
	int idCounter;
	int funcSize = 0;

	int temp[2] = {-1,-1};


	struct variable getFromVariables(char* str)
	{
		for(int i = 0; i < variablesSize; i++)
		{
			if(strcmp(variables[i].name, str) == 0)
			{
				
				return variables[i];
			}
		}
		return;
	}

	void addFunc(char* id, int num)
	{
		for(int i = 0; i < strlen(id); i++)
		{
			functions[funcSize].name[i] = id[i];
		}

		functions[funcSize].type = 1;
		functions[funcSize].argNum = num;
		funcSize++;
	}

	struct function getFromFunctions(char* str)
	{
		for(int i = 0; i < funcSize; i++)
		{
			if(strcmp(functions[i].name, str) == 0)
			{
				return functions[i];
			}
		}
		return;
	}

	void addVarI(char* id , int v)
	{
		for(int i = 0; i < variablesSize; i++)
		{
			if(strcmp(variables[i].name, id) == 0)
			{	
				variables[i].val = v;
				variables[i].type = 1;
				return;
			}
		}

		for(int i = 0; i < strlen(id); i++)
		{
			variables[variablesSize].name[i] = id[i];
		}
		
		variables[variablesSize].val = v;
		variables[variablesSize].type = 1;
		variables[variablesSize].type = 1;variablesSize++;
	}
	
	void addVarB(char id [], int v)
	{
		for(int i = 0; i < variablesSize; i++)
		{
			if(strcmp(variables[i].name, id) == 0)
			{	
				variables[i].val = v;
				variables[i].type = 2;
				return;
			}
		}

		for(int i = 0; i < strlen(id); i++)
		{
			variables[variablesSize].name[i] = id[i];
		}
		variables[variablesSize].type = 2;
		variables[variablesSize].val = v;
		variablesSize++;
	}	

	void copyArr(int arr1[], int arr2[], int size)
	{
		for(int i = 0; i < size; i++) arr1[i] = arr2[i];		
	}

%}

%union {
	struct{
	int number;
	char* id;
	int valArr [1000];
	int output;
	};
}



%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT KW_SET KW_DEFFUN KW_DEFVAR KW_FOR KW_WHILE KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE 
%token OP_PLUS OP_MINUS OP_DIV OP_DBLMULT OP_MULT OP_OP OP_CP OP_AP OP_OC OP_CC OP_COMMA
%start START
%token <number>VALUE
%token COMMENT
%token <id>IDENTIFIER



%%
	START : | START INPUT	{printf("Input: ");}
	INPUT : EXPI 	
	{
		printf("SYNTAX: OK\n");
		
		if($<output>1 == 2)
		{
			if($<number>$ == 0) printf("RESULT: FALSE\n\n");
			else if($<number>$ == 1) printf("RESULT: TRUE\n\n");
		}
		else if($<output>1 == 8)	{printf("RESULT: "); for(int i = 0; i < temp[0]; i++)	printf("%d ",  $<valArr>$[i]); printf("\n"); temp[0] = -1;temp[1] = -1;}
		else if($<output>1 != 3)printf("RESULT: %d\n\n",  $<number>$);
	}
		  | EXPLISTI {printf("RESULT: "); for(int i = 0; i < temp[0]; i++)	printf("%d ",  $<valArr>$[i]); printf("\n"); temp[1] = -1;
			temp[0] = -1;}

	EXPI: OP_OP OP_PLUS EXPI EXPI OP_CP {$<number>$ = $<number>3 + $<number>4;}
		| OP_OP OP_MINUS EXPI EXPI OP_CP {$<number>$ = $<number>3 - $<number>4;}
		| OP_OP OP_MULT EXPI EXPI OP_CP {$<number>$ = $<number>3 * $<number>4;}
		| OP_OP OP_DIV EXPI EXPI OP_CP {$<number>$ = $<number>3 / $<number>4;}
		| OP_OP OP_DBLMULT EXPI EXPI OP_CP 
		{
			int x = $<number>3;
			for(int i = 1; i<$<number>4; i++)
			x *= $<number>3;
			$<number>$ = x;
		}

	  | VALUE {$<number>$ = $<number>1;}
	  | IDENTIFIER 
	  {
	    struct variable tt = getFromVariables($<id>1);
	    
	    if(tt.type == 0 && funcCheck == 0)	{printf("SYNTAX ERROR: Variable has no value\n"); exit(1);};
	    if(tt.type != 0 && funcCheck == 0 )
	    {
	     $<number>$ = tt.val;
	    }
	  }

	  | OP_OP KW_SET IDENTIFIER EXPI OP_CP
	  {
	  	if($<output>4 == 2)
	  	{
	  		addVarB($<id>3, $<number>4);
	  		$<output>$ = 2;
	  	}
			else 
			{
				addVarI($<id>3, $<number>4);
			}
			$<number>$ = $<number>4;
			
		}

	  | OP_OP KW_DEFVAR IDENTIFIER EXPI OP_CP
	  {
			if($<output>4 == 2)
	  	{
	  		addVarB($<id>3, $<number>4);
	  		$<output>$ = 2;
	  	}
			else 
			{
				addVarI($<id>3, $<number>4);
			}
			$<number>$ = $<number>4;
			
		}

		| EXPB {$<output>$ = 2; $<number>$ == $<number>1; }

			| OP_OP KW_IF EXPB EXPLISTI OP_CP {$<output>$ = 3;}

			| OP_OP KW_IF EXPB EXPLISTI EXPLISTI OP_CP{$<output>$ = 3;}

			| OP_OP KW_WHILE OP_OP EXPB OP_CP EXPLISTI OP_CP {$<output>$ = 3;}

			| OP_OP KW_FOR OP_OP IDENTIFIER EXPI EXPI OP_CP EXPLISTI OP_CP {$<output>$ = 3;}

			| OP_OP IDENTIFIER EXPLISTI OP_CP 
			{
				struct function tf = getFromFunctions($<id>2);

				if(tf.type == 0)	{printf("SYNTAX ERROR: Function is not defined1\n"); exit(1);};
	   		if(tf.type != 0 && tf.argNum != temp[0])	{printf("SYNTAX ERROR: Function is not defined2\n"); exit(1);};
				$<output>$ = 3;
			}

			| OP_OP KW_DEFFUN IDENTIFIER OP_OP IDLIST OP_CP EXPLISTI OP_CP 
			{
				addFunc($<id>3, idCounter); 
				idCounter = 0;
				$<output>$ = 3;
			}

			| OP_OP KW_DISP EXPI OP_CP 
			{	$<number>$ = $<number>3; $<output>$ = $<output>3;}
			| OP_OP KW_DISP EXPLISTI OP_CP 
			{	copyArr($<valArr>$, $<valArr>3, temp[0]); $<output>$ = 8;}

			| OP_OP KW_EXIT OP_CP	{$<output>$ = 3;exit(0);}

			| COMMENT  {$<output>$ = 3;}


	EXPB:
		  OP_OP KW_AND EXPB EXPB OP_CP {$<number>$ = $<number>3 && $<number>4; $<output>$ = 2;}

		| OP_OP KW_OR EXPB EXPB OP_CP {$<number>$ = $<number>3 || $<number>4; $<output>$ = 2;}

		| OP_OP KW_NOT EXPB OP_CP {if($<number>3 == 0) $<number>$ = 1; else $<number>$ = 0; $<output>$ = 2;}

		| OP_OP KW_EQUAL EXPI EXPI OP_CP 
		{
			if($<number>3 == $<number>4) $<number>$ = 1; 
			else $<number>$ = 0; $<output>$ = 2;
		}

		| OP_OP KW_EQUAL EXPB EXPB OP_CP 
		{
			if($<number>3 == $<number>4) $<number>$ = 1; 
			else $<number>$ = 0; $<output>$ = 2;
		}

		| OP_OP KW_LESS EXPI EXPI OP_CP {if($<number>3 < $<number>4) $<number>$ = 1; else $<number>$ = 0; $<output>$ = 2;}

		| IDENTIFIER 
		{

			struct variable tt = getFromVariables($<id>1);

	    if(tt.type == 0 && funcCheck == 0)	{printf("SYNTAX ERROR: Variable has no value\n"); exit(1);};
	    if(tt.type != 0 && funcCheck == 0 )
	    {
	     $<number>$ = tt.val;
	    }
		}

		| KW_TRUE {$<number>$ = 1; $<output>$ = 2;}

		| KW_FALSE {$<number>$ = 0; $<output>$ = 2;}

		| KW_NIL {$<number>$ = 0; $<output>$ = 2;}
		
	EXPLISTI: 

	OP_OP KW_CONCAT EXPLISTI EXPLISTI OP_CP 
	{
		for(int i = 0; i < temp[1]; i++)
		{
			$<valArr>3[temp[0] + i] = $<valArr>4[i]; 
		}

		int u = temp[0] + temp[1];

		copyArr($<valArr>$, $<valArr>3, u);

		
		temp[1] = -1;
		temp[0] = u;
		
	}

	| OP_OP KW_APPEND EXPI EXPLISTI OP_CP 
	{
				int a [100];
				struct variable t = getFromVariables($<id>3);
					
				if(t.type == 0)
				{
					int p = $<number>3;
					if(p != 0)
					{
						a[0] = p;
						temp[1] = 1;
					}
				}
				else
				{
					a[0] = $<number>3;
					temp[1] = 1;
				}

				for(int i = 0; i < temp[0]; i++)
				{
					a[temp[1] + i] = $<valArr>4[i]; 
				}

				int u = temp[0] + temp[1];

				copyArr($<valArr>$, a, u);
				temp[1] = -1;
				temp[0] = u;
	}

			| LISTVALUE {	
							copyArr($<valArr>$, $<valArr>1, counter);
														
							if(temp[0] == -1)	temp[0] = counter;
							else temp[1] = counter;
							counter = 0;
						}

	LISTVALUE: OP_AP OP_OP VALUES OP_CP { copyArr($<valArr>$, $<valArr>3, counter); }
			 | OP_AP OP_OP OP_CP {}
			 | OP_OP KW_LIST VALUES OP_CP { copyArr($<valArr>$, $<valArr>3, counter); }

	VALUES: VALUES VALUE {$<valArr>$[counter] = $<number>2; counter+= 1;}
						
		  | VALUE	{$<valArr>$[counter] = $<number>1; counter += 1;}

IDLIST:
	IDLIST IDENTIFIER{	idCounter++;}
	| IDENTIFIER	{idCounter++;}

%%

int yyerror(char *msg) 
{ 
  printf("SYNTAX_ERROR: Expression not recognized\n");
  exit(0); 
}

int yywrap()
{
	return 1;
} 

int main(void) 
{
  		printf("Input: ");
  		yyparse();
  		exit(1);
  	
} 