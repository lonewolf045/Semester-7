letter [a-zA-Z]
digit [0-9]
intconst [+\-]?{digit}+
realconst [+\-]?{digit}*\.{digit}+(e[+\-]?{digit}+)?
charconst (\'[\\]?{letter}\')
stringconst (\"[^"\n]*\")
delim	[ \t]
identifier (_|{letter})(_|{letter}|{digit})*

%{
#include "y.tab.h"
int comment = 0;

int validKwrd(char *buff) {
	if(!strcmp("for", buff)) return FOR_TOK;
    else if(!strcmp("while", buff)) return WHILE_TOK;
    else if(!strcmp("else", buff)) return ELSE_TOK;
	else if(!strcmp("do", buff)) return DO_TOK;
    else if(!strcmp("printf", buff)) return PRINT_TOK;
    else if(!strcmp("return", buff)) return RET_TOK;
	else if(!strcmp("scanf", buff)) return SCAN_TOK;
	else if(!strcmp("include", buff)) return INL_TOK;
	else if(!strcmp("if", buff)) return IF_TOK;
	else if(!strcmp("define", buff)) return DEF_TOK;
	else if(!strcmp("int", buff)) return INT_TOK;
	else if(!strcmp("char", buff)) return CHAR_TOK;
	else if(!strcmp("float", buff)) return FLOAT_TOK;
	else if(!strcmp("double", buff)) return DOUBLE_TOK;
	else if(!strcmp("main", buff)) return MAIN_TOK;
	else if(!strcmp("void", buff)) return VOID_TOK;
	else return -1;
}
%}

%%
"/*"                            {comment = 1;}
"*/"                            {if(comment) {comment = 0;} else {printf("ERROR\n");return -1;}}
"//"                            {comment = 1;}
\n                              {if(comment) comment = 0; 
				yylineno++;}
#                               {if(!comment) return (HASH_TOK);}
\.                              {if(!comment) return (DOT_TOK);}
;                               {if(!comment) return (SEMICOLON_TOK);}
,                               {if(!comment) return (COMMA_TOK);}

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
\-=					{if(!comment) return(MINUS_ASSIGN_TOK);}
\*=					{if(!comment) return(MULT_ASSIGN_TOK);}
\/=					{if(!comment) return(DIV_ASSIGN_TOK);}
%=					{if(!comment) return(MOD_ASSIGN_TOK);}
\+					{if(!comment) return(PLUS_TOK);}
-					{if(!comment) return(MINUS_TOK);}
\*					{if(!comment) return(STAR_TOK);}
\/					{if(!comment) return(DIV_TOK);}
%					{if(!comment) return(MOD_TOK);}
"++"					{if(!comment) return(INC_TOK);}
"--"					{if(!comment) return(DEC_TOK);}

\{					{if(!comment) return(LCURL_TOK);}
\}					{if(!comment) return(RCURL_TOK);}
\(					{if(!comment) return(LPAREN_TOK);}
\)					{if(!comment) return(RPAREN_TOK);}
\[                  {if(!comment) return(LSQR_TOK);}
\]                  {if(!comment) return(RSQR_TOK);}
{intconst}				{if(!comment) return(INTCONST);}
{realconst}				{if(!comment) return(REALCONST);}
{charconst}             {if(!comment) return(CHARCONST);}
{stringconst}           {if(!comment) return(STRINGCONST);}

{delim}+				{;}

{identifier}	{
            if(!comment) {
                int i;
                if ((i = validKwrd(yytext)) != -1) {
                    return (i);
                }
                yylval.string = strdup(yytext);
                return (ID_TOK);
            };
    }
%%

/* int main()
{
    int val;
    while((val = yylex()) > 0)
    {
        printf("Token: %s   Token ID: %d\n", yytext, val);
    }
    
    return(0);
}*/
