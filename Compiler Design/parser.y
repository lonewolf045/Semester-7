%{
#include "lex.yy.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining the max size of symbol table
#define TABLE_SIZE 500

void yyerror(char *s);

//keep trackk of scope during parsing
int scope = 0;

//variable to keep track of datatype
int dtype;
int flag = 0;

//structure defining the individual units of the symbol table
typedef struct val {
	char name[20];
	int dtype;
	int scope;
	//int isPointer;
	struct val *next;
} Symbol;

//defining the structure of the symbol table
typedef struct table {
	Symbol *entry[TABLE_SIZE];
	int size;
} SymTable;

//the global variable for the symbol table
SymTable table;

/*	defining the hash function for the symbol table:
	http://www.cse.yorku.ca/~oz/hash.html => using the djb2 algorithm
*/
int hash (char *str) {
	int h = 5381;
	int c;
	
	while((c = *str++)) {
		h = ((h << 5) + h) + c;	//hash * 33 + c
	}
	
	return h % TABLE_SIZE;
}

//initializing the symbol table
void initTable() {
	for(int i=0;i<TABLE_SIZE;i++) {
		table.entry[i] = NULL;
	}
	table.size = 0;
}

//function to print symmbol table
void printTable() {
	printf("Entry no.\tSymbol name\tDatatype\tScope\n");
	for(int i=0;i<TABLE_SIZE;i++) {
		if(table.entry[i] != NULL) {
			Symbol *head = table.entry[i];
			while(head != NULL) {
				printf("%d\t\t%s\t\t%d\t\t%d\n", i, head->name, head->dtype, head->scope);
				head = head->next;
			}
		}
	}
}

//function to check is a symbol is present in the symbol table
int isPresent(char *sym) {
	int hval = hash(sym);
	Symbol *head = table.entry[hval];

	if(head == NULL) {
		return 0;
	}

	while(head != NULL) {
		if(strcmp(head->name, sym) == 0)
			return 1;
		
		head = head->next;
	}

	return 0;
}

//function to check for a redeclaration
int alreadyDeclared(char *sym) {
	int hval = hash(sym);
	Symbol *head = table.entry[hval];
	
	if(head == NULL) {
		return 0;
	}

	while(head != NULL) {
		//checking if the same symbol name exists in the same scope
		if(strcmp(head->name, sym) == 0 && head->scope == scope) {
			
			//checking if the datatype is also same
			if(head->dtype == dtype)
				return 1;

			else
				return 2;
		}

		head = head->next;
	}

	return 0;
}

//function to add a new entry in the table
int addSymbol(char *sym) {

	if(table.size == TABLE_SIZE) {
		return -1;
	}
	
	int hval = hash(sym);
	Symbol *head = table.entry[hval];
	
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	strcpy(tmp->name, sym);
	tmp->dtype = dtype;
	tmp->scope = scope;
	tmp->next = NULL;

	//in case there is no collision
	if(head == NULL) {
		head = tmp;
		table.size++;
		table.entry[hval] = head;
		//printTable();
		return 0;
	}

	//in case of a collision, add to the end of list
	while(head->next != NULL) {
		head = head->next;
	}

	head->next = tmp;
	//printTable();
	return 0;
}

// function to add a user defined function identifier to symbol table
int addFunction(char *sym) {
	if(table.size == TABLE_SIZE) {
		return -1;
	}
	int hval = hash(sym);
	Symbol *head = table.entry[hval];
	Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
	strcpy(tmp->name, sym);
	tmp->dtype = dtype;
	tmp->scope = 0;
	tmp->next = NULL;
	
	//in case there is no collision
	if(head == NULL) {
		head = tmp;
		table.size++;
		table.entry[hval] = head;
		//printTable();
		return 0;
	}

	//in case of a collision, add to the end of list
	while(head->next != NULL) {
		head = head->next;
	}

	head->next = tmp;
	//printTable();
	return 0;
}

//function to get the datatype of a symbol from symbol table
int getDatatype(char *sym) {
	int hval = hash(sym);
	Symbol *head = table.entry[hval];
	while(head != NULL && strcmp(head->name, sym) != 0) {
		head = head->next;
	}
	if(head == NULL) return 0;
	return head->dtype;
}

//function to delete all nodes from a list with matching scope
void deleteNodes(int i, int key) {
	Symbol *tmp, *p, *q;
	Symbol *head = table.entry[i];

	while(head != NULL && head->scope == key) {
		tmp = head;
		head = head->next;
		free(tmp);
	}

	if(head == NULL) {
		table.entry[i] = head;
		return;
	}

	table.entry[i] = head;
	p = head;
	q = head->next;
	while(q != NULL) {
		if(q->scope == key) {
			p->next = q->next;
			free(q);
			q = p->next;
		}
		else {
			p = p->next;
			q = q->next;
		}
	}
}

//function to delete the symbols of a scope when scope ends
void deleteScopeSyms(int s) {
	printf("Table before deleting scope %d:\n", s);
	printTable();
	printf("Table size = %d\n", table.size);
	for(int i=0;i<TABLE_SIZE;i++) {
		if(table.entry[i] != NULL) {
			deleteNodes(i, s);
			if(table.entry[i] == NULL) {
				table.size--;
			}
		}
	}
	printf("Table after deleting scope %d:\n", s);
	printTable();
	printf("Table size = %d\n", table.size);
}

//function to free memory by deleting entire symbol table
void freeTable() {
	for(int i=0;i<TABLE_SIZE;i++) {
		Symbol *head = table.entry[i];
		Symbol *tmp;
		while(head != NULL) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
	}
}

%}

//defining the yyval union
%union {
	int number;		//for integer constants
	float decimal;	//for real constants
	char *string;	//for identifier names
	int dtype;		//for datatype of expressions
}

// Single caharacter lexemes
%token LPAREN_TOK
%token GT_TOK
%token LT_TOK
%token RPAREN_TOK
%token ASSIGN_TOK
%token MINUS_TOK
%token PLUS_TOK
%token MULT_TOK
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

%left PLUS_TOK MINUS_TOK
%left MULT_TOK DIV_TOK MOD_TOK

//Double character lexemes
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

//Arithmetic assignment operators
%token PLUS_ASSIGN_TOK
%token MINUS_ASSIGN_TOK
%token MULT_ASSIGN_TOK
%token DIV_ASSIGN_TOK
%token MOD_ASSIGN_TOK

// Reserved words
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
%token INCLUDE_TOK
%token PRINTF_TOK
%token SCANF_TOK

// Identifiers, constants
%token ID_TOK
%token INT_CONST_TOK
%token CHAR_CONST_TOK
%token REAL_CONST_TOK
%token STRING_CONST_TOK

// Type specifiers
%token INT_TOK
%token CHAR_TOK
%token FLOAT_TOK
%token DOUBLE_TOK
%token LONG_TOK
%token SHORT_TOK
%token VOID_TOK

//%type tells bison which member of the yyval union is used by which expression
%type<number> INT_CONST_TOK
%type<string> ID_TOK
%type<decimal> REAL_CONST_TOK
%type<dtype> expression
%type<dtype> FUNCTION_CALL

%start S

%%

/*
S:				header USER_FN MAIN_FN
				;
*/

S: 				header S
  				| header
				| MAIN_FN
				| USER_FN S1
				| DECLARE_ASSIGN SEMICOLON_TOK S1
				;

S1: 			USER_FN S1
				| DECLARE_ASSIGN SEMICOLON_TOK S1
				| header
				| USER_FN
				| MAIN_FN
				| DECLARE_ASSIGN SEMICOLON_TOK
				;

open_paren:		LPAREN_TOK											{scope++;}
				;

close_paren:	RPAREN_TOK											{scope--;}
				;

header:			HASH_TOK INCLUDE_TOK LT_TOK ID_TOK DOT_TOK ID_TOK GT_TOK 		{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
    			| HASH_TOK INCLUDE_TOK STRING_CONST_TOK 						{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
				;

MAIN_FN:		INT_TOK MAIN_TOK param block  						{printf("Main function syntax OK;\tline no:\t%d\n", yylineno);}
				;

USER_FN:		datatype ID_TOK param block 						{	printf("User defined function OK;\tline no:\t%d\n", yylineno);
																		int c = addFunction($2);
																		if(c == -1) {
																			char msg[100];
																			sprintf(msg, ": Symbol table full");
																			yyerror(msg);
																		}
																	}
				;

param:			open_paren params close_paren						
    			| LPAREN_TOK RPAREN_TOK
				;

params: 		datatype ID_TOK COMMA_TOK params					{	int c = addSymbol($2);
																		if(c == -1) {
																			char msg[100];
																			sprintf(msg, ": Symbol table full");
																			yyerror(msg);
																		}
																	}
				| datatype ID_TOK									{	int c = addSymbol($2);
																		if(c == -1) {
																			char msg[100];
																			sprintf(msg, ": Symbol table full");
																			yyerror(msg);
																		}
																	}
				;

datatype:		INT_TOK												{dtype = 1;}
	 			| FLOAT_TOK											{dtype = 2;}
				| DOUBLE_TOK										{dtype = 2;}
				| CHAR_TOK											{dtype = 4;}
				| VOID_TOK											{dtype = 5;}
				;

start_block:	LCURL_TOK											{scope++;}
				;

end_block:		RCURL_TOK											{deleteScopeSyms(scope); scope--;}
				;

block:			start_block blocks end_block
      			| start_block end_block
      			;

blocks: 		block STATEMENTS blocks
       			| STATEMENTS blocks
				| STATEMENTS
				;

STATEMENTS:		stmt STATEMENTS
	   			| stmt
				;

stmt:			DECLARE_ASSIGN SEMICOLON_TOK						{printf("Declaration over;\tline no:\t%d\n", yylineno);}
				| EXP_ASSIGN SEMICOLON_TOK							{printf("Assignment over;\tline no:\t%d\n", yylineno);}
				| CONDITIONAL										{printf("Conditional block ends;\tline no:\t%d\n", yylineno);}
				| ITERATIVE											{printf("Iterative block ends;\tline no:\t%d\n", yylineno);}
				| FUNCTION_CALL SEMICOLON_TOK									
				| RET_TOK expression SEMICOLON_TOK					{printf("Return statement found;\tline no:\t%d\n", yylineno);}
				| PRINTF_ST
    			;

PRINTF_ST:		PRINTF_TOK args SEMICOLON_TOK						{printf("printf statement found;\tline no:\t%d\n", yylineno);}
	  			;

expression:		ID_TOK												{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undefined identifier found");
																			yyerror(msg);
																		}
																		else {
																			$$ = getDatatype($1);
																		}
																	}
				| ID_TOK array_ele									{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undefined identifier found");
																			yyerror(msg);
																		}
																		else {
																			$$ = getDatatype($1);
																		}
																	}
	   			| INT_CONST_TOK										{$$ = 1;}
				| REAL_CONST_TOK									{$$ = 2;}
				| CHAR_CONST_TOK									{$$ = 4;}
				| STRING_CONST_TOK									{$$ = 5;}
				| FUNCTION_CALL										{$$ = $1;}
				| LPAREN_TOK expression RPAREN_TOK					{$$ = $2;}
				| expression arith_op expression					{	if($1 != $3) {
																			char msg[100];
																			sprintf(msg, ": Type matching error in expression");
																			yyerror(msg);
																		}
																		$$ = $1;
																	}
				;

arith_op:		PLUS_TOK
	 			| MINUS_TOK
				| MULT_TOK
				| DIV_TOK
				| MOD_TOK
				;

DECLARE_ASSIGN:	datatype varlist
				;

array_ele:		LSQUARE_TOK ID_TOK RSQUARE_TOK						{	if(!isPresent($2)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																	}
				| LSQUARE_TOK INT_CONST_TOK RSQUARE_TOK
				| LSQUARE_TOK RSQUARE_TOK							{	char msg[100];
																		sprintf(msg, ": Unspecified array element access");
																		yyerror(msg);
																	}
				;

array_size:		LSQUARE_TOK INT_CONST_TOK RSQUARE_TOK
				| LSQUARE_TOK REAL_CONST_TOK RSQUARE_TOK			{	char msg[100];
																		sprintf(msg, ": Incorrect type for array size");
																		yyerror(msg);
																	}
				;

varlist:		ID_TOK												{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of the identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																	}
	    		| ID_TOK COMMA_TOK varlist							{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of the identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																	}
				| ID_TOK ASSIGN_TOK expression COMMA_TOK varlist	{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																		if(dtype != $3) {
																			sprintf(msg, ": Type mismatch between identifier and expression");
																			yyerror(msg);
																		}
																	}
				| ID_TOK ASSIGN_TOK expression						{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																		if(getDatatype($1) != $3) {
																			sprintf(msg, ": Type mismatch between identifier and expression");
																			yyerror(msg);
																		}
																	}
				| ID_TOK array_size COMMA_TOK varlist				{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																	}
				| ID_TOK array_size									{	int c = alreadyDeclared($1);
																		char msg[100];
																		if(c == 1) {
																			sprintf(msg, ": Multiple declarations of identifier in same scope");
																			yyerror(msg);
																		}
																		else if(c == 2) {
																			sprintf(msg, ": Conflicting types for identifier");
																			yyerror(msg);
																		}
																		else {
																			int x = addSymbol($1);
																			if(x == -1) {
																				sprintf(msg, ": Symbol table full");
																				yyerror(msg);
																			}
																		}
																	}
				;

EXP_ASSIGN:		ID_TOK ASSIGN_TOK expression						{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undefined identifier found");
																			yyerror(msg);
																		}
																		if(getDatatype($1) != $3) {
																			char msg[100];
																			sprintf(msg, ": Type mismatch between identifier and expression");
																			yyerror(msg);
																		}
																	}
				;

IF_ST:			IF_TOK open_paren cond_exp close_paren
    			;

ELSE_IF_ST:		ELSE_TOK IF_TOK open_paren cond_exp close_paren
	   			;

ELSE_IF_PART:	ELSE_IF_ST block
	     		| ELSE_IF_ST SEMICOLON_TOK
				; 

ELSE_PART:		ELSE_TOK block
	  			| ELSE_TOK SEMICOLON_TOK
				;

IF_PART:		IF_ST block
				| IF_ST SEMICOLON_TOK
				;

CONDITIONAL:	IF_PART ELSE_IF_PART ELSE_PART
	    		| IF_PART ELSE_IF_PART
				| IF_PART ELSE_PART
				| IF_PART
				;

cond_exp:		expression
				| expression comparison expression					{	if($1 != $3) {
																			char msg[100];
																			sprintf(msg, ": Type mismatch of operands");
																			yyerror(msg);
																		}
																	}
				;

comparison:		GT_TOK
	   			| LT_TOK
				| GTE_TOK
				| LTE_TOK
				| NEQ_TOK
				| EQ_TOK
				| AND_TOK
	   			| OR_TOK
				;

WHILE_ST:		WHILE_TOK open_paren cond_exp close_paren
	 			;

FOR_ST:			FOR_TOK open_paren FOR_COND close_paren
       			;

FOR_COND:		for_init SEMICOLON_TOK cond_exp SEMICOLON_TOK inc_dec
				| for_init SEMICOLON_TOK cond_exp SEMICOLON_TOK 
				| for_init SEMICOLON_TOK SEMICOLON_TOK
				| SEMICOLON_TOK cond_exp SEMICOLON_TOK inc_dec
				| SEMICOLON_TOK cond_exp SEMICOLON_TOK
				| SEMICOLON_TOK SEMICOLON_TOK inc_dec
				| SEMICOLON_TOK SEMICOLON_TOK
				;

ITERATIVE:		WHILE_ST block
				| WHILE_ST SEMICOLON_TOK
				| FOR_ST block
				| FOR_ST SEMICOLON_TOK
				| DO_TOK block WHILE_ST SEMICOLON_TOK
				;

for_init:		DECLARE_ASSIGN
				;
				/* COMMA_TOK for_init_list
				| EXP_ASSIGN COMMA_TOK for_init_list
				| DECLARE_ASSIGN
				| EXP_ASSIGN
				;

for_init_list:	EXP_ASSIGN COMMA_TOK for_init_list
				| EXP_ASSIGN
				;*/

inc_dec:		EXP_ASSIGN COMMA_TOK inc_dec
				| EXP_ASSIGN
				| ID_TOK INC_TOK COMMA_TOK inc_dec					{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																	}
				| ID_TOK DEC_TOK COMMA_TOK inc_dec					{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																	}
				| ID_TOK INC_TOK									{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																	}
				| ID_TOK DEC_TOK									{	if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																	}
				;

FUNCTION_CALL:	ID_TOK args											{	printf("Function call statement found;\tline no:\t%d\n", yylineno);
																		if(!isPresent($1)) {
																			char msg[100];
																			sprintf(msg, ": Undeclared identifier found");
																			yyerror(msg);
																		}
																		else {
																			$$ = getDatatype($1);
																		}
																	}
				;

args:			LPAREN_TOK arg RPAREN_TOK
				| LPAREN_TOK RPAREN_TOK
				;

arg:			expression
    			| expression COMMA_TOK arg
				;

%%

void yyerror(char *s) {
	flag = 1;
	printf("\nParsing error at line: %d; Error%s\n\n", yylineno, s);
}

int main() {
	initTable();
	if(yyparse() == 0){
		if(flag == 0)
			printf("Parsed Successfully\n");
		else
			printf("Parsing Finished\n");
	}
	else {
		printf("\nSyntax error at line no.: %d\n", yylineno);
		printf("Last read token: %s\n", yytext);
	}
	freeTable();
	return 0;
}

