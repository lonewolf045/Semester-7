%{
#include "lex.yy.c"

void yyerror(char *);
int scope = 0;
%}

%token LPAREN_TOK
%token GT_TOK
%token LT_TOK
%token RPAREN_TOK
%token ASSIGN_TOK
%token MINUS_TOK
%token PLUS_TOK
%token STAR_TOK
%token DIV_TOK
%token MOD_TOK
%token NOT_TOK

%token SEMICOLON_TOK
%token HASH_TOK
%token DOT_TOK
%token COMMA_TOK
%token LCURL_TOK
%token RCURL_TOK
%token LSQUARE_TOK
%token RSQUARE_TOK

%token BIT_AND_TOK
%token BIT_OR_TOK
%token BACKSLASH_TOK
%token SPEECH_TOK
%token INVCOMMA_TOK
%token XOR_TOK
%token NEG_TOK
%token EQ_TOK
%token GTE_TOK
%token LTE_TOK
%token LSHIFT_TOK
%token RSHIFT_TOK
%token AND_TOK
%token OR_TOK
%token NEQ_TOK
%token INC_TOK
%token DEC_TOK

%token PLUS_ASSIGN_TOK
%token MINUS_ASSIGN_TOK
%token MULT_ASSIGN_TOK
%token DIV_ASSIGN_TOK
%token MOD_ASSIGN_TOK

%token DO_TOK
%token WHILE_TOK
%token FOR_TOK
%token IF_TOK
%token ELSE_TOK
%token RET_TOK
%token PRINT_TOK
%token SCAN_TOK
%token DEF_TOK
%token TYPEDEF_TOK
%token BREAK_TOK
%token CONTINUE_TOK
%token SIZEOF_TOK
%token CASE_TOK
%token SWITCH_TOK
%token MAIN_TOK
%token INL_TOK
%token ID_TOK

%token INTCONST
%token CHARCONST
%token REALCONST
%token STRINGCONST

%token INT_TOK
%token CHAR_TOK
%token FLOAT_TOK
%token DOUBLE_TOK
%token VOID_TOK

%start S

%%

HEADER_INC	: HASH_TOK INL_TOK LT_TOK ID_TOK DOT_TOK ID_TOK GT_TOK		{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
       		| HASH_TOK INL_TOK STRINGCONST					{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
		;

MAIN	: INT_TOK MAIN_TOK ARGUMENT_LIST BLOCK  {printf("Main function syntax OK;\tline no:\t%d\n", yylineno);}
	;

S	: HEADER_INC S
  	| HEADER_INC
	| MAIN
	| USER_FN S1
	| DECL_STMT SEMICOLON_TOK S1
	;

S1	: USER_FN S1
	| DECL_STMT SEMICOLON_TOK S1
	| HEADER_INC
	| USER_FN
	| MAIN
	| DECL_STMT SEMICOLON_TOK
	;

ARGUMENT_LIST	: LPAREN_TOK ARGUMENT RPAREN_TOK
     		| LPAREN_TOK RPAREN_TOK
		;

ARGUMENT	: EXPRESSION
    		| EXPRESSION COMMA_TOK ARGUMENT
		;

EXPRESSION	: ID_TOK
	   	| INTCONST
		| REALCONST
		| CHARCONST
		| STRINGCONST
		| FUNC_CALL_STMT
		| LPAREN_TOK EXPRESSION RPAREN_TOK
		| EXPRESSION OPERATOR_ARITH EXPRESSION
		;

OPERATOR_ARITH	: PLUS_TOK
	 	| MINUS_TOK
		| STAR_TOK
		| DIV_TOK
		| MOD_TOK
		;

USER_FN	: datatype ID_TOK PARAMETER_LIST BLOCK {printf("User defined function OK;\tline no:\t%d\n", yylineno);}
		;

PARAMETER_LIST	: LPAREN_TOK PARAMETER RPAREN_TOK
      		| LPAREN_TOK RPAREN_TOK
		;

PARAMETER	: datatype ID_TOK COMMA_TOK PARAMETER
       		| datatype ID_TOK
		;

datatype	: INT_TOK
	 	| FLOAT_TOK
		| DOUBLE_TOK
		| CHAR_TOK
		| VOID_TOK
		;

BLOCK	: LCURL_TOK ENCLOSED RCURL_TOK
      	| LCURL_TOK STATEMENTS RCURL_TOK
	| LCURL_TOK RCURL_TOK
      	;

ENCLOSED	: BLOCK STATEMENTS ENCLOSED
       		| STATEMENTS ENCLOSED
		| STATEMENTS
		;

STATEMENTS	: stmt STATEMENTS
	   	| stmt
		;

stmt	: DECL_STMT SEMICOLON_TOK		{printf("Declaration over;\tline no:\t%d\n", yylineno);}
	| ASSIGN_STMT SEMICOLON_TOK		{printf("Assignment over;\tline no:\t%d\n", yylineno);}
	| CONDITIONAL_STMT			{printf("Conditional block;\tline no:\t%d\n", yylineno);}
	| ITERATIVE_STMT			{printf("Iterative block;\tline no:\t%d\n", yylineno);}
	| FUNC_CALL_STMT SEMICOLON_TOK
	| RET_TOK EXPRESSION SEMICOLON_TOK	{printf("return statement found\tline no: %d\n", yylineno);}
	| PRINT_STMT
	;

DECL_STMT	: datatype VARLIST
		;

VARLIST		: ID_TOK
	    	| ID_TOK COMMA_TOK VARLIST
		| ASSIGN_STMT
		| ASSIGN_STMT COMMA_TOK VARLIST
		;

ASSIGN_STMT	: ID_TOK ASSIGN_TOK EXPRESSION
		;

PRINT_STMT	: PRINT_TOK ARGUMENT_LIST SEMICOLON_TOK {printf("print statement found\tline no: %d\n", yylineno);}
	   	;

CONDITIONAL_STMT	: IF_BLOCK ELSE_IF_BLOCK ELSE_BLOCK
		  	| IF_BLOCK ELSE_IF_BLOCK
			| IF_BLOCK ELSE_BLOCK
			| IF_BLOCK
	    		;

ITERATIVE_STMT	: WHILE_STMT BLOCK
	  	| WHILE_STMT SEMICOLON_TOK
		| FOR_STMT BLOCK
		| FOR_STMT SEMICOLON_TOK
		| DO_TOK BLOCK WHILE_STMT SEMICOLON_TOK
		;

FUNC_CALL_STMT	: ID_TOK ARGUMENT_LIST	{printf("Function call at line no:\t%d\n", yylineno);}
		;

IF_BLOCK	: IF_STMT BLOCK
		| IF_STMT SEMICOLON_TOK
		;

IF_STMT	: IF_TOK LPAREN_TOK CONDITION RPAREN_TOK
      	;

ELSE_IF_BLOCK	: ELSE_TOK IF_STMT BLOCK
		| ELSE_TOK IF_STMT SEMICOLON_TOK
		;

ELSE_BLOCK	: ELSE_TOK BLOCK
	  	| ELSE_TOK SEMICOLON_TOK
		;

CONDITION	: EXPRESSION OPERATOR_REL EXPRESSION
	 	| EXPRESSION
		;

OPERATOR_REL	: GT_TOK
	   	| LT_TOK
		| GTE_TOK
		| LTE_TOK
		| NEQ_TOK
		| EQ_TOK
		| AND_TOK
	   	| OR_TOK
		;

WHILE_STMT	: WHILE_TOK LPAREN_TOK CONDITION RPAREN_TOK
	 	;

FOR_STMT	: FOR_TOK LPAREN_TOK FOR_CONTENTS RPAREN_TOK
       		;

FOR_CONTENTS	: init SEMICOLON_TOK CONDITION SEMICOLON_TOK update
		| init SEMICOLON_TOK CONDITION SEMICOLON_TOK
		| init SEMICOLON_TOK SEMICOLON_TOK
		| SEMICOLON_TOK CONDITION SEMICOLON_TOK update
		| SEMICOLON_TOK CONDITION SEMICOLON_TOK
		| SEMICOLON_TOK SEMICOLON_TOK update
		| SEMICOLON_TOK SEMICOLON_TOK
		;

init	: DECL_STMT COMMA_TOK init_list
 	| ASSIGN_STMT COMMA_TOK init_list
	| DECL_STMT
	| ASSIGN_STMT
	;

init_list	: ASSIGN_STMT COMMA_TOK init_list
      		| ASSIGN_STMT
		;

update	: ASSIGN_STMT COMMA_TOK update
	| ASSIGN_STMT
	| ID_TOK INC_TOK COMMA_TOK update
	| ID_TOK DEC_TOK COMMA_TOK update
	| ID_TOK INC_TOK
	| ID_TOK DEC_TOK
	;

%%

void yyerror(char *s) {
	printf("yyerror: %s\n", s);
}

int main() {
	if(yyparse() == 0){
		printf("Parsed Successfully\n");
	}
	else {
		printf("Syntax error at line no: %d\n", yylineno);
		printf("Last read token: %s\n", yytext);
	}
	return 0;
}

