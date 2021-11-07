
letter	[a-zA-Z]
digit	[0-9]
intconst [+\-]?{digit}+
realconst [+\-]?{digit}*\.{digit}+(e[+\-]?{digit}+)?
str_const \"(\\.|[^"\\])*\"
char_const \'(\\.|.)\'
delim	[ \t]

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"

int comment = 0;
%}

%%

"/*"					{comment = 1;}
"*/"					{comment = 0;}
#					{if(!comment) return(HASH_TOK);}
\.					{if(!comment) return(DOT_TOK);}
;					{if(!comment) return(SEMICOLON_TOK);}

int					{if(!comment) return(INT_TOK);}
float					{if(!comment) return(FLOAT_TOK);}
char					{if(!comment) return(CHAR_TOK);}
double					{if(!comment) return(DOUBLE_TOK);}
for					{if(!comment) return(FOR_TOK);}
do					{if(!comment) return(DO_TOK);}
while					{if(!comment) return(WHILE_TOK);}
if					{if(!comment) return(IF_TOK);}
else					{if(!comment) return(ELSE_TOK);}
return					{if(!comment) return(RET_TOK);}
printf					{if(!comment) return(PRINTF_TOK);}
scanf					{if(!comment) return(SCANF_TOK);}
define					{if(!comment) return(DEF_TOK);}
include					{if(!comment) return(INCLUDE_TOK);}
main					{if(!comment) return(MAIN_TOK);}

"++"					{if(!comment) return(INC_TOK);}
"--"					{if(!comment) return(DEC_TOK);}
== 					{if(!comment) return(EQ_TOK);}
\<=					{if(!comment) return(LTE_TOK);}
>=					{if(!comment) return(GTE_TOK);}
!=					{if(!comment) return(NEQ_TOK);}
\<<					{if(!comment) return(LSHIFT_TOK);}
>>					{if(!comment) return(RSHIFT_TOK);}
>					{if(!comment) return(GT_TOK);}
\<					{if(!comment) return(LT_TOK);}
&&					{if(!comment) return(AND_TOK);}
\|\|					{if(!comment) return(OR_TOK);}
!					{if(!comment) return(NOT_TOK);}
&					{if(!comment) return(BIT_AND_TOK);}
\|					{if(!comment) return(BIT_OR_TOK);}
\^					{if(!comment) return(XOR_TOK);}
~					{if(!comment) return(NEG_TOK);}
=					{if(!comment) return(ASSIGN_TOK);}
\+=					{if(!comment) return(PLUS_ASSIGN_TOK);}
-=					{if(!comment) return(MINUS_ASSIGN_TOK);}
\*=					{if(!comment) return(MULT_ASSIGN_TOK);}
\/=					{if(!comment) return(DIV_ASSIGN_TOK);}
%=					{if(!comment) return(MOD_ASSIGN_TOK);}
\+					{if(!comment) return(PLUS_TOK);}
-					{if(!comment) return(MINUS_TOK);}
\*					{if(!comment) return(MULT_TOK);}
\/					{if(!comment) return(DIV_TOK);}
%					{if(!comment) return(MOD_TOK);}
,					{if(!comment) return(COMMA_TOK);}

\{					{if(!comment) return(LCURL_TOK);}
\}					{if(!comment) return(RCURL_TOK);}
\(					{if(!comment) return(LPAREN_TOK);}
\)					{if(!comment) return(RPAREN_TOK);}
\[					{if(!comment) return(LSQUARE_TOK);}
\]					{if(!comment) return(RSQUARE_TOK);}

{intconst}				{if(!comment) return(INT_CONST_TOK);}
{realconst}				{if(!comment) return(REAL_CONST_TOK);}
{str_const}				{if(!comment) return(STRING_CONST_TOK);}
{char_const}				{if(!comment) return(CHAR_CONST_TOK);}
({letter}|_)({letter}|{digit}|_)*	{if(!comment) {yylval.string = strdup(yytext); return(ID_TOK);}}

"\n"					{yylineno++;}
{delim}+				{;}

%%

/*int main() {
	int val;
	while((val = yylex()) > 0) {
		printf("Token: %s\tToken ID: %d\n", yytext, val);
	}
	
	return 0;
}*/
