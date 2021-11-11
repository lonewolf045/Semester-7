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
void __init__() {
	for(int i=0;i<TABLE_SIZE;i++) {
		table.entry[i] = NULL;
	}
	table.size = 0;
}

//function to print symmbol table
void displayTable() {
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
int checkIfPresent(char *sym) {
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
int checkIfDeclared(char *sym) {
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
int insert(char *sym) {

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
		//displayTable();
		return 0;
	}

	//in case of a collision, add to the end of list
	while(head->next != NULL) {
		head = head->next;
	}

	head->next = tmp;
	//displayTable();
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
		//displayTable();
		return 0;
	}

	//in case of a collision, add to the end of list
	while(head->next != NULL) {
		head = head->next;
	}

	head->next = tmp;
	//displayTable();
	return 0;
}

//function to get the datatype of a symbol from symbol table
int getType(char *sym) {
	int hval = hash(sym);
	Symbol *head = table.entry[hval];
	while(head != NULL && strcmp(head->name, sym) != 0) {
		head = head->next;
	}
	if(head == NULL) return 0;
	return head->dtype;
}

//function to delete all nodes from a list with matching scope
void delete(int i, int key) {
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
void freeSymbolsFromScope(int s) {
	printf("Table before deleting scope %d:\n", s);
	//displayTable();
	printf("Table size = %d\n", table.size);
	for(int i=0;i<TABLE_SIZE;i++) {
		if(table.entry[i] != NULL) {
			delete(i, s);
			if(table.entry[i] == NULL) {
				table.size--;
			}
		}
	}
	printf("Table after deleting scope %d:\n", s);
	//displayTable();
	printf("Table size = %d\n", table.size);
}

//function to free memory by deleting entire symbol table
void __free__() {
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

//Interim Table 

//file to store the intermediate code generated
FILE *out_file;

//variables to keep track of labels for intermediate code
int interm_label = 0, temp_label = 0, backpatch_label = 0;

//variable to help create new unique temporary variables
int new_tmp_index = 1;

int temp = 0;

//stores intermediate code corresponding to a label
char *interm_table[100];

//expr_var keeps track of last assigned var in expressions/assignment
//temp_var used in backpatching
char expr_var[20], tmp_var[20];

//function to return name of new temporary variable
char *new_temp_var() {
	static char t[5];
	sprintf(t, "t%d", new_tmp_index);
	return t;
}

//function to write quadruple to output array
void generate(int label, char *str) {
	interm_table[label] = (char *)malloc((strlen(str) + 1) * sizeof(char));
	strcpy(interm_table[label], str); 
}

//function to print intermediate code to output file
void write_to_file() {
	for(int i=0;i<100;i++) {
		if(interm_table[i] != NULL) {
			fprintf(out_file, "%d: %s", i, interm_table[i]);
			free(interm_table[i]);
			interm_table[i] = NULL;
		}
	}
}

%}

//defining the yyval union
%union {
	int number;		//for integer constants
	float decimal;	//for real constants
	char *string;	//for identifier names
	struct Dtype {
        int dtype;		//for datatype of expressions
        char *place;
    } Dtype;
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

%left PLUS_TOK MINUS_TOK
%left STAR_TOK DIV_TOK MOD_TOK


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
%type<string> OPERATOR_REL
%type<string> OPERATOR_ARITH
%type<decimal> REALCONST
%type<string> CHARCONST
%type<string> STRINGCONST
%type<Dtype> EXPRESSION
%type<Dtype> FUNC_CALL_STMT
%type<Dtype> CONDITION
%type<string> PLUS_TOK
%type<string> MINUS_TOK
%type<string> STAR_TOK
%type<string> DIV_TOK
%type<string> MOD_TOK
%type<string> GT_TOK
%type<string> LT_TOK
%type<string> GTE_TOK
%type<string> LTE_TOK
%type<string> EQ_TOK
%type<string> NEQ_TOK
%type<string> AND_TOK
%type<string> OR_TOK

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

MAIN:       INT_TOK MAIN_TOK                             {
                                                            char msg[50];
                                                            sprintf(msg,"MAIN: \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                        }
            ARGUMENT_LIST BLOCK                         {
                                                            char msg[50];
                                                            sprintf(msg,"END \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("Main Function: OK \tline no:\t%d\n",yylineno);
                                                        }
		;

USER_FN	:   datatype ID_TOK                             {
                                                            int c = insert($2);
                                                            if(c == -1) {
                                                                char msg[100];
                                                                sprintf(msg, ": Symbol table full");
                                                                yyerror(msg);
                                                            }
                                                            char msg[50];
                                                            sprintf(msg,"%s:\n",$2);
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                        }
            PARAMETER_LIST BLOCK                        {
                                                            char msg[50];
                                                            sprintf(msg,"END \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("%s Function: OK \tline no:\t%d\n",$2,yylineno);
                                                        
                                                        }
		;

PARAMETER_LIST	: LPAREN PARAMETER RPAREN
      		| LPAREN_TOK RPAREN_TOK
		;

PARAMETER	: datatype ID_TOK COMMA_TOK PARAMETER	    {	
                                                            int dec = checkIfDeclared($2);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg,": Multiple declarations of the identifier %s in the same scope",$2);
                                                                yyerror(msg);
                                                            } else if(dec == 2) {
                                                                sprintf(msg,": Conflicting types for identifier %s",$2);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($2);
                                                                if(c == -1) {
                                                                    char msg[100];
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
       		| datatype ID_TOK						    {	
                                                            int dec = checkIfDeclared($2);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg,": Multiple declarations of the identifier %s in the same scope",$2);
                                                                yyerror(msg);
                                                            } else if(dec == 2) {
                                                                sprintf(msg,": Conflicting types for identifier %s",$2);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($2);
                                                                if(c == -1) {
                                                                    char msg[100];
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
		;

datatype	: INT_TOK								    {dtype = 1;}
	 	| FLOAT_TOK									    {dtype = 2;}
		| DOUBLE_TOK								    {dtype = 2;}
		| CHAR_TOK									    {dtype = 4;}
		| VOID_TOK									    {dtype = 5;}
		;

START_BLK: 	LCURL_TOK							        {scope++;}
			;
		
END_BLK: 	RCURL_TOK							        {freeSymbolsFromScope(scope); scope--;}
			;

BLOCK:  START_BLK ENCLOSED END_BLK
	    | START_BLK END_BLK
      	;

ENCLOSED:   BLOCK STATEMENTS ENCLOSED
       		| STATEMENTS ENCLOSED
		    | STATEMENTS
		    ;

STATEMENTS: STATEMENT STATEMENTS
	   	    | STATEMENT
		    ;

STATEMENT:  DECL_STMT SEMICOLON_TOK		                {printf("DECLARATION: FINISHED \tline no:\t%d\n", yylineno);}
	        | ASSIGN_STMT SEMICOLON_TOK		            {printf("ASSIGNMENT: FINISHED \tline no:\t%d\n", yylineno);}
	        | CONDITIONAL_STMT			                {printf("CONDITIONAL: FINISHED \tline no:\t%d\n", yylineno);}
	        | ITERATIVE_STMT			                {printf("ITERATIVE: FINISHED \tline no:\t%d\n", yylineno);}
	        | FUNC_CALL_STMT SEMICOLON_TOK
	        | RET_TOK EXPRESSION SEMICOLON_TOK	        {
                                                            char msg[50];
                                                            sprintf(msg,"RET %s\n",$2.place);
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("RETURN: OK \tline no: %d\n", yylineno);
                                                        }
	        | PRINT_STMT
	        ;


ARGUMENT_LIST:  LPAREN_TOK ARGUMENT RPAREN_TOK
     		    | LPAREN_TOK RPAREN_TOK
		        ;

ARGUMENT:       EXPRESSION
    		    | EXPRESSION COMMA_TOK ARGUMENT
		        ;

EXPRESSION:     ID_TOK								    {	
                                                            if(!checkIfPresent($1)) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undefined identifier found");
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                $$.dtype = getType($1);
                                                                $$.place = strdup($1);
                                                            }
                                                        }
                | ID_TOK ARRAY_ELE						{	
                                                            if(!checkIfPresent($1)) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undefined identifier found");
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                $$.dtype = getType($1);
                                                                $$.place = strdup($1);
                                                            }
                                                        }		
	   	        | INTCONST								{
                                                            $$.dtype = 1;
                                                            char tmp[20];
                                                            sprintf(tmp, "%d", $1);
                                                            $$.place = strdup(tmp);
                                                        }
		        | REALCONST								{  
                                                            $$.dtype = 2;
                                                            char tmp[20];
                                                            sprintf(tmp, "%.2f", $1);
                                                            $$.place = strdup(tmp);
                                                        }
		        | CHARCONST								{   
                                                            $$.dtype = 4;
                                                            char tmp[20];
                                                            sprintf(tmp, "%s", $1);
                                                            $$.place = strdup(tmp);
                                                        }
		        | STRINGCONST							{
                                                            $$.dtype = 5;
                                                            char tmp[20];
                                                            sprintf(tmp, "%s", $1);
                                                            $$.place = strdup(tmp);
                                                        }
		        | FUNC_CALL_STMT						{
                                                            $$.dtype = $1.dtype;
                                                            $$.place = strdup($1.place);
                                                        }
		        | LPAREN_TOK EXPRESSION RPAREN_TOK		{
                                                            $$.dtype = $2.dtype;
                                                            $$.place = strdup($2.place);
                                                        }
		        | EXPRESSION OPERATOR_ARITH EXPRESSION	{
                                                            if($1.dtype != $3.dtype) {
                                                                char msg[100];
                                                                sprintf(msg, ": Type matching error in expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char * tmp = new_temp_var();
                                                                new_tmp_index++;
                                                                $$.place = strdup(tmp);
                                                                char msg[50];
                                                                sprintf(msg, "%s = %s %s %s\n",$$.place,$1.place,$2,$3.place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,$$.place);
                                                                interm_label++;
                                                            }
                                                            $$.dtype = $1.dtype;
                                                        }
		;

OPERATOR_ARITH: PLUS_TOK                                {$$ = strdup($1);}
	 	        | MINUS_TOK                             {$$ = strdup($1);}
		        | STAR_TOK                              {$$ = strdup($1);}
		        | DIV_TOK                               {$$ = strdup($1);}
		        | MOD_TOK                               {$$ = strdup($1);}
		        ;

DECL_STMT:  datatype VARLIST
		    ;

ARRAY_ELE:	LSQR_TOK ID_TOK RSQR_TOK			        {	
                                                            if(!checkIfPresent($2)) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undeclared identifier found");
                                                                yyerror(msg);
                                                            }
                                                        }
			| LSQR_TOK INTCONST RSQR_TOK
			| LSQR_TOK RSQR_TOK				            {	
                                                            char msg[100];
                                                            sprintf(msg, ": Unspecified array element access");
                                                            yyerror(msg);
                                                        }
            ;

ARRAY_SIZE:	LSQR_TOK INTCONST RSQR_TOK
			| LSQR_TOK REALCONST RSQR_TOK		        {	
                                                            char msg[100];
                                                            sprintf(msg, ": Incorrect type for array size");
                                                            yyerror(msg);
                                                        }
			;

VARLIST		: ID_TOK								    {	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
	    	| ID_TOK COMMA_TOK VARLIST				    {	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
		| ID_TOK ASSIGN_TOK EXPRESSION				    {	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                            if(getType($1) != $3.dtype) {
                                                                sprintf(msg, ": Type mismatch between identifier and expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char msg[100];
                                                                sprintf(msg, "%s = %s\n", $1, $3.place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,$1);
                                                                interm_label++;
                                                            }
                                                        }
		| ID_TOK ASSIGN_TOK EXPRESSION COMMA_TOK VARLIST{	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                            if(dtype != $3.dtype) {
                                                                sprintf(msg, ": Type mismatch between identifier and expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char msg[100];
                                                                sprintf(msg, "%s = %s\n", $1, $3.place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,$1);
                                                                interm_label++;
                                                            }
                                                        }
		| ID_TOK ARRAY_ELE COMMA_TOK VARLIST		    {	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
		| ID_TOK ARRAY_SIZE							    {	
                                                            int dec = checkIfDeclared($1);
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",$1);
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", $1);
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert($1);
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
		;

ASSIGN_STMT:    ID_TOK ASSIGN_TOK EXPRESSION			{	
                                                            char msg[100];
                                                            if(!checkIfPresent($1)) {
                                                                sprintf(msg, ": Undefined identifier %s",$1);
                                                                yyerror(msg);
                                                            } else {
                                                                if(getType($1) != $3.dtype) {
                                                                    sprintf(msg, ": Type mismatch between identifier and expression");
                                                                    yyerror(msg);
                                                                } else {
                                                                    char msg[100];
                                                                    sprintf(msg, "%s = %s\n", $1, $3.place);
                                                                    generate(interm_label,msg);
                                                                    strcpy(expr_var,$1);
                                                                    interm_label++;
                                                                }
                                                            }
                                                            
                                                        }
		        ;

PRINT_STMT:     PRINT_TOK ARGUMENT_LIST SEMICOLON_TOK   {printf("PRINT STMT: FINISHED \tline no: %d\n", yylineno);}
	   	        ;

IF_BLOCK:       IF_STMT BLOCK
		        ;

IF_STMT:        IF_TOK                                  {temp_label = interm_label;}
      	        LPAREN CONDITION RPAREN                 {
                                                            char *tmp = new_temp_var();
                                                            new_tmp_index++;
                                                            char msg[50];
                                                            sprintf(msg,"%s = NOT %s\n",tmp,expr_var);
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            backpatch_label = interm_label;
                                                            strcpy(tmp_var,tmp);
                                                            interm_label++;
                                                        }
                ;

ELSE_IF_BLOCK:  ELSE_TOK IF_STMT BLOCK
		        | ELSE_TOK IF_STMT SEMICOLON_TOK
		        ;

ELSE_BLOCK:     ELSE_TOK                                {
                                                            char msg[50];
                                                            sprintf(msg, "IF %s GOTO %d\n", tmp_var, interm_label+1);
                                                            generate(backpatch_label,msg);
                                                            backpatch_label = interm_label;
                                                            interm_label++;
                                                        } 
                BLOCK                                   {
                                                            char msg[50];
                                                            sprintf(msg, "GOTO %d\n", interm_label);
                                                            generate(backpatch_label,msg);
                                                        }
		        ;

CONDITIONAL_STMT:   IF_BLOCK ELSE_IF_BLOCK ELSE_BLOCK
		  	        | IF_BLOCK ELSE_IF_BLOCK
			        | IF_BLOCK ELSE_BLOCK
			        | IF_BLOCK                          {
                                                            char msg[50];
                                                            sprintf(msg,"IF %s GOTO %d\n",tmp_var,interm_label);
                                                            generate(backpatch_label,msg);
                                                        }
	    		    ;

CONDITION:          EXPRESSION OPERATOR_REL EXPRESSION	{	
                                                            if($1.dtype != $3.dtype) {
																char msg[100];
																sprintf(msg, ": Type mismatch of operands");
																yyerror(msg);
															} else {
                                                                char *tmp = new_temp_var();
                                                                new_tmp_index++;
                                                                $$.place = strdup(tmp);
                                                                char msg[50];
                                                                sprintf(msg, "%s = %s %s %s\n", $$.place,$1.place,$2,$3.place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,$$.place);
                                                                interm_label++;
                                                            }
                                                            $$.dtype = $1.dtype;
														}
	 	            | EXPRESSION                        {
                                                            $$.dtype = $1.dtype;
                                                            $$.place = strdup($1.place);
                                                        }
		            ;

OPERATOR_REL:   GT_TOK                                  {$$ = strdup($1);}
	   	        | LT_TOK                                {$$ = strdup($1);}
		        | GTE_TOK                               {$$ = strdup($1);}
		        | LTE_TOK                               {$$ = strdup($1);}
		        | NEQ_TOK                               {$$ = strdup($1);}
		        | EQ_TOK                                {$$ = strdup($1);}
		        | AND_TOK                               {$$ = strdup($1);}
	   	        | OR_TOK                                {$$ = strdup($1);}
		        ;

ITERATIVE_STMT: WHILE_STMT BLOCK                        {
                                                            char msg[50];
                                                            sprintf(msg, "GOTO %d\n", temp_label);
                                                            generate(interm_label,msg);
                                                            interm_label++;

                                                            sprintf(msg, "IF %s GOTO %d\n", tmp_var,interm_label);
                                                            generate(backpatch_label,msg);   
                                                        }
                | WHILE_STMT SEMICOLON_TOK
		        | FOR_STMT BLOCK
		        | FOR_STMT SEMICOLON_TOK
		        | DO_TOK BLOCK WHILE_STMT SEMICOLON_TOK
		        ;

WHILE_STMT:     WHILE_TOK                               {temp_label = interm_label;} 
                LPAREN CONDITION RPAREN                 {
                                                            char *tmp = new_temp_var();
                                                            new_tmp_index++;
                                                            char msg[50];
                                                            sprintf(msg,"%s = NOT %s\n",tmp,expr_var);
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            backpatch_label = interm_label;
                                                            strcpy(tmp_var,tmp);
                                                            interm_label++;
                                                        }
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
	| ID_TOK INC_TOK COMMA_TOK update						{	if(!checkIfPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK DEC_TOK COMMA_TOK update						{	if(!checkIfPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK INC_TOK										{	if(!checkIfPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	| ID_TOK DEC_TOK										{	if(!checkIfPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
	;

FUNC_CALL_STMT: ID_TOK ARGUMENT_LIST						{	
                                                                printf("%s CALL: FOUND \tline no:\t%d\n",$1, yylineno);
																if(!checkIfPresent($1)) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
																else {
																	$$.dtype = getType($1);
                                                                    $$.place = strdup($1);
																}
															}	
				;

%%

void yyerror(char *s) {
	flag = 1;
	printf("Error(%d): %s\n",yylineno, s);
}

int main() {
    yyin = fopen("sample.c","r");
    out_file = fopen("code.txt","w");
    __init__();
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
    write_to_file();
    fclose(yyin);
    fclose(out_file);
    __free__();
	return 0;
}

