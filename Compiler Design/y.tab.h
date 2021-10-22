/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    LSQUARE_TOK = 275,
    RSQUARE_TOK = 276,
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
#define LSQUARE_TOK 275
#define RSQUARE_TOK 276
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
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
