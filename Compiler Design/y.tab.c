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
#line 1 "yacc2.y"

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


#line 331 "y.tab.c"

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
#line 263 "yacc2.y"

	int number;		//for integer constants
	float decimal;	//for real constants
	char *string;	//for identifier names
	int dtype;		//for datatype of expressions

#line 530 "y.tab.c"

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
#define YYLAST   287

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

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
       0,   357,   357,   358,   359,   360,   361,   364,   365,   366,
     367,   368,   369,   372,   375,   378,   379,   382,   385,   395,
     396,   399,   407,   417,   418,   419,   420,   421,   424,   427,
     430,   431,   432,   435,   436,   437,   440,   441,   444,   445,
     446,   447,   448,   449,   450,   454,   455,   458,   459,   462,
     472,   482,   483,   484,   485,   486,   487,   488,   498,   499,
     500,   501,   502,   510,   513,   520,   521,   527,   528,   535,
     553,   571,   593,   615,   633,   653,   666,   669,   670,   671,
     672,   677,   678,   681,   684,   685,   688,   689,   692,   698,
     701,   702,   703,   704,   705,   706,   707,   708,   711,   712,
     713,   714,   715,   718,   721,   724,   725,   726,   727,   728,
     729,   730,   733,   737,   738,   739,   745,   751,   757,   765
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
  "HEADER_INC", "MAIN", "USER_FN", "PARAMETER_LIST", "PARAMETER",
  "datatype", "START_BLK", "END_BLK", "BLOCK", "ENCLOSED", "STATEMENTS",
  "stmt", "ARGUMENT_LIST", "ARGUMENT", "EXPRESSION", "OPERATOR_ARITH",
  "DECL_STMT", "ARRAY_ELE", "ARRAY_SIZE", "VARLIST", "ASSIGN_STMT",
  "PRINT_STMT", "CONDITIONAL_STMT", "IF_BLOCK", "IF_STMT", "ELSE_IF_BLOCK",
  "ELSE_BLOCK", "CONDITION", "OPERATOR_REL", "ITERATIVE_STMT",
  "WHILE_STMT", "FOR_STMT", "FOR_CONTENTS", "init", "update",
  "FUNC_CALL_STMT", YY_NULLPTR
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

#define YYPACT_NINF (-127)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-68)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      55,   -40,   -24,  -127,  -127,  -127,  -127,    47,    55,  -127,
      55,   -20,    50,    14,    58,  -127,  -127,  -127,  -127,  -127,
      55,    67,   142,  -127,    55,    11,  -127,    27,    69,  -127,
      55,    78,     4,    53,    -4,   203,    69,   103,  -127,  -127,
     116,     4,  -127,    19,  -127,  -127,  -127,  -127,   138,   234,
    -127,  -127,   108,  -127,  -127,  -127,   244,    66,  -127,  -127,
     129,   130,   135,  -127,   155,   102,  -127,    53,   111,   204,
      73,  -127,  -127,  -127,  -127,  -127,  -127,  -127,  -127,     4,
       4,  -127,    69,   170,   170,   170,     4,    58,   140,    53,
    -127,   159,   175,   108,   159,   172,   181,  -127,  -127,   153,
     146,  -127,   165,   166,   216,    53,  -127,   217,  -127,  -127,
    -127,   185,  -127,   232,  -127,   226,  -127,   266,   205,  -127,
       4,    49,     4,   254,   224,     4,   121,  -127,  -127,  -127,
     121,  -127,  -127,  -127,    81,   200,  -127,  -127,  -127,  -127,
    -127,  -127,  -127,  -127,  -127,   203,  -127,  -127,   235,   188,
     155,    41,  -127,   155,   243,   155,  -127,  -127,   266,  -127,
    -127,  -127,   167,   193,  -127,  -127,  -127,  -127,  -127,  -127,
    -127,  -127,  -127,  -127,  -127,     4,  -127,   197,   245,  -127,
      48,  -127,  -127,  -127,   266,    93,   250,  -127,   197,  -127,
     246,   262,   263,   197,  -127,   197,   197,   197,  -127,  -127,
    -127,  -127
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    23,    26,    24,    25,    27,     0,     3,     4,
       0,     0,     0,     0,     0,     1,     2,     5,     9,    11,
      10,     0,    69,    63,     0,     0,    16,     0,     0,     7,
      12,    13,     0,     0,     0,     0,     0,     0,    74,     6,
       0,     0,    46,    49,    51,    53,    52,    54,     0,    47,
      55,    28,     0,    17,     8,    20,    71,    69,    70,    66,
       0,     0,     0,    23,     0,     0,    18,     0,     0,     0,
       0,   119,    50,    45,    59,    58,    60,    61,    62,     0,
       0,    29,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     0,     0,     0,    37,     0,     0,    44,    40,    80,
       0,    41,     0,     0,     0,     0,    64,    65,    68,    14,
      19,    22,    73,     0,    56,     0,    48,    57,     0,    13,
       0,     0,     0,     0,     0,     0,     0,    30,    31,    34,
      35,    36,    38,    39,     0,    78,    79,    82,    81,    99,
      98,   101,   100,    42,    72,     0,    15,    65,     0,    89,
       0,     0,   112,     0,     0,     0,    43,    76,    75,    33,
      87,    86,     0,     0,    77,    21,   102,    90,    91,    95,
      92,    93,    96,    97,    94,     0,   103,   111,     0,   104,
       0,    83,    85,    84,    88,     0,   114,   110,   109,   107,
       0,   117,   118,     0,   108,   106,     0,     0,   113,   105,
     115,   116
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -127,   273,    77,   150,  -126,    15,    26,    40,  -127,   137,
       1,  -127,   -43,   -26,   -46,     2,  -127,    -2,   206,   -27,
    -127,     8,   240,  -127,   -30,    44,  -127,  -127,  -127,   152,
    -127,   149,  -109,  -127,  -127,   169,  -127,  -127,  -127,   -55,
     -48
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,    17,    35,   110,    18,    19,    20,    36,    64,
      89,    52,    90,    91,   129,   130,    94,    71,    48,   149,
      80,    95,    37,    38,    23,    96,    97,    98,    99,   100,
     135,   136,   150,   175,   101,   102,   103,   153,   154,   187,
      50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    11,    53,    58,   104,    56,    92,    41,    12,    11,
      66,    11,    28,   155,    69,     8,    12,    59,    21,    25,
      13,    11,    27,     8,   176,    11,     9,   179,    21,   181,
      41,    11,    21,    42,     9,    14,    65,   112,    21,    70,
      10,    22,   178,   104,    41,   104,   104,    15,    10,   127,
     128,    41,    49,   117,    93,   177,   118,    60,    61,   123,
      62,    27,   189,   151,    24,    43,    44,    45,    46,    47,
       1,   190,    40,    32,   138,   144,   140,   142,   104,    26,
     159,    30,   104,    33,    55,   124,    34,    51,    43,    44,
      45,    46,    47,   126,    59,   160,   131,    29,   158,    51,
     125,    39,    43,    44,    45,    46,    47,    54,   161,    43,
      44,    45,    46,    47,    57,    63,     3,     4,     5,     6,
      67,     2,     3,     4,     5,     6,    51,    81,    85,   152,
     191,   192,    68,   194,    60,   115,   183,   161,   198,    51,
     199,   200,   201,    27,    73,    31,    65,   125,   184,    32,
     106,   107,    82,    83,    84,    85,   108,    86,    87,    33,
     137,   109,    34,   111,    51,    82,    83,    84,    85,    88,
      86,    87,   113,   119,    63,     3,     4,     5,     6,   139,
     141,   182,    88,    51,    51,    51,   132,    63,     3,     4,
       5,     6,   167,   168,    81,   133,    74,    75,    76,    77,
      78,   134,   145,    82,    83,    84,    85,   160,    86,    87,
     114,    51,    74,    75,    76,    77,    78,   169,   170,   171,
      88,   186,   172,   173,   174,    63,     3,     4,     5,     6,
     143,   -67,   186,   120,   121,   122,   146,   186,   157,   186,
     186,   186,    74,    75,    76,    77,    78,   147,   163,   166,
      83,    79,    74,    75,    76,    77,    78,   180,   185,   188,
     195,   105,    74,    75,    76,    77,    78,   193,   156,    63,
       3,     4,     5,     6,    74,    75,    76,    77,    78,   196,
     197,    16,   165,    72,   164,   116,   162,   148
};

static const yytype_uint8 yycheck[] =
{
      27,     0,    28,    33,    52,    32,    52,     3,     0,     8,
      36,    10,    14,   122,    41,     0,     8,    21,    10,     5,
      60,    20,     3,     8,   150,    24,     0,   153,    20,   155,
       3,    30,    24,     6,     8,    59,    35,    67,    30,    20,
       0,    61,   151,    91,     3,    93,    94,     0,     8,    92,
      93,     3,    79,    80,    52,    14,    82,    61,    62,    86,
      64,     3,    14,    14,    14,    61,    62,    63,    64,    65,
      15,   180,    61,     7,   100,   105,   102,   103,   126,    65,
     126,    14,   130,    17,     6,    87,    20,    18,    61,    62,
      63,    64,    65,    91,    21,    14,    94,    20,   125,    18,
       7,    24,    61,    62,    63,    64,    65,    30,   134,    61,
      62,    63,    64,    65,    61,    66,    67,    68,    69,    70,
      17,    66,    67,    68,    69,    70,    18,    19,    47,   121,
      37,    38,    16,   188,    61,    62,   162,   163,   193,    18,
     195,   196,   197,     3,     6,     3,   145,     7,   175,     7,
      21,    21,    44,    45,    46,    47,    21,    49,    50,    17,
      14,     6,    20,    61,    18,    44,    45,    46,    47,    61,
      49,    50,    61,     3,    66,    67,    68,    69,    70,    14,
      14,    14,    61,    18,    18,    18,    14,    66,    67,    68,
      69,    70,     4,     5,    19,    14,     8,     9,    10,    11,
      12,    48,    17,    44,    45,    46,    47,    14,    49,    50,
       6,    18,     8,     9,    10,    11,    12,    29,    30,    31,
      61,   177,    34,    35,    36,    66,    67,    68,    69,    70,
      14,    14,   188,    83,    84,    85,     4,   193,    14,   195,
     196,   197,     8,     9,    10,    11,    12,    21,    48,    14,
      45,    17,     8,     9,    10,    11,    12,    14,    61,    14,
      14,    17,     8,     9,    10,    11,    12,    17,    14,    66,
      67,    68,    69,    70,     8,     9,    10,    11,    12,    17,
      17,     8,   145,    43,   135,    79,   134,   118
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    15,    66,    67,    68,    69,    70,    72,    76,    77,
      78,    81,    92,    60,    59,     0,    72,    73,    76,    77,
      78,    92,    61,    95,    14,     5,    65,     3,    88,    73,
      14,     3,     7,    17,    20,    74,    79,    93,    94,    73,
      61,     3,     6,    61,    62,    63,    64,    65,    89,    90,
     111,    18,    82,    84,    73,     6,    90,    61,    95,    21,
      61,    62,    64,    66,    80,    81,    84,    17,    16,    90,
      20,    88,    93,     6,     8,     9,    10,    11,    12,    17,
      91,    19,    44,    45,    46,    47,    49,    50,    61,    81,
      83,    84,    85,    86,    87,    92,    96,    97,    98,    99,
     100,   105,   106,   107,   111,    17,    21,    21,    21,     6,
      75,    61,    95,    61,     6,    62,    89,    90,    84,     3,
      74,    74,    74,    90,    88,     7,    86,    83,    83,    85,
      86,    86,    14,    14,    48,   101,   102,    14,    84,    14,
      84,    14,    84,    14,    95,    17,     4,    21,   106,    90,
     103,    14,    92,   108,   109,   103,    14,    14,    90,    85,
      14,    84,   100,    48,   102,    80,    14,     4,     5,    29,
      30,    31,    34,    35,    36,   104,    75,    14,   103,    75,
      14,    75,    14,    84,    90,    61,    96,   110,    14,    14,
     103,    37,    38,    17,   110,    14,    17,    17,   110,   110,
     110,   110
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    71,    72,    72,    72,    72,    72,    73,    73,    73,
      73,    73,    73,    74,    75,    76,    76,    77,    78,    79,
      79,    80,    80,    81,    81,    81,    81,    81,    82,    83,
      84,    84,    84,    85,    85,    85,    86,    86,    87,    87,
      87,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    91,    91,
      91,    91,    91,    92,    93,    93,    93,    94,    94,    95,
      95,    95,    95,    95,    95,    96,    97,    98,    98,    98,
      98,    99,    99,   100,   101,   101,   102,   102,   103,   103,
     104,   104,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   106,   107,   108,   108,   108,   108,   108,
     108,   108,   109,   110,   110,   110,   110,   110,   110,   111
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     3,     2,     3,     1,
       1,     1,     2,     1,     1,     7,     3,     4,     4,     3,
       2,     4,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     3,     2,     1,     2,     1,     2,     2,
       1,     1,     2,     3,     1,     3,     2,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     3,     3,     1,     1,
       1,     1,     1,     2,     3,     3,     2,     3,     3,     1,
       3,     3,     5,     4,     2,     3,     3,     3,     2,     2,
       1,     2,     2,     4,     3,     3,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     4,     4,     4,     5,     4,     3,     4,     3,
       3,     2,     1,     3,     1,     4,     4,     2,     2,     2
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
#line 372 "yacc2.y"
                                        {scope++;}
#line 1883 "y.tab.c"
    break;

  case 14:
#line 375 "yacc2.y"
                                        {scope--;}
#line 1889 "y.tab.c"
    break;

  case 15:
#line 378 "yacc2.y"
                                                                                {printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
#line 1895 "y.tab.c"
    break;

  case 16:
#line 379 "yacc2.y"
                                                                                {printf("Header syntax correct;\tline no:\t%d\n", yylineno);}
#line 1901 "y.tab.c"
    break;

  case 17:
#line 382 "yacc2.y"
                                            {printf("Main function syntax OK;\tline no:\t%d\n", yylineno);}
#line 1907 "y.tab.c"
    break;

  case 18:
#line 385 "yacc2.y"
                                               {printf("User defined function OK;\tline no:\t%d\n", yylineno);
												int c = addFunction((yyvsp[-2].string));
												if(c == -1) {
													char msg[100];
													sprintf(msg, ": Symbol table full");
													yyerror(msg);
												}
												}
#line 1920 "y.tab.c"
    break;

  case 21:
#line 399 "yacc2.y"
                                                        {	
														int c = addSymbol((yyvsp[-2].string));
														if(c == -1) {
															char msg[100];
															sprintf(msg, ": Symbol table full");
															yyerror(msg);
														}
													}
#line 1933 "y.tab.c"
    break;

  case 22:
#line 407 "yacc2.y"
                                                                                {	
														int c = addSymbol((yyvsp[0].string));
														if(c == -1) {
															char msg[100];
															sprintf(msg, ": Symbol table full");
															yyerror(msg);
														}
													}
#line 1946 "y.tab.c"
    break;

  case 23:
#line 417 "yacc2.y"
                                                                                        {dtype = 1;}
#line 1952 "y.tab.c"
    break;

  case 24:
#line 418 "yacc2.y"
                                                                                                {dtype = 2;}
#line 1958 "y.tab.c"
    break;

  case 25:
#line 419 "yacc2.y"
                                                                                        {dtype = 2;}
#line 1964 "y.tab.c"
    break;

  case 26:
#line 420 "yacc2.y"
                                                                                                {dtype = 4;}
#line 1970 "y.tab.c"
    break;

  case 27:
#line 421 "yacc2.y"
                                                                                                {dtype = 5;}
#line 1976 "y.tab.c"
    break;

  case 28:
#line 424 "yacc2.y"
                                                                                {scope++;}
#line 1982 "y.tab.c"
    break;

  case 29:
#line 427 "yacc2.y"
                                                                                {deleteScopeSyms(scope); scope--;}
#line 1988 "y.tab.c"
    break;

  case 38:
#line 444 "yacc2.y"
                                                {printf("Declaration over;\tline no:\t%d\n", yylineno);}
#line 1994 "y.tab.c"
    break;

  case 39:
#line 445 "yacc2.y"
                                                {printf("Assignment over;\tline no:\t%d\n", yylineno);}
#line 2000 "y.tab.c"
    break;

  case 40:
#line 446 "yacc2.y"
                                                {printf("Conditional block;\tline no:\t%d\n", yylineno);}
#line 2006 "y.tab.c"
    break;

  case 41:
#line 447 "yacc2.y"
                                                {printf("Iterative block;\tline no:\t%d\n", yylineno);}
#line 2012 "y.tab.c"
    break;

  case 43:
#line 449 "yacc2.y"
                                                {printf("return statement found\tline no: %d\n", yylineno);}
#line 2018 "y.tab.c"
    break;

  case 49:
#line 462 "yacc2.y"
                                                                                {	
													if(!isPresent((yyvsp[0].string))) {
															char msg[100];
															sprintf(msg, ": Undefined identifier found");
															yyerror(msg);
														}
														else {
															(yyval.dtype) = getDatatype((yyvsp[0].string));
														}
												}
#line 2033 "y.tab.c"
    break;

  case 50:
#line 472 "yacc2.y"
                                                                                {	
													if(!isPresent((yyvsp[-1].string))) {
															char msg[100];
															sprintf(msg, ": Undefined identifier found");
															yyerror(msg);
														}
														else {
															(yyval.dtype) = getDatatype((yyvsp[-1].string));
														}
												}
#line 2048 "y.tab.c"
    break;

  case 51:
#line 482 "yacc2.y"
                                                                                        {(yyval.dtype) = 1;}
#line 2054 "y.tab.c"
    break;

  case 52:
#line 483 "yacc2.y"
                                                                                        {(yyval.dtype) = 2;}
#line 2060 "y.tab.c"
    break;

  case 53:
#line 484 "yacc2.y"
                                                                                        {(yyval.dtype) = 4;}
#line 2066 "y.tab.c"
    break;

  case 54:
#line 485 "yacc2.y"
                                                                                {(yyval.dtype) = 5;}
#line 2072 "y.tab.c"
    break;

  case 55:
#line 486 "yacc2.y"
                                                                                {(yyval.dtype) = (yyvsp[0].dtype);}
#line 2078 "y.tab.c"
    break;

  case 56:
#line 487 "yacc2.y"
                                                                {(yyval.dtype) = (yyvsp[-1].dtype);}
#line 2084 "y.tab.c"
    break;

  case 57:
#line 488 "yacc2.y"
                                                        {
													if((yyvsp[-2].dtype) != (yyvsp[0].dtype)) {
														char msg[100];
														sprintf(msg, ": Type matching error in expression");
														yyerror(msg);
													}	
													(yyval.dtype) = (yyvsp[-2].dtype);
												}
#line 2097 "y.tab.c"
    break;

  case 64:
#line 513 "yacc2.y"
                                                                {	
														if(!isPresent((yyvsp[-1].string))) {
															char msg[100];
															sprintf(msg, ": Undeclared identifier found");
															yyerror(msg);
														}
													}
#line 2109 "y.tab.c"
    break;

  case 66:
#line 521 "yacc2.y"
                                                                        {	
														char msg[100];
														sprintf(msg, ": Unspecified array element access");
														yyerror(msg);
													}
#line 2119 "y.tab.c"
    break;

  case 68:
#line 528 "yacc2.y"
                                                                {	
														char msg[100];
														sprintf(msg, ": Incorrect type for array size");
														yyerror(msg);
													}
#line 2129 "y.tab.c"
    break;

  case 69:
#line 535 "yacc2.y"
                                                                                        {	int c = alreadyDeclared((yyvsp[0].string));
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
															int x = addSymbol((yyvsp[0].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
													}
#line 2152 "y.tab.c"
    break;

  case 70:
#line 553 "yacc2.y"
                                                                        {	int c = alreadyDeclared((yyvsp[-2].string));
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
															int x = addSymbol((yyvsp[-2].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
													}
#line 2175 "y.tab.c"
    break;

  case 71:
#line 571 "yacc2.y"
                                                                        {	int c = alreadyDeclared((yyvsp[-2].string));
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
															int x = addSymbol((yyvsp[-2].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
														if(getDatatype((yyvsp[-2].string)) != (yyvsp[0].dtype)) {
															sprintf(msg, ": Type mismatch between identifier and expression");
															yyerror(msg);
														}
													}
#line 2202 "y.tab.c"
    break;

  case 72:
#line 593 "yacc2.y"
                                                                        {	int c = alreadyDeclared((yyvsp[-4].string));
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
															int x = addSymbol((yyvsp[-4].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
														if(dtype != (yyvsp[-2].dtype)) {
															sprintf(msg, ": Type mismatch between identifier and expression");
															yyerror(msg);
														}
													}
#line 2229 "y.tab.c"
    break;

  case 73:
#line 615 "yacc2.y"
                                                                {	int c = alreadyDeclared((yyvsp[-3].string));
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
															int x = addSymbol((yyvsp[-3].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
													}
#line 2252 "y.tab.c"
    break;

  case 74:
#line 633 "yacc2.y"
                                                                                        {	int c = alreadyDeclared((yyvsp[-1].string));
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
															int x = addSymbol((yyvsp[-1].string));
															if(x == -1) {
																sprintf(msg, ": Symbol table full");
																yyerror(msg);
															}
														}
													}
#line 2275 "y.tab.c"
    break;

  case 75:
#line 653 "yacc2.y"
                                                                {	if(!isPresent((yyvsp[-2].string))) {
															char msg[100];
															sprintf(msg, ": Undefined identifier found");
															yyerror(msg);
														}
														if(getDatatype((yyvsp[-2].string)) != (yyvsp[0].dtype)) {
															char msg[100];
															sprintf(msg, ": Type mismatch between identifier and expression");
															yyerror(msg);
														}
													}
#line 2291 "y.tab.c"
    break;

  case 76:
#line 666 "yacc2.y"
                                                        {printf("print statement found\tline no: %d\n", yylineno);}
#line 2297 "y.tab.c"
    break;

  case 88:
#line 692 "yacc2.y"
                                                                        {	if((yyvsp[-2].dtype) != (yyvsp[0].dtype)) {
																	char msg[100];
																	sprintf(msg, ": Type mismatch of operands");
																	yyerror(msg);
																}
															}
#line 2308 "y.tab.c"
    break;

  case 115:
#line 739 "yacc2.y"
                                                                                        {	if(!isPresent((yyvsp[-3].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2319 "y.tab.c"
    break;

  case 116:
#line 745 "yacc2.y"
                                                                                        {	if(!isPresent((yyvsp[-3].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2330 "y.tab.c"
    break;

  case 117:
#line 751 "yacc2.y"
                                                                                                        {	if(!isPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2341 "y.tab.c"
    break;

  case 118:
#line 757 "yacc2.y"
                                                                                                        {	if(!isPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
															}
#line 2352 "y.tab.c"
    break;

  case 119:
#line 765 "yacc2.y"
                                                                                {	printf("Function call statement found;\tline no:\t%d\n", yylineno);
																if(!isPresent((yyvsp[-1].string))) {
																	char msg[100];
																	sprintf(msg, ": Undeclared identifier found");
																	yyerror(msg);
																}
																else {
																	(yyval.dtype) = getDatatype((yyvsp[-1].string));
																}
															}
#line 2367 "y.tab.c"
    break;


#line 2371 "y.tab.c"

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
#line 777 "yacc2.y"


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

