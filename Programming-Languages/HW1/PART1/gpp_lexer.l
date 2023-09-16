%{
	#include "scanner.h"	
	#include <stdio.h>	
	#include <string.h>	
	#include <math.h>
	int cOpen = 0;
%}

%%

;;.*\n	return COMMENT;

"and" 			{

	return KW_AND;
}

"or" 			{
	
	return KW_OR;
}

"not" return KW_NOT;

"equal" return KW_EQUAL;

"less" return KW_LESS;

"nil" return KW_NIL;

"list" return KW_LIST;

"append" return KW_APPEND;

"concat" return KW_CONCAT;

"set" return KW_SET;

"deffun"		{

	return KW_DEFFUN;
}

"for" return KW_FOR;

"if" return KW_IF;

"exit" return KW_EXIT;

"load" return KW_LOAD;

"disp" return KW_DISP;

"true" return KW_TRUE;

"false" return KW_FALSE;

")"		return OP_CP;	

"("	return OP_OP;	

-[0-9]+"."[0-9]*			{ 		// FLOAT
	return VALUE;
}
[0-9]+"."[0-9]*			{ 		// FLOAT
	return VALUE;
}

-[0-9][0-9]*   return VALUE;		// INTEGER
[0-9][0-9]*   return VALUE;		// INTEGER

[0-9][0-9]*[a-zA-Z][_a-zA-Z0-9]*   return ERROR;		
-[0-9][0-9]*[a-zA-Z][_a-zA-Z0-9]*   return ERROR;		
[0-9]+"."[0-9]*[a-zA-Z][_a-zA-Z0-9]* return ERROR;	
-[0-9]+"."[0-9]*[a-zA-Z][_a-zA-Z0-9]* return ERROR;	

"+" return OP_PLUS;

"-" return OP_MINUS;

"*" return OP_MULT;

"/" return OP_DIV;

"**" return OP_DBLMULT;

"\"" 		{
	if(cOpen == 0)	
	{
		cOpen = 1;
		return OP_OC;
	}
	else if(cOpen == 1)
	{
		cOpen = 0;
		return OP_CC;
	}	
}

"," return OP_COMMA;

[a-zA-Z][_a-zA-Z0-9]*		{

	return IDENTIFIER;

}


[ \n\t]			
{
;
}

.  return ERROR;
%%

int yywrap(void)
{
	return 1;
}