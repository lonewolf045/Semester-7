#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
#define TINT_TOK 400
#define TCHAR_TOK 401
#define TFLOAT_TOK 402
#define TDOUBLE_TOK 403
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

FILE *yyin, *yyout;
char *yytext;
int yylen = 0,state,ftok;

int validId(char *buff, int l) {
	for(int i=0;i<l;i++) {
		if(!isalnum(buff[i]) && buff[i] != '_')
			return 0;
	}
	return 1;
}

int isHeader(char *buff, int l) {
	if(buff[l-1] == 'h' && buff[l-2] == '.')
		return 1;
	return 0;
}

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
	else if(!strcmp("int", buff)) return TINT_TOK;
	else if(!strcmp("char", buff)) return TCHAR_TOK;
	else if(!strcmp("float", buff)) return TFLOAT_TOK;
	else if(!strcmp("double", buff)) return TDOUBLE_TOK;
	else return -1;
}

void getState(char c) {

	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
		state = 5;
		return;
	}
	else if((c >= '0' && c <= '9') || c == '.') {
		state = 4;
		return;
	}
	else if(c == '#') {
		state = 7;
		printf("# %d\n", HASH_TOK);
		ftok = HASH_TOK;
		return;
	}
	else if (c == '(') {
		state = 7;
		printf("( %d\n", LPAREN_TOK);
		ftok = LPAREN_TOK;
		return;
	} 
	else if (c == ')') {
        state = 7;
		printf(") %d\n", RPAREN_TOK);
		ftok = RPAREN_TOK;
		return;
    } 
	else if (c == '=' || c == '>' || c == '<' || c == '!') {
		state = 1;
		return;
        }
	else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
		state = 3;
        return;
	}
	else if(c == '\\') {
		state = 7;
		printf("\\ %d\n", BACKSLASH_TOK);
		ftok = BACKSLASH_TOK;
		return;
	}
	else if (c == ';') {
		state = 7;
		printf("; %d\n", SEMICOLON_TOK);
        ftok = SEMICOLON_TOK;
		return;
    }
	else if (c == ',') {
		state = 7;
		printf(", %d\n", COMMA_TOK);
        ftok = COMMA_TOK;
		return;
    }
	else if(c == '{') {
		state = 7;
		printf("{ %d\n", LCURL_TOK);
		ftok = LCURL_TOK;
		return;
	}
	else if (c == '}') {
		state = 7;
		printf("} %d\n", RCURL_TOK);
        ftok = RCURL_TOK;
		return;
    }
	else if(c == '[') {
		state = 7;
		printf("[ %d\n", LSQUARE_TOK);
		ftok = LSQUARE_TOK;
		return;
	}
	else if (c == ']') {
		state = 7;
		printf("] %d\n", RSQUARE_TOK);
        ftok = RSQUARE_TOK;
		return;
    } 
	else if (c == '"') {
		state = 9;
		//printf("\" %d\n", SPEECH_TOK);
        //ftok = SPEECH_TOK;
		return;
    } 
	else if (c == '\'') {
		state = 8;
		//printf("\' %d\n", INVCOMMA_TOK);
        //ftok = INVCOMMA_TOK;
		return;
    }
	else if (c == '&' || c == '|' || c == '^' || c == '~') {
		state = 2;
		return;
    }
	else if(c == ' ' || c == '\n' || c == '\t') {
		state = 6;
		return;
	}
	else {
		state = 0;
		return;
	}
}

int yylex () {
    char c;
	char buff[100] = "";
	int i = 0,token;
	if(state == -1) {
		c = fgetc(yyin);
		getState(c);
        //printf("%c",c);
		if(state == 7) {
			state = -1;
			return ftok;
		}
		if(state == 0) {
            printf("ERR_TOK\n");
			state = -1;
			return -1;
		}
	}
    while(1) {
        //printf("\n");
        switch(state) {
            case 0:
                printf("ERR_TOK\n");
                getState(c);
                return -1;
                break;
            case 1:
                if(c == '=') state = 10;
                else if(c == '<') state = 11;
                else if(c == '>') state = 12;
                else if(c == '!') state = 13;
                break;
            case 2:
                if(c == '&') state = 20;
                else if(c == '|') state = 21;
                else if(c == '^') state = 22;
                else if(c == '~') state = 23;
                break;
            case 3:
                if(c == '+') state = 30;
                else if(c == '-') state = 31;
                else if(c == '*') state = 32;
                else if(c == '%') state = 33;
                else if(c == '/') state = 34;
                break;
            case 4:
                if(c >= '0' && c <= '9') state = 41;
                else if(c == '.') state = 42;
                break;
            case 5:
                i = 0;
                strcpy(buff,"");
                state = 51;
                break;
            case 6:
                state = 61;
                break;
            case 7:
                //printf("%c %d\n",c,ftok);
                state = -1;
                return ftok;
            case 8:
                state = 81;
                break;
            case 9:
                state = 91;
                break;
            case 10:
                c = fgetc(yyin);
				if(c == '=') state = 101;
				else state = 102;
				break;
            case 11:
                c = fgetc(yyin);
				if(c == '<') state = 111;
                else if(c == '=') state = 112;
				else state = 113;
				break;
            case 12:
                c = fgetc(yyin);
				if(c == '>') state = 121;
                else if(c == '=') state = 122;
				else state = 123;
				break;
            case 13:
                c = fgetc(yyin);
				if(c == '=') state = 131;
				else state = 132;
				break;
            case 20:
                c = fgetc(yyin);
				if(c == '&') state = 201;
				else state = 202;
				break;
            case 21:
                c = fgetc(yyin);
				if(c == '|') state = 211;
				else state = 212;
				break;
            case 22:
                printf("^ %d\n",XOR_TOK);
                state = -1;
                return XOR_TOK;
            case 23:
                printf("~ %d\n",NEG_TOK);
                state = -1;
                return NEG_TOK;
            case 30:
                c = fgetc(yyin);
				if(c == '+') state = 301;
                else if(c == '=') state = 302;
				else state = 303;
				break;
            case 31:
                c = fgetc(yyin);
				if(c == '-') state = 311;
                else if(c == '=') state = 312;
				else state = 313;
				break;
            case 32:
                c = fgetc(yyin);
				if(c == '=') state = 321;
				else state = 323;
				break;
            case 33:
                c = fgetc(yyin);
				if(c == '=') state = 331;
				else state = 332;
				break;
            case 34:
                c = fgetc(yyin);
				if(c == '=') state = 341;
                else if(c == '/') state = 342;
                else if(c == '*') state = 343;
				else state = 344;
				break;

            case 41:
                c = fgetc(yyin);
                if(c >= '0' && c <= '9') state = 41;
                else if( c == '.') state = 42;
                else if( c == 'E' || c == 'e') state = 411;
                else state = 410;
                break;

            case 42:
                c = fgetc(yyin);
                if(c == '.') state = 0;
                else if(c >= '0' && c <= '9') state = 421;
                else state = 420;
                break;

            case 51:
                buff[i++] = c;
                c = fgetc(yyin);
                if(isalnum(c) || c == '_') state = 51;
                else state = 510;
                break;

            case 61:
                c = fgetc(yyin);
                if(c == ' ' || c == '\n' || c == '\t') state = 61;
                else state = 610;
                break;

            case 81:
                c = fgetc(yyin);
                if(c == '\'') state = 810;
                else if(c == '\\') state = 812;
                else state = 811;
                break;

            case 91:
                c = fgetc(yyin);
                if(c == '"') state = 910;
                else if(c == '\\') state = 911;
                else state = 91;
                break;

            case 101:
                printf("= %d\n",EQ_TOK);
                state = -1;
                return EQ_TOK;
            case 102:
                printf("= %d\n",ASSIGN_TOK);
                getState(c);
                return ASSIGN_TOK;
            case 111:
                printf("<< %d\n",LSHIFT_TOK);
                state = -1;
                return LSHIFT_TOK;
            case 112:
                printf("<= %d\n",LTE_TOK);
                state = -1;
                return LTE_TOK;
            case 113:
                printf("< %d\n",LT_TOK);
                getState(c);
                return LT_TOK;
            case 121:
                printf(">> %d\n",RSHIFT_TOK);
                state = -1;
                return RSHIFT_TOK;
            case 122:
                printf(">= %d\n",GTE_TOK);
                state = -1;
                return GTE_TOK;
            case 123:
                printf("> %d\n",GT_TOK);
                getState(c);
                return GT_TOK;
            case 131:
                printf("!= %d\n",NEQ_TOK);
                state = -1;
                return NEQ_TOK;
            case 132:
                printf("! %d\n",NOT_TOK);
                getState(c);
                return NOT_TOK;
            case 201:
                printf("&& %d\n",AND_TOK);
                state = -1;
                return AND_TOK;
            case 202:
                printf("& %d\n",BIT_AND_TOK);
                getState(c);
                return BIT_AND_TOK;
            case 211:
                printf("|| %d\n",OR_TOK);
                state = -1;
                return OR_TOK;
            case 212:
                printf("| %d\n",BIT_OR_TOK);
                getState(c);
                return BIT_OR_TOK;
            case 301:
                printf("++ %d\n",INC_TOK);
                state = -1;
                return INC_TOK;
            case 302:
                printf("+= %d\n",PLUS_ASSIGN_TOK);
                state = -1;
                return PLUS_ASSIGN_TOK;
            case 303:
                printf("+ %d\n",PLUS_TOK);
                getState(c);
                return PLUS_TOK;
            case 311:
                printf("-- %d\n",DEC_TOK);
                state = -1;
                return DEC_TOK;
            case 312:
                printf("-= %d\n",MINUS_ASSIGN_TOK);
                state = -1;
                return MINUS_ASSIGN_TOK;
            case 313:
                printf("- %d\n",MINUS_TOK);
                getState(c);
                return MINUS_TOK;
            case 321:
                printf("*= %d\n",MULT_ASSIGN_TOK);
                state = -1;
                return MULT_ASSIGN_TOK;
            case 322:
                printf("* %d\n",STAR_TOK);
                getState(c);
                return STAR_TOK;
            case 331:
                printf("%%= %d\n",MOD_ASSIGN_TOK);
                state = -1;
                return MOD_ASSIGN_TOK;
            case 332:
                printf("%% %d\n",MOD_TOK);
                getState(c);
                return MOD_TOK;
            case 341:
                printf("/= %d\n",DIV_ASSIGN_TOK);            
                state = -1;
                return DIV_ASSIGN_TOK;
            case 342:
                c = fgetc(yyin);
                if(c == '\n') {
                    state = 3421;
                } else {
                    state = 342;
                }
                break;
            case 343:
                if(feof(yyin)) {
					printf("Error: Unclosed multiline comment\n");
					return -2;
				}
                c = fgetc(yyin);
                if(c == '*') {
                    state = 3431;
                } else {
                    state = 343;
                }
                break;
            case 344:
                printf("/ %d\n",SLASH_TOK);
                getState(c);
                return SLASH_TOK;

            case 410:
                printf("INTCONST %d\n",INTCONST);
                getState(c);
                return INTCONST;

            case 411:
                c = fgetc(yyin);
                if(c == '+' || c == '-') state = 4111;
                else if(c >= '0' && c <= '9') state = 4112;
                else state = 0;
                break;
            case 420:
                printf(". %d\n",DOT_TOK);
                getState(c);
                return DOT_TOK;

            case 421:
                c = fgetc(yyin);
                if(c >= '0' && c <= '9') state = 421;
                else if(c == 'E' || c == 'e') state = 411;
                else state = 4210;
                break;
            
            case 510:
                //state = -1;
                token = validKwrd(buff);
                if(token != -1) {
                    printf("%s %d\n",buff,token);
                }
                else {
                    printf("%s %d\n",buff,ID_TOK);
                    token = ID_TOK;
                }
                getState(c);
                return token;
            case 610:
                getState(c);
                break;

            case 810:
                printf("CHARCONST %d\n",CHARCONST);
                state = -1;
                return CHARCONST;
            
            case 811:
                c = fgetc(yyin);
                if(c == '\'') state = 810;
                else state = 0;
                break;
            case 812:
                c = fgetc(yyin);
                state = 811;
                break;
            case 910:
                printf("STRCONST %d\n",STRCONST);
                state = -1;
                return STRCONST;
            case 911:
                c = fgetc(yyin);
                state = 91;
                break;
            case 3421:
                c = fgetc(yyin);
                getState(c);
                break;

            case 3431:
                if(feof(yyin)) {
					printf("Error: Unclosed multiline comment\n");
					return -2;
				}
                c = fgetc(yyin);
                if(c == '*') state = 3431;
                else if(c == '/') state = 3432;
                else state = 343;
                break;

            case 3432:
                if(feof(yyin))
					return -2;
				c = fgetc(yyin);
				getState(c);
				break;
            
            case 4111:
                c = fgetc(yyin);
                if(c >= '0' && c <= '9') state = 4112;
                else state = 0;
                break;

            case 4112:
                c = fgetc(yyin);
                if(c >= '0' && c <= '9') state = 4112;
                else state = 4210;
                break;

            case 4210:
                printf("REALCONST %d\n",REALCONST);
                getState(c);
                return REALCONST;
        }
    }
}
int main(int argc, char *argv[]) {
	int token;
	if (argc != 3){
		printf("Insufficient input as command line arguments. Aborting!\n");
		return 0;
	}
	yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2],"w");
	state = -1;
	while(!feof(yyin)) {
		token = yylex();
		if(token >= 0) 
			fprintf(yyout,"%d\n", token);
		else if(token < 0 && token != -2 && !feof(yyin))
			fprintf(yyin,"ERR_TOK\n");
	}
	fclose(yyin);
	return 0;
}