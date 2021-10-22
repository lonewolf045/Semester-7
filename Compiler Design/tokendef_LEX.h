// Single character lexemes 
#define LPAREN_TOK '('
#define GT_TOK '>'
#define LT_TOK '<'
#define RPAREN_TOK ')'
#define ASSIGN_TOK '='
#define MINUS_TOK '-'
#define PLUS_TOK '+'
#define STAR_TOK '*'
#define SLASH_TOK '/'
#define MOD_TOK '%'
#define NOT_TOK '!'
#define SEMICOLON_TOK ';'
#define HASH_TOK '#'
#define DOT_TOK '.'
#define COMMA_TOK ','
#define LCURL_TOK '{'
#define RCURL_TOK '}'
#define LSQUARE_TOK '['
#define RSQUARE_TOK ']'
#define BIT_AND_TOK '&'
#define BIT_OR_TOK '|'
#define PERC_TOK '%'
#define BACKSLASH_TOK '\\'
#define SPEECH_TOK '"'
#define INVCOMMA_TOK '\''
#define XOR_TOK '^'
#define NEG_TOK '~'
//---------------------------

//Double character lexemes
#define NEQ_TOK 127
#define EQ_TOK 120
#define GTE_TOK 121
#define LTE_TOK 122
#define LSHIFT_TOK 123
#define RSHIFT_TOK 124
#define AND_TOK 128
#define OR_TOK 129
//---------------------------

// Reserved words 
#define DO_TOK 255
#define WHILE_TOK 256
#define FOR_TOK 257
#define IF_TOK 258
#define ELSE_TOK 259
#define RET_TOK 260
#define MAIN_TOK 261
#define INL_TOK 262
#define PRINT_TOK 263
#define SCAN_TOK 264
#define DEF_TOK 265
//---------------------------

// Identifiers, constants
#define ID_TOK 350
#define INTCONST 351
#define REALCONST 352
#define CHARCONST 353
#define STRCONST 354
//---------------------------


// Type specifiers
#define INT_TOK 400
#define CHAR_TOK 401
#define FLOAT_TOK 402
#define DOUBLE_TOK 403
//---------------------------

// Format specifiers
#define FINT_TOK 410
#define FCHAR_TOK 411
#define FFLOAT_TOK 412
#define FDOUBLE_TOK 413
//---------------------------

//Arithmetic assignment operators
#define PLUS_ASSIGN_TOK 701
#define MINUS_ASSIGN_TOK 702
#define MULT_ASSIGN_TOK 703
#define DIV_ASSIGN_TOK 704
#define MOD_ASSIGN_TOK 705
//---------------------------

#define INC_TOK 801
#define DEC_TOK 802

//For Header Files
#define HEADER_TOK 669
