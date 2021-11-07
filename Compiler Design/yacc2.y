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
	
	while((c = *(str++))) {
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
	//printTable();
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
	//printTable();
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
%token LSQR_TOK
%token RSQR_TOK

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

//%type tells bison which member of the yyval union is used by which expression
%type<number> INTCONST
%type<string> ID_TOK
%type<decimal> REALCONST
%type<dtype> EXPRESSION
%type<dtype> FUNC_CALL_STMT

%start S

%%

S: 	HEADER_INC S
  	| HEADER_INC
	| MAIN
	| USER_FN S1
	| DECL_STMT SEMICOLON_TOK S1
	;

S1: USER_FN S1
	| DECL_STMT SEMICOLON_TOK S1
	| HEADER_INC
	| USER_FN
	| MAIN
	| DECL_STMT SEMICOLON_TOK
	;

LPAREN: LPAREN_TOK			{scope++;}
		;

RPAREN: RPAREN_TOK			{scope--;}
		;

HEADER_INC: HASH_TOK INL_TOK LT_TOK ID_TOK DOT_TOK ID_TOK GT_TOK		{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
       		| HASH_TOK INL_TOK STRINGCONST					{printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
			;

MAIN: INT_TOK MAIN_TOK ARGUMENT_LIST BLOCK  {printf("Main function syntax OK;\tline no:\t%d\n", yylineno);}
		;

USER_FN	: datatype ID_TOK PARAMETER_LIST BLOCK {printf("User defined function OK;\tline no:\t%d\n", yylineno);
												int c = addFunction($2);
												if(c == -1) {
													char msg[100];
													sprintf(msg, ": Symbol table full");
													yyerror(msg);
												}
												}
		;

PARAMETER_LIST	: LPAREN PARAMETER RPAREN
      		| LPAREN_TOK RPAREN_TOK
		;

PARAMETER	: datatype ID_TOK COMMA_TOK PARAMETER	{	
														int c = addSymbol($2);
														if(c == -1) {
															char msg[100];
															sprintf(msg, ": Symbol table full");
															yyerror(msg);
														}
													}
       		| datatype ID_TOK						{	
														int c = addSymbol($2);
														if(c == -1) {
															char msg[100];
															sprintf(msg, ": Symbol table full");
															yyerror(msg);
														}
													}
		;

datatype	: INT_TOK								{dtype = 1;}
	 	| FLOAT_TOK									{dtype = 2;}
		| DOUBLE_TOK								{dtype = 2;}
		| CHAR_TOK									{dtype = 4;}
		| VOID_TOK									{dtype = 5;}
		;

START_BLK: 	LCURL_TOK							{scope++;}
			;
		
END_BLK: 	RCURL_TOK							{deleteScopeSyms(scope); scope--;}
			;

BLOCK	: START_BLK ENCLOSED END_BLK
      	| START_BLK STATEMENTS END_BLK
	| START_BLK END_BLK
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


ARGUMENT_LIST	: LPAREN_TOK ARGUMENT RPAREN_TOK
     		| LPAREN_TOK RPAREN_TOK
		;

ARGUMENT	: EXPRESSION
    		| EXPRESSION COMMA_TOK ARGUMENT
		;

EXPRESSION: ID_TOK								{	
													if(!isPresent($1)) {
															char msg[100];
															sprintf(msg, ": Undefined identifier found");
															yyerror(msg);
														}
														else {
															$$ = getDatatype($1);
														}
												}
		| ID_TOK ARRAY_ELE						{	
													if(!isPresent($1)) {
															char msg[100];
															sprintf(msg, ": Undefined identifier found");
															yyerror(msg);
														}
														else {
															$$ = getDatatype($1);
														}
												}			
	   	| INTCONST								{$$ = 1;}
		| REALCONST								{$$ = 2;}
		| CHARCONST								{$$ = 4;}
		| STRINGCONST							{$$ = 5;}
		| FUNC_CALL_STMT						{$$ = $1;}
		| LPAREN_TOK EXPRESSION RPAREN_TOK		{$$ = $2;}
		| EXPRESSION OPERATOR_ARITH EXPRESSION	{
													if($1 != $3) {
														char msg[100];
														sprintf(msg, ": Type matching error in expression");
														yyerror(msg);
													}	
													$$ = $1;
												}
		;

OPERATOR_ARITH	: PLUS_TOK
	 	| MINUS_TOK
		| STAR_TOK
		| DIV_TOK
		| MOD_TOK
		;






DECL_STMT	: datatype VARLIST
		;

ARRAY_ELE:	LSQR_TOK ID_TOK RSQR_TOK			{	
														if(!isPresent($2)) {
															char msg[100];
															sprintf(msg, ": Undeclared identifier found");
															yyerror(msg);
														}
													}
			| LSQR_TOK INTCONST RSQR_TOK
			| LSQR_TOK RSQR_TOK				{	
														char msg[100];
														sprintf(msg, ": Unspecified array element access");
														yyerror(msg);
													}

ARRAY_SIZE:	LSQR_TOK INTCONST RSQR_TOK
			| LSQR_TOK REALCONST RSQR_TOK		{	
														char msg[100];
														sprintf(msg, ": Incorrect type for array size");
														yyerror(msg);
													}
			;

VARLIST		: ID_TOK								{	int c = alreadyDeclared($1);
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
	    	| ID_TOK COMMA_TOK VARLIST				{	int c = alreadyDeclared($1);
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
		| ID_TOK ASSIGN_TOK EXPRESSION				{	int c = alreadyDeclared($1);
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
		| ID_TOK ASSIGN_TOK EXPRESSION COMMA_TOK VARLIST	{	int c = alreadyDeclared($1);
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
		| ID_TOK ARRAY_ELE COMMA_TOK VARLIST		{	int c = alreadyDeclared($1);
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
		| ID_TOK ARRAY_SIZE							{	int c = alreadyDeclared($1);
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

ASSIGN_STMT	: ID_TOK ASSIGN_TOK EXPRESSION			{	if(!isPresent($1)) {
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

PRINT_STMT	: PRINT_TOK ARGUMENT_LIST SEMICOLON_TOK {printf("print statement found\tline no: %d\n", yylineno);}
	   	;

CONDITIONAL_STMT	: IF_BLOCK ELSE_IF_BLOCK ELSE_BLOCK
		  	| IF_BLOCK ELSE_IF_BLOCK
			| IF_BLOCK ELSE_BLOCK
			| IF_BLOCK
	    		;



IF_BLOCK	: IF_STMT BLOCK
		| IF_STMT SEMICOLON_TOK
		;

IF_STMT	: IF_TOK LPAREN CONDITION RPAREN
      	;

ELSE_IF_BLOCK	: ELSE_TOK IF_STMT BLOCK
		| ELSE_TOK IF_STMT SEMICOLON_TOK
		;

ELSE_BLOCK	: ELSE_TOK BLOCK
	  	| ELSE_TOK SEMICOLON_TOK
		;

CONDITION	: EXPRESSION OPERATOR_REL EXPRESSION			{	if($1 != $3) {
																	char msg[100];
																	sprintf(msg, ": Type mismatch of operands");
																	yyerror(msg);
																}
															}
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

ITERATIVE_STMT	: WHILE_STMT BLOCK
	  	| WHILE_STMT SEMICOLON_TOK
		| FOR_STMT BLOCK
		| FOR_STMT SEMICOLON_TOK
		| DO_TOK BLOCK WHILE_STMT SEMICOLON_TOK
		;

WHILE_STMT	: WHILE_TOK LPAREN CONDITION RPAREN
	 	;

FOR_STMT	: FOR_TOK LPAREN FOR_CONTENTS RPAREN
       		;

FOR_CONTENTS	: init SEMICOLON_TOK CONDITION SEMICOLON_TOK update
		| init SEMICOLON_TOK CONDITION SEMICOLON_TOK
		| init SEMICOLON_TOK SEMICOLON_TOK
		| SEMICOLON_TOK CONDITION SEMICOLON_TOK update
		| SEMICOLON_TOK CONDITION SEMICOLON_TOK
		| SEMICOLON_TOK SEMICOLON_TOK update
		| SEMICOLON_TOK SEMICOLON_TOK
		;

init: 	DECL_STMT
		;


update	: ASSIGN_STMT COMMA_TOK update
	| ASSIGN_STMT
	| ID_TOK INC_TOK COMMA_TOK update						{	if(!isPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK DEC_TOK COMMA_TOK update						{	if(!isPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK INC_TOK										{	if(!isPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK DEC_TOK										{	if(!isPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	;

FUNC_CALL_STMT: ID_TOK ARGUMENT_LIST						{	printf("Function call statement found;\tline no:\t%d\n", yylineno);
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

%%

void yyerror(char *s) {
	flag = 1;
	printf("yyerror: %s\n", s);
}

int main() {
	if(yyparse() == 0){
		if(flag == 0)
			printf("Parsed Successfully\n");
		else
			printf("Parsing Finished\n");
	}
	else {
		printf("Syntax error at line no: %d\n", yylineno);
		printf("Last read token: %s\n", yytext);
	}
	return 0;
}

