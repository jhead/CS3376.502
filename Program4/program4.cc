/*
 * SE3376.502 - Program 4
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include "y.tab.h"

using namespace std;

extern char* yytext;
int scan_mode;

// Function prototypes
int yylex(void);
int yyparse (void);
string parseExecName(char* arg);
int scanner();
int parser();

/*
 * Main entry point
 */
int main(int argc, char* argv[]) {

  printf("\n");

  string exec = parseExecName(argv[0]);

  if (exec == "scanner") {
      printf("Operating in scan mode\n\n");

      scan_mode = 1;
      return scanner();
  } else {
      printf("Operating in parse mode\n\n");

      scan_mode = 0;
      return parser();
  }

}

/*
 * Takes argv[0] and finds executable file's name after last slash
 * @return string executable name
 */
string parseExecName(char* arg) {
  string exec = string(arg);
  int last_slash = exec.find_last_of("/");

  if (last_slash != string::npos) {
    exec = exec.substr(last_slash + 1);
  }

  return exec;
}

/*
 * Execute yylex until no tokens are left
 * Information for each token is displayed by scanner
 */
int scanner() {

  // Iterate through tokens
  while ( yylex() ) { }

  printf("\n");

  return 0;
}

/*
 * Execute Bison parser using lexical grammer found in parse.y and return result
 * @return int yyparse result; 0 for success, otherwise failure
 */
 int parser() {
   int result = yyparse();

   printf("\nParse %s!\n", (result == 0 ? "successful" : "failed"));

   return result;
 }
