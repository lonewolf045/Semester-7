#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Single caharacter lexemes 
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
#define AND_TOK '&'
#define OR_TOK '|'
#define PERC_TOK '%'
#define BACKSLASH_TOK '\\'
#define SPEECH_TOK '"'
#define INVCOMMA_TOK '\''
//---------------------------

//Double character lexemes
#define EQ_TOK 120
#define GTE_TOK 121
#define LTE_TOK 122
#define LSHIFT_TOK 123
#define RSHIFT_TOK 124
#define BIT_AND_TOK 121
#define BIT_OR_TOK 122
//---------------------------

// Reserved words 
#define WHILE_TOK 256
#define FOR_TOK 257
#define IF_TOK 258
#define ELSE_TOK 259
#define RET_TOK 260
#define MAIN_TOK 261
#define INC_TOK 262
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


//For Header Files
#define HEADER_TOK 669

FILE *yyin, *yyout;
char *yytext;
int yylen = 0;
char prevchar = '\0';

/* will check if the given token is a valid identifier */

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

int yylex() {
	int count = 0, i;
	char c, temp;
	FILE *ftemp, *ftemp2;
	ftemp2 = yyin;
	if(prevchar == '\0')
		c = fgetc(yyin);
	else
		c = prevchar;
	
	//jumping whitespace

	if(c == ' ' || c == '\n' || c == '\t') {
		prevchar = '\0';
		return -2;
	}

	//ignoring comments
	if(c == '/') {
		prevchar = c;
		c = fgetc(yyin);
		if(c == '/') {
			while(c != '\n') {
				c = fgetc(yyin);
			}
			prevchar = '\0';
		}
		else if(c == '*') {
			char a = fgetc(yyin);
			char b = fgetc(yyin);
			while(!feof(yyin) && !(a == '*' && b == '/')) {
				a = b;
				b = fgetc(yyin);
			}
			c = fgetc(yyin);
			prevchar = '\0';
		}
		else {
			c = prevchar;
			prevchar = '\0';
		}
	}

	//jumping whitespaces again
	if(c == ' ' || c == '\n' || c == '\t') {
		return -2;
	}
	
	/* checking for keywords, reserved words, types, identifiers */

	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		char buff[50] = "";
		int i = 0;
		while(!feof(yyin) && c != ' ' && c != '\n' && c != '\t' && (isalnum(c) || c == '.' || c == '_')) {
			buff[i] = c;
			i++;
			c = fgetc(yyin);
		}
		prevchar = c;
		
		if(!strcmp("for", buff)) {
			return FOR_TOK;
		}
		else if(!strcmp("while", buff)) {
			return WHILE_TOK;
		}
		else if(!strcmp("else", buff)) {
			return ELSE_TOK;
		}
		else if(!strcmp("printf", buff)) {
			return PRINT_TOK;
		}
		else if(!strcmp("return", buff)) {
			return RET_TOK;
		}
		else if(!strcmp("main", buff)) {
			return MAIN_TOK;
		}
		else if(!strcmp("scanf", buff)) {
			return SCAN_TOK;
		}
		else if(!strcmp("include", buff)) {
			return INC_TOK;
		}
		else if(!strcmp("if", buff)) {
			return IF_TOK;
		}
		else if(!strcmp("define", buff)) {
			return DEF_TOK;
		}
		else if(!strcmp("int", buff)) {
			return TINT_TOK;
		}
		else if(!strcmp("char", buff)) {
            return TCHAR_TOK;
        }
		else if(!strcmp("float", buff)) {
            return TFLOAT_TOK;
        }
		else if(!strcmp("double", buff)) {
            return TDOUBLE_TOK;
        }
		else {
			if(validId(buff, i)) {
				return ID_TOK;
			}
			if(isHeader(buff, i)) {
				return HEADER_TOK;
			}
			else {
				return -1;
			}
		}
	}

	/* checking for integer and real constants */

	else if(c >= '0' && c <= '9') {
		char buff[20] = "";
		int i = 0;
		int f = 0;
		while(c >= '0' && c <= '9') {
			buff[i] = c;
			i++;
			c = fgetc(yyin);
		}
		if(c == '.') {
			f = 1;
			buff[i] == c;
			i++;
			while(c >= '0' && c <= '9') {
				buff[i] = c;
				i++;
				c = fgetc(yyin);
			}
		}
		prevchar = c;
		if(f == 0) {
			return INTCONST;
		}
		else {
			return REALCONST;
		}
	}

	/*char const*/
	else if(c == '\'') {
		int i = 0;
		c = fgetc(yyin);
		while(!feof(yyin) && c != '\'') {
			i++;
			c = fgetc(yyin);
		}
		prevchar = c;
		if(i <= 1) {
			return CHARCONST;
		} else {
			return -3;
		}
	}

	/*string const*/
	else if(c == '"') {
		int i = 0;
		c = fgetc(yyin);
		while(!feof(yyin) && c != '"') {
			i++;
			c = fgetc(yyin);
		}
		prevchar = c;
		if(c != '"') {
			return -3;
		}
		return STRCONST;
	}

	/* checking for special symbols */
	
	else if(c == '#') {
		prevchar = '\0';
		return HASH_TOK;
	} else if (c == '(') {
		prevchar = '\0';
		return LPAREN_TOK;
	} else if (c == ')') {
		prevchar = '\0';
        return RPAREN_TOK;
    } 
	
	//checking assignment and logical equals operators
	else if (c == '=') {
		c = fgetc(yyin);
		if(c == '=') {
			prevchar = '\0';
            return EQ_TOK;
		}
		else {
			prevchar = c;
			return ASSIGN_TOK;
		}
        } 

	//more operators and brackets
	else if (c == '-') {
 		prevchar = '\0';
		return MINUS_TOK;
        } else if (c == '+') {
  		prevchar = '\0';
		return PLUS_TOK;
        } else if (c == '*') {
  		prevchar = '\0';
		return STAR_TOK;
        } else if (c == '/') {
    		prevchar = '\0';
		return SLASH_TOK;
        } else if(c == '\\') {
		prevchar = '\0';
		return BACKSLASH_TOK;
	} else if(c == '%') {
		prevchar = '\0';
		return MOD_TOK;
	} else if (c == ';') {
     		prevchar = '\0';
		return SEMICOLON_TOK;
        } else if (c == '!') {
      		prevchar = '\0';
		return NOT_TOK;
        } else if (c == '.') {
       		prevchar = '\0';
		return DOT_TOK;
        } else if (c == ',') {
        	prevchar = '\0';
		return COMMA_TOK;
        } else if (c == '{') {
         	prevchar = '\0';
		return LCURL_TOK;
        } else if (c == '}') {
          	prevchar = '\0';
		return RCURL_TOK;
        } else if (c == '[') {
          	prevchar = '\0';
		return LSQUARE_TOK;
        } else if (c == ']') {
            	prevchar = '\0';
		return RSQUARE_TOK;
        } else if (c == '"') {
             	prevchar = '\0';
		return SPEECH_TOK;
        } else if (c == '\'') {
		prevchar = '\0';
		return INVCOMMA_TOK;
        } 
	
	//checking comparison and bitwise shift operators
	else if (c == '<') {
		c = fgetc(yyin);
		
		if(c == '<') {
			
			prevchar = '\0';
			return LSHIFT_TOK;
		}
		else if(c == '=') {
			
			prevchar = '\0';
			return LTE_TOK;
		}
		else {
            
			prevchar = c;
			return LT_TOK;
		}
        } else if (c == '>') {
		c = fgetc(yyin);
		
		if(c == '>') {
			
			prevchar = '\0';
			return RSHIFT_TOK;
		}
		else if(c == '=') {
			
			prevchar = '\0';
			return GTE_TOK;
		}
		else {
			
                	prevchar = c;
			return GT_TOK;
		}
        } 
	
	/* checking between bitwise and logical operators */

	else if (c == '&') {
		c = fgetc(yyin);
		if(c == '&') {
			prevchar = '\0';
			return AND_TOK;
		}
		else {
			prevchar = c;
			return BIT_AND_TOK;
		}	
        } else if (c == '|') {
                c = fgetc(yyin);
		if(c == '|') {
			prevchar = '\0';
			return OR_TOK;
		}
		else {
			prevchar = c;
			return BIT_OR_TOK;
		}
        }

	/* unidentified token */

	else {
		prevchar = '\0';
		return -3;
	}
}

int main(int argc, char *argv[]) {
	int token;
	if (argc != 3){
		printf("Please enter the name of the file you wish to analyze followed by the name of output file.\n");
		return 0;
	}
	yyin = fopen(argv[1], "r");
	yyout = fopen(argv[2], "w");
	while(!feof(yyin)) {
		token = yylex();
		if(token != -2) fprintf(yyout, "%d\n", token);
	}
	fclose(yyin);
	fclose(yyout);
	return 0;
}
