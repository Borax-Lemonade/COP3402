/* 
	This is the lex.c file for the UCF Fall 2021 Systems Software Project.
	For HW2, you must implement the function lexeme *lexanalyzer(char *input).
	You may add as many constants, global variables, and support functions
	as you desire.
	
	If you choose to alter the printing functions or delete list or lex_index, 
	you MUST make a note of that in you readme file, otherwise you will lose 
	5 points.
*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;
void printlexerror(int type);
void printtokens();
lexeme *lexanalyzer(char *input);
instruction *parse(lexeme *list);
void execute_program(instruction *code);

lexeme *lexanalyzer(char *input)
{	
	int lex_index = 0;
	
	int length_of_input = strlen(input);
	int start = 0, end = 0;
	
	while (end <= length_of_input) {
		if (isValidDelimeter(input[end] == false) {
			end++;
		}
		else if (isValidDelimeter(input[end]) == true) {
			if (start == end) {
				if (isValidSymbol(input[end]) == true) {
					strcpy(list[lex_index].name, substring(input, start, end));
					//based on symbol put that type into list[lex_index++].type;
				}
				//shift start and end
			} else if (start != end || end == length) { //not just one char,, could be the end of input
				if (isValidReserved(subString(input, start, end-1))) {
					strcpy(list[lex_index].name, substring(input, start, end)); //could be end-1
					//based on reserved put that type into list[lex_index++].type;
				}
				else if (isValidDigit(subString(input, start, end-1))) {
					strcpy(list[lex_index].name, substring(input, start, end));
					list[lex_index++].type = 15;
				}
				else if (isValidIdentifier(substring(input, start, end))) {
					strcpy(list[lex_index].name, substring(input, start, end));
					list[lex_index++].type = 14;
				}
			}
			start = end;
		}
	}
	if (list == null)
		return NULL;
	
	printtokens();
	
	return list;
}

//0 is false
bool isValidDelimeter(char ch) {
	
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ',' || ch == ';' || ch == '.' ||
		ch == '>' || ch == '<' || ch = ':=' || ch == '(' || ch == ')' || ch == '<=' || ch == '>=' || ch == '%' ||
		ch == '!=' || ch == '==' || ch == '\0' || ch == '\t')
			return true;
	return false;
}

bool isValidSymbol(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' || ch == '<' || ch = ':=' || ch == '(' 
		|| ch == ')' || ch == '<=' || ch == '>=' || ch == '%' || ch == '!=' || ch == '==' || ch == ',' 
		|| ch == ';' || ch == '.' ||)
			return true;
	return false;
}

bool isValidIdentifier(char* str) {
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3' || str[0] == '4' || str[0] == '5' || 
		str[0] == '6' || str[0] == '7' || str[0] == '8' || str[0] == '9' || isValidDelimeter(str[0]) == true)
			return false;
	return true;
	
}

bool isValidDigit(char* str) {
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3' || str[0] == '4' || str[0] == '5' || 
		str[0] == '6' || str[0] == '7' || str[0] == '8' || str[0] == '9')
			return true;
	return false;
}

bool isValidReserved(char* str) {
   if (!strcmp(str, "const") || !strcmp(str, "var") || !strcmp(str, "procedure") || !strcmp(str, "call") 
	    || !strcmp(str, "if") || !strcmp(str, "then") || !strcmp(str, "else") || !strcmp(str, "while")
		|| !strcmp(str, "do") || !strcmp(str, "begin") || !strcmp(str,"end") || !strcmp(str, "read") || !strcmp(str, "write") || !strcmp(str, "odd"))
			return true;
   return false;
   
}

int getReservedWord(char *str) {
	switch(str) {
		case "const":
			return 1;
			break;
		case "var":
			return 2;
			break;
		case "procedure":
			return 3;
			break;
		case "begin":
			return 4;
			break;
		case "end":
			return 5;
			break;
		case "while":
			return 6;
			break;
		case "do":
			return 7;
			break;
		case "if":
			return 8;
			break;
		case "then":
			return 9;
			break;
		case "else":
			return 10;
			break;
		case "call":
			return 11;
			break;
		case "write":
			return 12;
			break;
		case "read":
			return 13;
			break;
}

int getSymbol(char *str) {
	switch(str) {
		case ":=":
			return 16;
			break;
		case "+":
			return 17;
			break;
		case "-":
			return 18;
			break;
		case "*":
			return 19;
			break;
		case "/":
			return 20;
			break;
		case "%":
			return 21;
			break;
		case "==":
			return 22;
			break;
		case "!=":
			return 23;
			break;
		case "<":
			return 24;
			break;
		case "<=":
			return 25;
			break;
		case ">":
			return 26;
			break;
		case ">=":
			return 27;
			break;
		case "(":
			return 29;
			break;
		case ")":
			return 30;
			break;
		case ",":
			return 31;
			break;
		case ".":
			return 32;
			break;
		case ";":
			return 33;
			break;
		
	}
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}
}