/*
 * SE3376.502 - Program 4
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

%option nounput
%option noinput
%option noyywrap

%{
  #include <iostream>
  #include "y.tab.h"
  using namespace std;

  extern int scan_mode;
  int scan(int, char*);
%}

%%

[ \t\r]+                     { /* gobble gobble */ }
[0-9]+                       { return scan(INTTOKEN, yytext); }
[IVX]+                       { return scan(ROMANTOKEN, yytext); }
[a-zA-Z](\.?)                { return scan(NAME_INITIAL_TOKEN, yytext); }
[a-zA-Z]{2,}                 { return scan(NAMETOKEN, yytext); }
[a-zA-Z0-9]{1,}              { return scan(IDENTIFIERTOKEN, yytext); }
"Sr\."                       { return scan(SRTOKEN, yytext); }
"Jr\."                       { return scan(JRTOKEN, yytext); }
"\n"                         { return scan(EOLTOKEN, yytext); }
","                          { return scan(COMMATOKEN, yytext); }
"-"                          { return scan(DASHTOKEN, yytext); }
"#"                          { return scan(HASHTOKEN, yytext); }

%%

int scan(int token_enum, char* str) {

  const char* token_str;
  char* strval = strdup(yytext);
  int token_type = 0;

  switch(token_enum) {
    case INTTOKEN:
      token_str = "INTTOKEN";
      token_type = 1;
      break;
    case ROMANTOKEN:
      token_str = "ROMANTOKEN";
      break;
    case NAMETOKEN:
      token_str = "NAMETOKEN";
      break;
    case IDENTIFIERTOKEN:
      token_str = "IDENTIFIERTOKEN";
      break;
    case NAME_INITIAL_TOKEN:
      token_str = "NAME_INITIAL_TOKEN";
      break;
    case SRTOKEN:
      token_str = "SRTOKEN";
      break;
    case JRTOKEN:
      token_str = "JRTOKEN";
      break;
    case EOLTOKEN:
      token_str = "EOLTOKEN";
      break;
    case COMMATOKEN:
      token_str = "COMMATOKEN";
      break;
    case DASHTOKEN:
      token_str = "DASHTOKEN";
      break;
    case HASHTOKEN:
      token_str = "HASHTOKEN";
      break;
    default:
      token_str = "UNKNOWN";
  }

  switch (token_type) {
    // String
    case 0:
      yylval.strval = yytext;
      break;
    // int
    case 1:
      yylval.intval = atoi(yytext);
      break;
    // Everything else
    default:
      break;
  }

  if (scan_mode) {
    if (token_enum != EOLTOKEN) {
      printf("yylex found %s token (%s)\n", token_str, strval);
    } else {
      printf("yylex found %s token\n", token_str);
    }
  }

  return token_enum;
}
