/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "inter_parser.y"

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


#line 374 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LPAREN_TOK = 258,
    GT_TOK = 259,
    LT_TOK = 260,
    RPAREN_TOK = 261,
    ASSIGN_TOK = 262,
    MINUS_TOK = 263,
    PLUS_TOK = 264,
    STAR_TOK = 265,
    DIV_TOK = 266,
    MOD_TOK = 267,
    NOT_TOK = 268,
    SEMICOLON_TOK = 269,
    HASH_TOK = 270,
    DOT_TOK = 271,
    COMMA_TOK = 272,
    LCURL_TOK = 273,
    RCURL_TOK = 274,
    LSQR_TOK = 275,
    RSQR_TOK = 276,
    BIT_AND_TOK = 277,
    BIT_OR_TOK = 278,
    BACKSLASH_TOK = 279,
    SPEECH_TOK = 280,
    INVCOMMA_TOK = 281,
    XOR_TOK = 282,
    NEG_TOK = 283,
    EQ_TOK = 284,
    GTE_TOK = 285,
    LTE_TOK = 286,
    LSHIFT_TOK = 287,
    RSHIFT_TOK = 288,
    AND_TOK = 289,
    OR_TOK = 290,
    NEQ_TOK = 291,
    INC_TOK = 292,
    DEC_TOK = 293,
    PLUS_ASSIGN_TOK = 294,
    MINUS_ASSIGN_TOK = 295,
    MULT_ASSIGN_TOK = 296,
    DIV_ASSIGN_TOK = 297,
    MOD_ASSIGN_TOK = 298,
    DO_TOK = 299,
    WHILE_TOK = 300,
    FOR_TOK = 301,
    IF_TOK = 302,
    ELSE_TOK = 303,
    RET_TOK = 304,
    PRINT_TOK = 305,
    SCAN_TOK = 306,
    DEF_TOK = 307,
    TYPEDEF_TOK = 308,
    BREAK_TOK = 309,
    CONTINUE_TOK = 310,
    SIZEOF_TOK = 311,
    CASE_TOK = 312,
    SWITCH_TOK = 313,
    MAIN_TOK = 314,
    INL_TOK = 315,
    ID_TOK = 316,
    INTCONST = 317,
    CHARCONST = 318,
    REALCONST = 319,
    STRINGCONST = 320,
    INT_TOK = 321,
    CHAR_TOK = 322,
    FLOAT_TOK = 323,
    DOUBLE_TOK = 324,
    VOID_TOK = 325
  };
#endif
/* Tokens.  */
#define LPAREN_TOK 258
#define GT_TOK 259
#define LT_TOK 260
#define RPAREN_TOK 261
#define ASSIGN_TOK 262
#define MINUS_TOK 263
#define PLUS_TOK 264
#define STAR_TOK 265
#define DIV_TOK 266
#define MOD_TOK 267
#define NOT_TOK 268
#define SEMICOLON_TOK 269
#define HASH_TOK 270
#define DOT_TOK 271
#define COMMA_TOK 272
#define LCURL_TOK 273
#define RCURL_TOK 274
#define LSQR_TOK 275
#define RSQR_TOK 276
#define BIT_AND_TOK 277
#define BIT_OR_TOK 278
#define BACKSLASH_TOK 279
#define SPEECH_TOK 280
#define INVCOMMA_TOK 281
#define XOR_TOK 282
#define NEG_TOK 283
#define EQ_TOK 284
#define GTE_TOK 285
#define LTE_TOK 286
#define LSHIFT_TOK 287
#define RSHIFT_TOK 288
#define AND_TOK 289
#define OR_TOK 290
#define NEQ_TOK 291
#define INC_TOK 292
#define DEC_TOK 293
#define PLUS_ASSIGN_TOK 294
#define MINUS_ASSIGN_TOK 295
#define MULT_ASSIGN_TOK 296
#define DIV_ASSIGN_TOK 297
#define MOD_ASSIGN_TOK 298
#define DO_TOK 299
#define WHILE_TOK 300
#define FOR_TOK 301
#define IF_TOK 302
#define ELSE_TOK 303
#define RET_TOK 304
#define PRINT_TOK 305
#define SCAN_TOK 306
#define DEF_TOK 307
#define TYPEDEF_TOK 308
#define BREAK_TOK 309
#define CONTINUE_TOK 310
#define SIZEOF_TOK 311
#define CASE_TOK 312
#define SWITCH_TOK 313
#define MAIN_TOK 314
#define INL_TOK 315
#define ID_TOK 316
#define INTCONST 317
#define CHARCONST 318
#define REALCONST 319
#define STRINGCONST 320
#define INT_TOK 321
#define CHAR_TOK 322
#define FLOAT_TOK 323
#define DOUBLE_TOK 324
#define VOID_TOK 325

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 306 "inter_parser.y"

	int number;		//for integer constants
	float decimal;	//for real constants
	char *string;	//for identifier names
	struct Dtype {
        int dtype;		//for datatype of expressions
        char *place;
    } Dtype;

#line 576 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  203

#define YYUNDEFTOK  2
#define YYMAXUTOK   325


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   424,   424,   425,   426,   427,   428,   431,   432,   433,
     434,   435,   436,   439,   442,   445,   446,   449,   449,   464,
     464,   486,   487,   490,   509,   530,   531,   532,   533,   534,
     537,   540,   543,   544,   547,   548,   549,   552,   553,   556,
     557,   558,   559,   560,   561,   568,   572,   573,   576,   577,
     580,   591,   602,   608,   614,   620,   626,   630,   634,   653,
     654,   655,   656,   657,   660,   663,   670,   671,   678,   679,
     686,   705,   724,   753,   782,   801,   822,   843,   846,   849,
     849,   863,   864,   867,   867,   881,   882,   883,   884,   891,
     908,   914,   915,   916,   917,   918,   919,   920,   921,   924,
     933,   934,   935,   936,   939,   939,   953,   956,   957,   958,
     959,   960,   961,   962,   965,   969,   970,   971,   977,   983,
     989,   997
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LPAREN_TOK", "GT_TOK", "LT_TOK",
  "RPAREN_TOK", "ASSIGN_TOK", "MINUS_TOK", "PLUS_TOK", "STAR_TOK",
  "DIV_TOK", "MOD_TOK", "NOT_TOK", "SEMICOLON_TOK", "HASH_TOK", "DOT_TOK",
  "COMMA_TOK", "LCURL_TOK", "RCURL_TOK", "LSQR_TOK", "RSQR_TOK",
  "BIT_AND_TOK", "BIT_OR_TOK", "BACKSLASH_TOK", "SPEECH_TOK",
  "INVCOMMA_TOK", "XOR_TOK", "NEG_TOK", "EQ_TOK", "GTE_TOK", "LTE_TOK",
  "LSHIFT_TOK", "RSHIFT_TOK", "AND_TOK", "OR_TOK", "NEQ_TOK", "INC_TOK",
  "DEC_TOK", "PLUS_ASSIGN_TOK", "MINUS_ASSIGN_TOK", "MULT_ASSIGN_TOK",
  "DIV_ASSIGN_TOK", "MOD_ASSIGN_TOK", "DO_TOK", "WHILE_TOK", "FOR_TOK",
  "IF_TOK", "ELSE_TOK", "RET_TOK", "PRINT_TOK", "SCAN_TOK", "DEF_TOK",
  "TYPEDEF_TOK", "BREAK_TOK", "CONTINUE_TOK", "SIZEOF_TOK", "CASE_TOK",
  "SWITCH_TOK", "MAIN_TOK", "INL_TOK", "ID_TOK", "INTCONST", "CHARCONST",
  "REALCONST", "STRINGCONST", "INT_TOK", "CHAR_TOK", "FLOAT_TOK",
  "DOUBLE_TOK", "VOID_TOK", "$accept", "S", "S1", "LPAREN", "RPAREN",
  "HEADER_INC", "MAIN", "$@1", "USER_FN", "$@2", "PARAMETER_LIST",
  "PARAMETER", "datatype", "START_BLK", "END_BLK", "BLOCK", "ENCLOSED",
  "STATEMENTS", "STATEMENT", "ARGUMENT_LIST", "ARGUMENT", "EXPRESSION",
  "OPERATOR_ARITH", "DECL_STMT", "ARRAY_ELE", "ARRAY_SIZE", "VARLIST",
  "ASSIGN_STMT", "PRINT_STMT", "IF_BLOCK", "IF_STMT", "$@3",
  "ELSE_IF_BLOCK", "ELSE_BLOCK", "$@4", "CONDITIONAL_STMT", "CONDITION",
  "OPERATOR_REL", "ITERATIVE_STMT", "WHILE_STMT", "$@5", "FOR_STMT",
  "FOR_CONTENTS", "init", "update", "FUNC_CALL_STMT", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325
};
# endif

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-71)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      62,   -37,   -45,  -137,  -137,  -137,  -137,    33,    62,  -137,
      62,   -14,    40,     1,  -137,  -137,  -137,  -137,  -137,  -137,
      62,    53,   197,  -137,    62,    20,  -137,    79,  -137,    62,
      41,    35,    14,    98,   106,  -137,  -137,   108,    24,   118,
    -137,    41,    80,  -137,  -137,  -137,  -137,   145,  -137,   105,
    -137,  -137,   121,   150,   151,   120,   186,   118,    35,    90,
    -137,   177,   216,  -137,   100,  -137,   232,    36,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,    35,    41,  -137,   179,  -137,
    -137,  -137,   188,   134,  -137,  -137,   198,  -137,    41,  -137,
     118,  -137,   200,  -137,    41,    79,    18,    35,  -137,   140,
     194,   130,   140,   191,   201,  -137,   170,   118,  -137,  -137,
     164,   174,   215,  -137,   211,  -137,   249,  -137,  -137,   214,
    -137,  -137,   190,   200,  -137,    47,   200,   237,   222,    41,
     130,  -137,  -137,  -137,  -137,  -137,   203,   189,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,   186,   248,    41,    29,
    -137,   188,   250,    41,  -137,  -137,   249,  -137,   202,   118,
    -137,  -137,  -137,  -137,   129,   188,   204,   252,  -137,    46,
     188,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,    41,  -137,     0,   246,  -137,   204,  -137,   253,
    -137,   249,   251,   254,   204,  -137,   204,   204,   204,  -137,
    -137,  -137,  -137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    25,    28,    26,    27,    29,     0,     3,     4,
       0,     0,     0,     0,    17,     1,     2,     5,     9,    11,
      10,     0,    19,    64,     0,     0,    16,     0,     7,    12,
       0,     0,     0,     0,     0,    75,     6,     0,     0,     0,
       8,     0,    50,    52,    54,    53,    55,    72,    56,    70,
      71,    67,     0,     0,     0,    13,     0,     0,     0,     0,
      47,     0,    48,    30,     0,    18,     0,     0,   121,    51,
      60,    59,    61,    62,    63,     0,     0,    65,    66,    69,
      22,    25,     0,     0,    20,    74,     0,    46,     0,    31,
       0,   104,     0,    79,     0,     0,     0,     0,    33,     0,
       0,    36,    38,     0,     0,    45,    88,     0,    41,    42,
       0,     0,     0,    57,     0,    73,    58,    14,    21,    24,
      15,    49,     0,     0,    13,     0,     0,     0,     0,     0,
       0,    32,    35,    37,    39,    40,    83,    86,    87,    78,
     100,    99,   102,   101,    43,    66,     0,     0,     0,     0,
     114,     0,     0,     0,    44,    77,    76,    34,     0,     0,
      83,    85,    23,   103,    90,     0,   113,     0,   106,     0,
       0,    82,    81,    84,    91,    92,    96,    93,    94,    97,
      98,    95,     0,   105,     0,   116,   112,   111,   109,     0,
      80,    89,   119,   120,     0,   110,   108,     0,     0,   115,
     107,   117,   118
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,   261,   210,   -64,  -136,     9,   135,  -137,   173,  -137,
    -137,   124,   -51,  -137,   172,   -38,   -85,   -34,  -137,   -24,
     185,   -30,  -137,     2,   233,  -137,     5,   -60,  -137,  -137,
     138,  -137,  -137,   139,  -137,  -137,  -129,  -137,  -137,   155,
    -137,  -137,  -137,  -137,    25,   -46
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,    17,    56,   118,    18,    19,    27,    20,    33,
      57,    82,    11,    64,    98,    99,   100,   101,   102,    68,
      61,   164,    76,   103,    34,    35,    23,   185,   105,   106,
     107,   126,   137,   138,   159,   108,   165,   182,   109,   110,
     123,   111,   151,   152,   186,    48
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      47,    65,    12,    39,   104,    83,    25,   129,    62,     8,
      12,    66,    21,    97,    14,   168,   132,     8,   112,    84,
     167,    38,    21,    13,   170,   129,    21,    41,   125,   183,
      60,    21,    41,    15,   190,    51,    50,   192,   193,   104,
     189,   104,   104,   166,    41,   157,   116,    22,    97,    41,
      97,    97,   122,   112,    24,   112,   112,    51,    62,   148,
     188,   149,   153,    85,   127,   130,    26,    29,   133,   139,
     104,   128,   141,   143,    97,    52,    53,     1,    54,    97,
     115,    37,    38,    38,   112,    42,    43,    44,    45,    46,
      42,    43,    44,    45,    46,    83,    49,    52,   114,   156,
      67,    55,    42,    43,    44,    45,    46,    42,    43,    44,
      45,    46,    30,    81,     3,     4,     5,     6,    63,    89,
     172,   173,    31,    58,    59,    32,    80,   150,     2,     3,
       4,     5,     6,   174,   175,     9,    63,    70,    71,    72,
      73,    74,    77,     9,    90,    91,    92,    93,    63,    94,
      95,    86,   191,    70,    71,    72,    73,    74,   176,   177,
     178,    96,    75,   179,   180,   181,    81,     3,     4,     5,
       6,    78,    79,    10,    90,    91,    92,    93,   140,    94,
      95,    10,    63,    87,    90,    91,    92,    93,   142,    94,
      95,    96,    63,   -68,   117,   119,    81,     3,     4,     5,
       6,    96,   120,   124,    30,   134,    81,     3,     4,     5,
       6,   -70,   195,    89,    31,   135,   171,    32,   136,   199,
      63,   200,   201,   202,    70,    71,    72,    73,    74,   144,
      28,   146,   145,    88,    36,    91,   155,   160,   113,    40,
      70,    71,    72,    73,    74,    70,    71,    72,    73,    74,
      93,   154,    81,     3,     4,     5,     6,    70,    71,    72,
      73,    74,   163,   194,   169,   184,   187,   196,   197,    16,
     162,   198,   131,   121,   158,    69,   161,   147
};

static const yytype_uint8 yycheck[] =
{
      30,    39,     0,    27,    64,    56,     5,     7,    38,     0,
       8,    41,    10,    64,    59,   151,   101,     8,    64,    57,
     149,     3,    20,    60,   153,     7,    24,     3,    92,   165,
       6,    29,     3,     0,   170,    21,    31,    37,    38,    99,
     169,   101,   102,    14,     3,   130,    76,    61,    99,     3,
     101,   102,    90,    99,    14,   101,   102,    21,    88,   123,
      14,    14,   126,    58,    94,    99,    65,    14,   102,   107,
     130,    95,   110,   111,   125,    61,    62,    15,    64,   130,
      75,    61,     3,     3,   130,    61,    62,    63,    64,    65,
      61,    62,    63,    64,    65,   146,    61,    61,    62,   129,
      20,     3,    61,    62,    63,    64,    65,    61,    62,    63,
      64,    65,     7,    66,    67,    68,    69,    70,    18,    19,
     158,   159,    17,    17,    16,    20,     6,   125,    66,    67,
      68,    69,    70,     4,     5,     0,    18,     8,     9,    10,
      11,    12,    21,     8,    44,    45,    46,    47,    18,    49,
      50,    61,   182,     8,     9,    10,    11,    12,    29,    30,
      31,    61,    17,    34,    35,    36,    66,    67,    68,    69,
      70,    21,    21,     0,    44,    45,    46,    47,    14,    49,
      50,     8,    18,     6,    44,    45,    46,    47,    14,    49,
      50,    61,    18,    14,     6,    61,    66,    67,    68,    69,
      70,    61,     4,     3,     7,    14,    66,    67,    68,    69,
      70,    14,   187,    19,    17,    14,    14,    20,    48,   194,
      18,   196,   197,   198,     8,     9,    10,    11,    12,    14,
      20,    17,    21,    17,    24,    45,    14,    48,     6,    29,
       8,     9,    10,    11,    12,     8,     9,    10,    11,    12,
      47,    14,    66,    67,    68,    69,    70,     8,     9,    10,
      11,    12,    14,    17,    14,    61,    14,    14,    17,     8,
     146,    17,   100,    88,   136,    42,   137,   122
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    15,    66,    67,    68,    69,    70,    72,    76,    77,
      79,    83,    94,    60,    59,     0,    72,    73,    76,    77,
      79,    94,    61,    97,    14,     5,    65,    78,    73,    14,
       7,    17,    20,    80,    95,    96,    73,    61,     3,    90,
      73,     3,    61,    62,    63,    64,    65,    92,   116,    61,
      97,    21,    61,    62,    64,     3,    74,    81,    17,    16,
       6,    91,    92,    18,    84,    86,    92,    20,    90,    95,
       8,     9,    10,    11,    12,    17,    93,    21,    21,    21,
       6,    66,    82,    83,    86,    97,    61,     6,    17,    19,
      44,    45,    46,    47,    49,    50,    61,    83,    85,    86,
      87,    88,    89,    94,    98,    99,   100,   101,   106,   109,
     110,   112,   116,     6,    62,    97,    92,     6,    75,    61,
       4,    91,    86,   111,     3,    74,   102,    92,    90,     7,
      88,    85,    87,    88,    14,    14,    48,   103,   104,    86,
      14,    86,    14,    86,    14,    21,    17,   110,    74,    14,
      94,   113,   114,    74,    14,    14,    92,    87,   101,   105,
      48,   104,    82,    14,    92,   107,    14,   107,    75,    14,
     107,    14,    86,    86,     4,     5,    29,    30,    31,    34,
      35,    36,   108,    75,    61,    98,   115,    14,    14,   107,
      75,    92,    37,    38,    17,   115,    14,    17,    17,   115,
     115,   115,   115
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    71,    72,    72,    72,    72,    72,    73,    73,    73,
      73,    73,    73,    74,    75,    76,    76,    78,    77,    80,
      79,    81,    81,    82,    82,    83,    83,    83,    83,    83,
      84,    85,    86,    86,    87,    87,    87,    88,    88,    89,
      89,    89,    89,    89,    89,    89,    90,    90,    91,    91,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    93,
      93,    93,    93,    93,    94,    95,    95,    95,    96,    96,
      97,    97,    97,    97,    97,    97,    98,    99,   100,   102,
     101,   103,   103,   105,   104,   106,   106,   106,   106,   107,
     107,   108,   108,   108,   108,   108,   108,   108,   108,   109,
     109,   109,   109,   109,   111,   110,   112,   113,   113,   113,
     113,   113,   113,   113,   114,   115,   115,   115,   115,   115,
     115,   116
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     3,     2,     3,     1,
       1,     1,     2,     1,     1,     7,     3,     0,     5,     0,
       5,     3,     2,     4,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     3,     2,     1,     2,     1,     2,
       2,     1,     1,     2,     3,     1,     3,     2,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     2,     3,     3,     2,     3,     3,
       1,     3,     3,     5,     4,     2,     3,     3,     2,     0,
       5,     3,     3,     0,     3,     3,     2,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     4,     0,     5,     4,     5,     4,     3,
       4,     3,     3,     2,     1,     3,     1,     4,     4,     2,
       2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 13:
#line 439 "inter_parser.y"
                                        {scope++;}
#line 1930 "y.tab.c"
    break;

  case 14:
#line 442 "inter_parser.y"
                                        {scope--;}
#line 1936 "y.tab.c"
    break;

  case 15:
#line 445 "inter_parser.y"
                                                                                {printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
#line 1942 "y.tab.c"
    break;

  case 16:
#line 446 "inter_parser.y"
                                                                                {printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
#line 1948 "y.tab.c"
    break;

  case 17:
#line 449 "inter_parser.y"
                                                         {
                                                            char msg[50];
                                                            sprintf(msg,"MAIN: \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                        }
#line 1959 "y.tab.c"
    break;

  case 18:
#line 455 "inter_parser.y"
                                                        {
                                                            char msg[50];
                                                            sprintf(msg,"END \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("Main Function: OK \tline no:\t%d\n",yylineno);
                                                        }
#line 1971 "y.tab.c"
    break;

  case 19:
#line 464 "inter_parser.y"
                                                        {
                                                            int c = insert((yyvsp[0].string));
                                                            if(c == -1) {
                                                                char msg[100];
                                                                sprintf(msg, ": Symbol table full");
                                                                yyerror(msg);
                                                            }
                                                            char msg[50];
                                                            sprintf(msg,"%s:\n",(yyvsp[0].string));
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                        }
#line 1988 "y.tab.c"
    break;

  case 20:
#line 476 "inter_parser.y"
                                                        {
                                                            char msg[50];
                                                            sprintf(msg,"END \n");
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("%s Function: OK \tline no:\t%d\n",(yyvsp[-3].string),yylineno);
                                                        
                                                        }
#line 2001 "y.tab.c"
    break;

  case 23:
#line 490 "inter_parser.y"
                                                            {	
                                                            int dec = checkIfDeclared((yyvsp[-2].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg,": Multiple declarations of the identifier %s in the same scope",(yyvsp[-2].string));
                                                                yyerror(msg);
                                                            } else if(dec == 2) {
                                                                sprintf(msg,": Conflicting types for identifier %s",(yyvsp[-2].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-2].string));
                                                                if(c == -1) {
                                                                    char msg[100];
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2025 "y.tab.c"
    break;

  case 24:
#line 509 "inter_parser.y"
                                                                                    {	
                                                            int dec = checkIfDeclared((yyvsp[0].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg,": Multiple declarations of the identifier %s in the same scope",(yyvsp[0].string));
                                                                yyerror(msg);
                                                            } else if(dec == 2) {
                                                                sprintf(msg,": Conflicting types for identifier %s",(yyvsp[0].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[0].string));
                                                                if(c == -1) {
                                                                    char msg[100];
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2049 "y.tab.c"
    break;

  case 25:
#line 530 "inter_parser.y"
                                                                                            {dtype = 1;}
#line 2055 "y.tab.c"
    break;

  case 26:
#line 531 "inter_parser.y"
                                                                                                    {dtype = 2;}
#line 2061 "y.tab.c"
    break;

  case 27:
#line 532 "inter_parser.y"
                                                                                            {dtype = 2;}
#line 2067 "y.tab.c"
    break;

  case 28:
#line 533 "inter_parser.y"
                                                                                                    {dtype = 4;}
#line 2073 "y.tab.c"
    break;

  case 29:
#line 534 "inter_parser.y"
                                                                                                    {dtype = 5;}
#line 2079 "y.tab.c"
    break;

  case 30:
#line 537 "inter_parser.y"
                                                                                        {scope++;}
#line 2085 "y.tab.c"
    break;

  case 31:
#line 540 "inter_parser.y"
                                                                                        {freeSymbolsFromScope(scope); scope--;}
#line 2091 "y.tab.c"
    break;

  case 39:
#line 556 "inter_parser.y"
                                                                {printf("DECLARATION: FINISHED \tline no:\t%d\n", yylineno);}
#line 2097 "y.tab.c"
    break;

  case 40:
#line 557 "inter_parser.y"
                                                                    {printf("ASSIGNMENT: FINISHED \tline no:\t%d\n", yylineno);}
#line 2103 "y.tab.c"
    break;

  case 41:
#line 558 "inter_parser.y"
                                                                        {printf("CONDITIONAL: FINISHED \tline no:\t%d\n", yylineno);}
#line 2109 "y.tab.c"
    break;

  case 42:
#line 559 "inter_parser.y"
                                                                        {printf("ITERATIVE: FINISHED \tline no:\t%d\n", yylineno);}
#line 2115 "y.tab.c"
    break;

  case 44:
#line 561 "inter_parser.y"
                                                                {
                                                            char msg[50];
                                                            sprintf(msg,"RET %s\n",(yyvsp[-1].Dtype).place);
                                                            generate(interm_label,msg);
                                                            interm_label++;
                                                            printf("RETURN: OK \tline no: %d\n", yylineno);
                                                        }
#line 2127 "y.tab.c"
    break;

  case 50:
#line 580 "inter_parser.y"
                                                                                    {	
                                                            if(!checkIfPresent((yyvsp[0].string))) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undefined identifier found");
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                (yyval.Dtype).dtype = getType((yyvsp[0].string));
                                                                (yyval.Dtype).place = strdup((yyvsp[0].string));
                                                            }
                                                        }
#line 2143 "y.tab.c"
    break;

  case 51:
#line 591 "inter_parser.y"
                                                                                {	
                                                            if(!checkIfPresent((yyvsp[-1].string))) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undefined identifier found");
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                (yyval.Dtype).dtype = getType((yyvsp[-1].string));
                                                                (yyval.Dtype).place = strdup((yyvsp[-1].string));
                                                            }
                                                        }
#line 2159 "y.tab.c"
    break;

  case 52:
#line 602 "inter_parser.y"
                                                                                                {
                                                            (yyval.Dtype).dtype = 1;
                                                            char tmp[20];
                                                            sprintf(tmp, "%d", (yyvsp[0].number));
                                                            (yyval.Dtype).place = strdup(tmp);
                                                        }
#line 2170 "y.tab.c"
    break;

  case 53:
#line 608 "inter_parser.y"
                                                                                                {  
                                                            (yyval.Dtype).dtype = 2;
                                                            char tmp[20];
                                                            sprintf(tmp, "%.2f", (yyvsp[0].decimal));
                                                            (yyval.Dtype).place = strdup(tmp);
                                                        }
#line 2181 "y.tab.c"
    break;

  case 54:
#line 614 "inter_parser.y"
                                                                                                {   
                                                            (yyval.Dtype).dtype = 4;
                                                            char tmp[20];
                                                            sprintf(tmp, "%s", (yyvsp[0].string));
                                                            (yyval.Dtype).place = strdup(tmp);
                                                        }
#line 2192 "y.tab.c"
    break;

  case 55:
#line 620 "inter_parser.y"
                                                                                        {
                                                            (yyval.Dtype).dtype = 5;
                                                            char tmp[20];
                                                            sprintf(tmp, "%s", (yyvsp[0].string));
                                                            (yyval.Dtype).place = strdup(tmp);
                                                        }
#line 2203 "y.tab.c"
    break;

  case 56:
#line 626 "inter_parser.y"
                                                                                        {
                                                            (yyval.Dtype).dtype = (yyvsp[0].Dtype).dtype;
                                                            (yyval.Dtype).place = strdup((yyvsp[0].Dtype).place);
                                                        }
#line 2212 "y.tab.c"
    break;

  case 57:
#line 630 "inter_parser.y"
                                                                        {
                                                            (yyval.Dtype).dtype = (yyvsp[-1].Dtype).dtype;
                                                            (yyval.Dtype).place = strdup((yyvsp[-1].Dtype).place);
                                                        }
#line 2221 "y.tab.c"
    break;

  case 58:
#line 634 "inter_parser.y"
                                                                {
                                                            if((yyvsp[-2].Dtype).dtype != (yyvsp[0].Dtype).dtype) {
                                                                char msg[100];
                                                                sprintf(msg, ": Type matching error in expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char * tmp = new_temp_var();
                                                                new_tmp_index++;
                                                                (yyval.Dtype).place = strdup(tmp);
                                                                char msg[50];
                                                                sprintf(msg, "%s = %s %s %s\n",(yyval.Dtype).place,(yyvsp[-2].Dtype).place,(yyvsp[-1].string),(yyvsp[0].Dtype).place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,(yyval.Dtype).place);
                                                                interm_label++;
                                                            }
                                                            (yyval.Dtype).dtype = (yyvsp[-2].Dtype).dtype;
                                                        }
#line 2243 "y.tab.c"
    break;

  case 59:
#line 653 "inter_parser.y"
                                                        {(yyval.string) = strdup((yyvsp[0].string));}
#line 2249 "y.tab.c"
    break;

  case 60:
#line 654 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2255 "y.tab.c"
    break;

  case 61:
#line 655 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2261 "y.tab.c"
    break;

  case 62:
#line 656 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2267 "y.tab.c"
    break;

  case 63:
#line 657 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2273 "y.tab.c"
    break;

  case 65:
#line 663 "inter_parser.y"
                                                                        {	
                                                            if(!checkIfPresent((yyvsp[-1].string))) {
                                                                char msg[100];
                                                                sprintf(msg, ": Undeclared identifier found");
                                                                yyerror(msg);
                                                            }
                                                        }
#line 2285 "y.tab.c"
    break;

  case 67:
#line 671 "inter_parser.y"
                                                                                    {	
                                                            char msg[100];
                                                            sprintf(msg, ": Unspecified array element access");
                                                            yyerror(msg);
                                                        }
#line 2295 "y.tab.c"
    break;

  case 69:
#line 679 "inter_parser.y"
                                                                        {	
                                                            char msg[100];
                                                            sprintf(msg, ": Incorrect type for array size");
                                                            yyerror(msg);
                                                        }
#line 2305 "y.tab.c"
    break;

  case 70:
#line 686 "inter_parser.y"
                                                                                            {	
                                                            int dec = checkIfDeclared((yyvsp[0].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[0].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[0].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[0].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2329 "y.tab.c"
    break;

  case 71:
#line 705 "inter_parser.y"
                                                                            {	
                                                            int dec = checkIfDeclared((yyvsp[-2].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[-2].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[-2].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-2].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2353 "y.tab.c"
    break;

  case 72:
#line 724 "inter_parser.y"
                                                                            {	
                                                            int dec = checkIfDeclared((yyvsp[-2].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[-2].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[-2].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-2].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                            if(getType((yyvsp[-2].string)) != (yyvsp[0].Dtype).dtype) {
                                                                sprintf(msg, ": Type mismatch between identifier and expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char msg[100];
                                                                sprintf(msg, "%s = %s\n", (yyvsp[-2].string), (yyvsp[0].Dtype).place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,(yyvsp[-2].string));
                                                                interm_label++;
                                                            }
                                                        }
#line 2387 "y.tab.c"
    break;

  case 73:
#line 753 "inter_parser.y"
                                                                {	
                                                            int dec = checkIfDeclared((yyvsp[-4].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[-4].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[-4].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-4].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                            if(dtype != (yyvsp[-2].Dtype).dtype) {
                                                                sprintf(msg, ": Type mismatch between identifier and expression");
                                                                yyerror(msg);
                                                            } else {
                                                                char msg[100];
                                                                sprintf(msg, "%s = %s\n", (yyvsp[-4].string), (yyvsp[-2].Dtype).place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,(yyvsp[-4].string));
                                                                interm_label++;
                                                            }
                                                        }
#line 2421 "y.tab.c"
    break;

  case 74:
#line 782 "inter_parser.y"
                                                                    {	
                                                            int dec = checkIfDeclared((yyvsp[-3].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[-3].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[-3].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-3].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2445 "y.tab.c"
    break;

  case 75:
#line 801 "inter_parser.y"
                                                                                            {	
                                                            int dec = checkIfDeclared((yyvsp[-1].string));
                                                            char msg[100];
                                                            if(dec == 1) {
                                                                sprintf(msg, ": Multiple declarations of identifier %s",(yyvsp[-1].string));
                                                                yyerror(msg);
                                                            }
                                                            else if(dec == 2) {
                                                                sprintf(msg, ": Conflicting types for identifier %s", (yyvsp[-1].string));
                                                                yyerror(msg);
                                                            }
                                                            else {
                                                                int c = insert((yyvsp[-1].string));
                                                                if(c == -1) {
                                                                    sprintf(msg, ": Symbol table full");
                                                                    yyerror(msg);
                                                                }
                                                            }
                                                        }
#line 2469 "y.tab.c"
    break;

  case 76:
#line 822 "inter_parser.y"
                                                                {	
                                                            char msg[100];
                                                            if(!checkIfPresent((yyvsp[-2].string))) {
                                                                sprintf(msg, ": Undefined identifier %s",(yyvsp[-2].string));
                                                                yyerror(msg);
                                                            } else {
                                                                if(getType((yyvsp[-2].string)) != (yyvsp[0].Dtype).dtype) {
                                                                    sprintf(msg, ": Type mismatch between identifier and expression");
                                                                    yyerror(msg);
                                                                } else {
                                                                    char msg[100];
                                                                    sprintf(msg, "%s = %s\n", (yyvsp[-2].string), (yyvsp[0].Dtype).place);
                                                                    generate(interm_label,msg);
                                                                    strcpy(expr_var,(yyvsp[-2].string));
                                                                    interm_label++;
                                                                }
                                                            }
                                                            
                                                        }
#line 2493 "y.tab.c"
    break;

  case 77:
#line 843 "inter_parser.y"
                                                        {printf("PRINT STMT: FINISHED \tline no: %d\n", yylineno);}
#line 2499 "y.tab.c"
    break;

  case 79:
#line 849 "inter_parser.y"
                                                        {temp_label = interm_label;}
#line 2505 "y.tab.c"
    break;

  case 80:
#line 850 "inter_parser.y"
                                                        {
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
#line 2521 "y.tab.c"
    break;

  case 83:
#line 867 "inter_parser.y"
                                                        {
                                                            char msg[50];
                                                            sprintf(msg, "IF %s GOTO %d\n", tmp_var, interm_label+1);
                                                            generate(backpatch_label,msg);
                                                            backpatch_label = interm_label;
                                                            interm_label++;
                                                        }
#line 2533 "y.tab.c"
    break;

  case 84:
#line 874 "inter_parser.y"
                                                        {
                                                            char msg[50];
                                                            sprintf(msg, "GOTO %d\n", interm_label);
                                                            generate(backpatch_label,msg);
                                                        }
#line 2543 "y.tab.c"
    break;

  case 88:
#line 884 "inter_parser.y"
                                                                    {
                                                            char msg[50];
                                                            sprintf(msg,"IF %s GOTO %d\n",tmp_var,interm_label);
                                                            generate(backpatch_label,msg);
                                                        }
#line 2553 "y.tab.c"
    break;

  case 89:
#line 891 "inter_parser.y"
                                                        {	
                                                            if((yyvsp[-2].Dtype).dtype != (yyvsp[0].Dtype).dtype) {
																char msg[100];
																sprintf(msg, ": Type mismatch of operands");
																yyerror(msg);
															} else {
                                                                char *tmp = new_temp_var();
                                                                new_tmp_index++;
                                                                (yyval.Dtype).place = strdup(tmp);
                                                                char msg[50];
                                                                sprintf(msg, "%s = %s %s %s\n", (yyval.Dtype).place,(yyvsp[-2].Dtype).place,(yyvsp[-1].string),(yyvsp[0].Dtype).place);
                                                                generate(interm_label,msg);
                                                                strcpy(expr_var,(yyval.Dtype).place);
                                                                interm_label++;
                                                            }
                                                            (yyval.Dtype).dtype = (yyvsp[-2].Dtype).dtype;
														}
#line 2575 "y.tab.c"
    break;

  case 90:
#line 908 "inter_parser.y"
                                                                {
                                                            (yyval.Dtype).dtype = (yyvsp[0].Dtype).dtype;
                                                            (yyval.Dtype).place = strdup((yyvsp[0].Dtype).place);
                                                        }
#line 2584 "y.tab.c"
    break;

  case 91:
#line 914 "inter_parser.y"
                                                        {(yyval.string) = strdup((yyvsp[0].string));}
#line 2590 "y.tab.c"
    break;

  case 92:
#line 915 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2596 "y.tab.c"
    break;

  case 93:
#line 916 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2602 "y.tab.c"
    break;

  case 94:
#line 917 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2608 "y.tab.c"
    break;

  case 95:
#line 918 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2614 "y.tab.c"
    break;

  case 96:
#line 919 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2620 "y.tab.c"
    break;

  case 97:
#line 920 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2626 "y.tab.c"
    break;

  case 98:
#line 921 "inter_parser.y"
                                                                {(yyval.string) = strdup((yyvsp[0].string));}
#line 2632 "y.tab.c"
    break;

  case 99:
#line 924 "inter_parser.y"
                                                        {
                                                            char msg[50];
                                                            sprintf(msg, "GOTO %d\n", temp_label);
                                                            generate(interm_label,msg);
                                                            interm_label++;

                                                            sprintf(msg, "IF %s GOTO %d\n", tmp_var,interm_label);
                                                            generate(backpatch_label,msg);   
                                                        }
#line 2646 "y.tab.c"
    break;

  case 104:
#line 939 "inter_parser.y"
                                                        {temp_label = interm_label;}
#line 2652 "y.tab.c"
    break;

  case 105:
#line 940 "inter_parser.y"
                                                        {
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
#line 2668 "y.tab.c"
    break;

  case 117:
#line 971 "inter_parser.y"
                                                                                        {	if(!checkIfPresent((yyvsp[-3].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2679 "y.tab.c"
    break;

  case 118:
#line 977 "inter_parser.y"
                                                                                        {	if(!checkIfPresent((yyvsp[-3].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2690 "y.tab.c"
    break;

  case 119:
#line 983 "inter_parser.y"
                                                                                                        {	if(!checkIfPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2701 "y.tab.c"
    break;

  case 120:
#line 989 "inter_parser.y"
                                                                                                        {	if(!checkIfPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2712 "y.tab.c"
    break;

  case 121:
#line 997 "inter_parser.y"
                                                                                {	
                                                                printf("%s CALL: FOUND \tline no:\t%d\n",(yyvsp[-1].string), yylineno);
																if(!checkIfPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
																else {
																	(yyval.Dtype).dtype = getType((yyvsp[-1].string));
                                                                    (yyval.Dtype).place = strdup((yyvsp[-1].string));
																}
															}
#line 2729 "y.tab.c"
    break;


#line 2733 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1011 "inter_parser.y"


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

