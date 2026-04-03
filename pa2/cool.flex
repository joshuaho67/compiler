/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <string>
#include <iostream>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;
int comment_val;

/*
 *  Add Your own definitions here
 */

%}

/*
 * Define names for regular expressions here.
 */

DARROW          =>
ASSIGN <-
LE <=
SINGLE_CHARACTER "+"|"-"|"*"|"/"|"~"|";"|"@"|":"|"{"|"}"|"("|")"|"."|","|"<"|"="
LETTER [a-zA-Z]
DIGIT [0-9]
TYPEID [A-Z]({LETTER}|{DIGIT}|_)* 
OBJECTID [a-z]({LETTER}|{DIGIT}|_)* 
INTEGER [0-9]+

/* %x: exclusive start condition
   %s: inclusive start condition
*/
%x STR COMMENT SINGLE_LINE
%%

 /*
  *  Nested comments
  *  Begin of rules following the format of 
  *  regular expression: action
  */


 /*
  *  The multiple-character operators.
  */
{DARROW}		{ return (DARROW); }
{ASSIGN} return ASSIGN;
{LE} return LE;

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */
t[rR][uU][eE] {
	yylval.boolean = true;
	return BOOL_CONST;
}
f[aA][lL][sS][eE] {
	yylval.boolean = false;
	return BOOL_CONST;
}
[cC][lL][aA][sS][sS] return CLASS;
[eE][lL][sS][eE] return ELSE;
[fF][iI] return FI;
[iI][fF] return IF;
[iI][nN] return IN;
[iI][nN][hH][eE][rR][iI][tT][sS] return INHERITS;
[iI][sS][vV][oO][iI][dD] return ISVOID;
[lL][eE][tT] return LET;
[lL][oO][oO][pP] return LOOP;
[pP][oO][oO][lL] return POOL;
[tT][hH][eE][nN] return THEN;
[wW][hH][iI][lL][eE] return WHILE;
[cC][aA][sS][eE] return CASE;
[eE][sS][aA][cC] return ESAC;
[nN][eE][wW] return NEW;
[oO][fF] return OF;
[nN][oO][tT] return NOT;

{INTEGER} {
	int acc=0;
	for (char* c=yytext;*c!=0;c++){
		acc = acc*10+(*c-'0');
	}
	yylval.symbol = inttable.add_string(yytext);
	return INT_CONST;
}
 /*
*	identifiers
*/
{TYPEID} {
	yylval.symbol = stringtable.add_string(yytext);
	return TYPEID;
}
{OBJECTID} {
	yylval.symbol = stringtable.add_string(yytext);
	return OBJECTID;
}
"-""-" {
	BEGIN SINGLE_LINE;
}
<SINGLE_LINE><<EOF>> BEGIN 0;
<SINGLE_LINE>\n {
	curr_lineno ++;
	BEGIN 0;
}
<SINGLE_LINE>(.) ;
"*"")" {
	yylval.error_msg = "Unmatched *)";
	return ERROR;
}
 /* comment */
"(""*" {
	BEGIN COMMENT;
	comment_val = 1;
}
<COMMENT>\n {
	curr_lineno ++;
}
<COMMENT>"(""*" {
	comment_val ++;
}
<COMMENT>"*"")" {
	if (comment_val == 1) {
		BEGIN 0;
	} else {
		comment_val --;
	}
}
<COMMENT><<EOF>> {
	yylval.error_msg = "EOF in comment";
	yyrestart(yyin);
	BEGIN 0;
	return ERROR;
}
<COMMENT>(.) ;

 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */
 /*
	initial state, if a single quot matches, enters the STR state.
	if \ \n is found, it's ok
	if \ anychar is found, it's ok
	if \n is found, it's not ok
	if other char is found, it's ok.
 */
\" {
	yymore();
	BEGIN STR;
}
<STR>(\\)(\n) {
	curr_lineno ++;
	yymore();
} 
<STR>(\\)(.) {
	//if (yytext[yyleng-1] == '\0'){
	//	yylval.error_msg = "String contains escaped null characters.";
	//	BEGIN 0;
	//	return ERROR;
	//}
	//else
		yymore();
}
<STR>(\n) {
	yylval.error_msg = "Unterminated string constant";
	BEGIN 0;
	//std::cout<<"newline in string detected at :"<<curr_lineno<<std::endl;
	curr_lineno ++;
	return ERROR;
}
 /* convert two characters to one character */
<STR>\" {
	/* resume normal state */
	BEGIN 0;
		char *ptr,*newstring;
		//std::cout<<"yytext and yylength"<<yytext<<":"<<yyleng<<std::endl;
		ptr = newstring = new char[yyleng];			
		int i=1;
		// string content is yytext[1:yyleng-2] 	
		// 
		int strlen = 0;
		while (i<yyleng-2) {
			char c = yytext[i];
			
			if (c == '\\')
			{
				char d = yytext[i+1];
				if (d=='n'){
					*(ptr++)='\n';
				} else if (d == 'b'){
					*(ptr++)='\b';
				} else if (d == 't'){
					*(ptr++) = '\t';
				} else if (d == 'f'){
					*(ptr++) = '\f';
				} else if (d == '\0'){
					yylval.error_msg = "String contains escaped null characters.";
					BEGIN 0;
					return ERROR;
				} else {
					*(ptr++)=d;
				}
				i+=2;
			} else if (c == '\0'){
				yylval.error_msg = "String contains null character.";
				BEGIN 0;
				return ERROR;
			} else {
				*(ptr++)=c;
				i++;
			}
			strlen++;
		}
		if (i==yyleng-2) {
			if (yytext[i]=='\0'){
				yylval.error_msg = "String contains null character.";
				BEGIN 0;
				return ERROR;
			}	
			*(ptr++) = yytext[i];
			strlen++;
		}
		*ptr='\0';
		if (strlen > MAX_STR_CONST-1){
			yylval.error_msg = "String constant too long";
			// std::cout<<"strlen is :"<<strlen<<std::endl;
			BEGIN 0;
			return ERROR;
		}
		yylval.symbol = stringtable.add_string(newstring);
		//std::cout<<"newstring is"<<newstring<<std::endl;
		return STR_CONST;
}
<STR><<EOF>> {
	yylval.error_msg = "EOF in string constant";
	/* a trick, switch to current file . to continue processing */
	// std::cout<<"yyin is"<<yyin<<std::endl;
	yyrestart(yyin);
	BEGIN 0;
	return ERROR;
}
<STR>. {
	yymore();
}
{SINGLE_CHARACTER} return yytext[0];
\n {
	// std::cout<<"newline match at :"<<curr_lineno<<std::endl;
	curr_lineno ++;
}
" "|\f|\r|\t|\v ;
. {
	yylval.error_msg = yytext;
	return ERROR;
}
%%
void print(char c){
	std::cout<<c;
}
//void print(char* c){
//	whilej
