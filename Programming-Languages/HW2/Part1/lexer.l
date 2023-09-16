%{ 
	#include <stdio.h>
	#include "y.tab.h"
	#include <math.h>
	int cOpen = 0;
%}

%%
[a][n][d][ ]					return KW_AND;
[o][r][ ]						return KW_OR;
[n][o][t][ ]					return KW_NOT;
[e][q][u][a][l][ ] 				return KW_EQUAL;
[l][e][s][s][ ]					return KW_LESS;
[n][i][l][ ]					return KW_NIL;
[l][i][s][t][ ]					return KW_LIST;
[a][p][p][e][n][d][ ]			return KW_APPEND;
[c][o][n][c][a][t][ ]			return KW_CONCAT;
[s][e][t][ ]					return KW_SET;
[d][e][f][f][u][n][ ]			return KW_DEFFUN;
[d][e][f][v][a][r][ ]			return KW_DEFVAR;
[f][o][r][ ]					return KW_FOR;
[i][f][ ]						return KW_IF;
[e][x][i][t]					return KW_EXIT;
[l][o][a][d][ ]					return KW_LOAD;
[d][i][s][p][ ]					return KW_DISP;
[t][r][u][e]					return KW_TRUE;
[f][a][l][s][e]					return KW_FALSE;

[+][ ] 							return OP_PLUS;
[-][ ] 							return OP_MINUS;
[/][ ] 							return OP_DIV;
[*][*][ ] 						return OP_DBLMULT;
[*][ ] 							return OP_MULT;
[(] 							return OP_OP;
[)] 							return OP_CP; 
[,] 							return OP_COMMA; 
[']								return OP_AP;
["] 							{
									if(cOpen == 0)
									{
										cOpen = 1;
										return OP_CC;
									}
									else
									{
										return OP_OC;
										cOpen = 0;
									}
								}

[a-zA-Z_][a-zA-Z0-9_]*			{
									yylval.id=strdup(yytext);
									return IDENTIFIER;
								} 
	
[0]|[1-9][0-9]*					{	
									yylval.number=atoi(yytext);
									return VALUE;
								}

[;][;][ a-zA-Z0-9\/.+-]+		return COMMENT;

[ \t\n]+ 						{;}

.		{yyerror("invalid token\n");}


%%