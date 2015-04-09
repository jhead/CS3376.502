/*
 * SE3376.502 - Program 4
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

%{
  #include <iostream>
  #include <stdio.h>
  using namespace std;

  int yylex(void);
  void yyerror(std::string);
%}

/* Tokens */
%token NAMETOKEN
%token IDENTIFIERTOKEN
%token NAME_INITIAL_TOKEN
%token ROMANTOKEN
%token SRTOKEN
%token JRTOKEN
%token EOLTOKEN
%token INTTOKEN
%token COMMATOKEN
%token DASHTOKEN
%token HASHTOKEN

/* yyval variables */
%union
{
    int intval;
    char *strval;
}

%type	<strval> NAMETOKEN
%type	<strval> IDENTIFIERTOKEN
%type	<strval> NAME_INITIAL_TOKEN
%type	<strval> ROMANTOKEN
%type	<strval> SRTOKEN
%type	<strval> JRTOKEN
%type	<strval> EOLTOKEN
%type	<strval> COMMATOKEN
%type	<strval> DASHTOKEN
%type	<strval> HASHTOKEN
%type	<intval> INTTOKEN

%%

/* Rules */
postal_addresses : address_block EOLTOKEN postal_addresses
								 | address_block
                 ;

address_block    : name_part street_address location_part { fprintf(stderr, "\n"); }
						     ;

name_part        : first_name last_name suffix_part EOLTOKEN
				         | first_name last_name EOLTOKEN
				         | error EOLTOKEN { printf("Invalid name! Skipping...\n"); }
				         ;

first_name       : NAMETOKEN { fprintf(stderr, "<FirstName>%s</FirstName>\n", $1); }
				 		     | NAME_INITIAL_TOKEN { fprintf(stderr, "<FirstName>%s</FirstName>\n", $1); }
						     ;

last_name        : NAMETOKEN { fprintf(stderr, "<LastName>%s</LastName>\n", $1); }
	               ;

suffix_part      : SRTOKEN { fprintf(stderr, "<Suffix>%s</Suffix>\n", $1); }
					       | JRTOKEN { fprintf(stderr, "<Suffix>%s</Suffix>\n", $1); }
					       | ROMANTOKEN { fprintf(stderr, "<Suffix>%s</Suffix>\n", $1); }
					       ;

street_address   : street_number street_name INTTOKEN EOLTOKEN { fprintf(stderr, "<AptNum>%d</AptNum>\n", $3); }
							   | street_number street_name HASHTOKEN INTTOKEN EOLTOKEN { fprintf(stderr, "<AptNum>%d</AptNum>\n", $4); }
							   | street_number street_name EOLTOKEN
							   | error EOLTOKEN { printf("Invalid street address! Skipping...\n"); }
							   ;

street_number    : INTTOKEN { fprintf(stderr, "<HouseNumber>%d</HouseNumber>\n", $1); }
                 | IDENTIFIERTOKEN { fprintf(stderr, "<HouseNumber>%s</HouseNumber>\n", $1); }
						     ;

street_name      : NAMETOKEN { fprintf(stderr, "<StreetName>%s</StreetName>\n", $1); }
                 ;

location_part    : town_name COMMATOKEN state_code zip_code EOLTOKEN
					       | error EOLTOKEN { printf("Invalid location! Skipping...\n"); }
						     ;

town_name        : NAMETOKEN { fprintf(stderr, "<City>%s</City>\n", $1); }
                 ;

state_code       : NAMETOKEN { fprintf(stderr, "<State>%s</State>\n", $1); }
                 ;

zip_code         : INTTOKEN DASHTOKEN INTTOKEN { fprintf(stderr, "<Zip5>%d</Zip5>\n<Zip4>%d</Zip4>\n", $1, $3); }
                 | INTTOKEN { fprintf(stderr, "<Zip5>%d</Zip5>\n", $1); }
				         ;

%%

void yyerror(std::string str) { /* Not needed */ }
