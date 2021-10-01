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
#define false 0
#define true 1

lexeme *list;
int lex_index;

void printlexerror(int type);
void printtokens();
lexeme *lexanalyzer(char *input);
instruction *parse(lexeme *list);
void execute_program(instruction *code);
int isValidDelimeter(char* str);
int isValidSymbol(char* str);
int isValidIdentifier(char* str);
int isValidDigit(char* str);
int isValidReserved(char* str);
char * subString(char* str, int right, int left);
int getType(char* str);

lexeme *lexanalyzer(char *input)
{	
	list = (lexeme*) malloc(sizeof(lexeme)*1000000);
	
	lex_index = 0;
	
	int length_of_input = strlen(input);
	int start = 0, end = 0;
	char* endEndSub = "";
	char* endSub = "";
	char* sub = "";
	
	//printf("we are beginning");
	while (end <= length_of_input-1 && start <= end) {  //might have to be end < length_of_input
		
		sub = subString(input, start, end);
		endSub = subString(input, end-1, end);
		endEndSub = subString(input, end, end);
		
		list[lex_index].name = malloc(sizeof(char)*12);
		strcpy(list[lex_index].name, " ");
		
		if (isValidDelimeter(sub) == false && isValidDelimeter(endSub) == false && isValidDelimeter(endEndSub) == false) {
			end++;
			//printf("%d", end);
		}
		
		else if (isValidDelimeter(sub) == true || isValidDelimeter(endSub) == true || isValidDelimeter(endEndSub) == true) {
				if(list[lex_index-1].type == 20 && getType(sub) == 20) {
					while (iscntrl(input[end]) == 0) {
						end++;
					}
					//strcpy(list[lex_index].name, " ");
					//free(list[lex_index-1].name);
					lex_index-=2;
					//free(list[lex_index-1].type);
				} else {
					if (isValidSymbol(sub) == true) {
						
						strcpy(list[lex_index].name, sub);
						list[lex_index].type = getType(sub);
						printf("sybmol: %s\n", list[lex_index].name);
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
					
					else if (isValidReserved(subString(input, start, end-1)) == true) {
						strcpy(list[lex_index].name, subString(input, start, end-1)); 
						list[lex_index].type = getType(subString(input, start, end-1));
						printf("%d ", lex_index);
						printf("reserved: %s\n", list[lex_index].name);
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
					else if (isValidDigit(sub) == true) {
						strcpy(list[lex_index].name, subString(input, start, end-1));
						list[lex_index].type = numbersym;
						list[lex_index].value = atoi(list[lex_index].name);
						printf("%d ", lex_index);
						printf("digit: %d\n", list[lex_index].value);
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
					else if (isValidIdentifier(sub) == true && isValidReserved(subString(input, start, end-1)) == false) {
						strcpy(list[lex_index].name, subString(input, start, end-1));
						list[lex_index].type = identsym;
						
						printf("%d ", lex_index);
						printf("id: %s\n",list[lex_index].name); //factor in getting rid of delimeter
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
					
					if (isValidSymbol(sub) == false && isValidSymbol(endSub) == true) {
						lex_index++;
						list[lex_index].name = malloc(sizeof(char)*12);
						strcpy(list[lex_index].name, endSub);
						list[lex_index].type = getType(endSub);
						
						printf("%d ", lex_index);
						printf("symbol2: %s\n", list[lex_index].name);
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
					if (isValidSymbol(sub) == false && isValidSymbol(endSub) == false && isValidSymbol(endEndSub) == true) {
						lex_index++;
						list[lex_index].name = malloc(sizeof(char)*12);
						strcpy(list[lex_index].name, endEndSub);
						list[lex_index].type = getType(endEndSub);
						
						printf("%d ", lex_index);
						printf("symbol3: %s\n", list[lex_index].name);
						printf("type: %d\n", list[lex_index].type);
						//lex_index++;
					}
				}
			//}
			end++;
			start = end;
			lex_index++;
			
		}
		if (lex_index >= sizeof(*list)) {
			//printf("i did it");
			//realloc list to be twice as large
			
			list = (lexeme*)realloc(list, (sizeof(lexeme)*(sizeof(list))*2));
		}
	}
	if (list == NULL) {
		printf("it be null");
		return NULL;
	}
	
	printf("lex_index %d\n", lex_index);

	printtokens();
	
	return list;
}
int getType(char* str) {
	if (strcmp(str, "const") == 0)
		return constsym;
	else if (strcmp(str, "var") == 0)
		return varsym;
	else if (strcmp(str, "procedure") == 0)
		return procsym;
	else if (strcmp(str, "begin") == 0)
		return beginsym;
	else if (strcmp(str, "end") == 0)
		return endsym;
	else if (strcmp(str, "while") == 0)
		return whilesym;
	else if (strcmp(str, "do") == 0)
		return dosym;
	else if (strcmp(str, "if") == 0)
		return ifsym;
	else if (strcmp(str, "then") == 0)
		return thensym;
	else if (strcmp(str, "else") == 0)
		return elsesym;
	else if (strcmp(str, "call") == 0)
		return callsym;
	else if (strcmp(str, "write") == 0)
		return writesym;
	else if (strcmp(str, "read") == 0)
		return readsym;
	//id/
	//numbersy
	else if (strcmp(str, ":=") == 0)
		return assignsym;
	else if (strcmp(str, "-") == 0)
		return subsym;
	else if (strcmp(str, "*") == 0)
		return multsym;
	else if (strcmp(str, "/") == 0)
		return divsym;
	else if (strcmp(str, "%") == 0)
		return modsym;
	else if (strcmp(str, "==") == 0)
		return eqlsym;
	else if (strcmp(str, "!=") == 0)
		return neqsym;
	else if (strcmp(str, "<") == 0)
		return lsssym;
	else if (strcmp(str, "<=") == 0)
		return leqsym;
	else if (strcmp(str, ">") == 0)
		return gtrsym;
	else if (strcmp(str, ">=") == 0)
		return geqsym;
	else if (strcmp(str, "odd") == 0)
		return oddsym;
	else if (strcmp(str, "(") == 0)
		return lparensym;
	else if (strcmp(str, ")") == 0)
		return rparensym;
	else if (strcmp(str, ",") == 0)
		return commasym;
	else if (strcmp(str, ".") == 0)
		return periodsym;
	else if (strcmp(str, ";") == 0)
		return semicolonsym;
	else
		return -1;
}

char* subString(char* str, int left, int right) {
	//printf("\nleft: %d right: %d\n", left, right);
	char *ret = (char*)malloc(sizeof(char)*(right-left+1));
	
	for (int i = left; i <= right; i++) {
		ret[i-left] = str[i];
	}
	ret[right-left+1] = '\0';
	//printf("substring: %s\n\n", ret);
	return ret;
}

//0 is false

int isValidDelimeter(char* str) {
	if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "*") == 0 
	  || strcmp(str, "/") == 0 || strcmp(str,">") == 0 || strcmp(str, "<") == 0 
	  || strcmp(str, ":=") == 0 || strcmp(str, "(") == 0 || strcmp(str, ")") == 0
	  || strcmp(str, "<=") == 0 || strcmp(str, ">=") == 0 || strcmp(str, "%") == 0
	  || strcmp(str, "!=") == 0 || strcmp(str, "==") == 0 || strcmp(str, ",") ==0  
	  || strcmp(str, ";") == 0 || strcmp(str, ".")  == 0 || strcmp(str, " ") == 0 
	  || strcmp(str, "\n") == 0 || strcmp(str, "\t") == 0  || iscntrl(str[strlen(str)-1]) != 0) {
			//printf("passed: %s\n", str);
			return true;
	} else {
		//printf("failed: %s\n", str);
		return false;
	}
}

int isValidSymbol(char* str) {
	if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "*") == 0 
  || strcmp(str, "/") == 0 || strcmp(str,">") == 0 || strcmp(str, "<") == 0 
  || strcmp(str, ":=") == 0 || strcmp(str, "(") == 0 || strcmp(str, ")") == 0
  || strcmp(str, "<=") == 0 || strcmp(str, ">=") == 0 || strcmp(str, "%") == 0
  || strcmp(str, "!=") == 0 || strcmp(str, "==") == 0 || strcmp(str, ",") ==0  
	|| strcmp(str, ";") == 0 || strcmp(str, ".")  == 0)
			return true;
	return false;
}

int isValidIdentifier(char* str) {
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3'
	   || str[0] == '4' || str[0] == '5' || str[0] == '6' || str[0] == '7'
	   || str[0] == '8' || str[0] == '9' || strlen(str) > 11 || str[strlen(str)-1] == ':'
	   ||isValidDelimeter(subString(str, 0, 0)) == true) //might have to be substring(str, 0, 1) -- but then what if the identifier is a single char
			return false;
	return true;
	
}

int isValidDigit(char* str) {
	if ((str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3' || str[0] == '4' || str[0] == '5' || 
		str[0] == '6' || str[0] == '7' || str[0] == '8' || str[0] == '9') && strlen(str) < 5)
			return true;
	return false;
}

int isValidReserved(char* str) {
	//printf("here");
   if (!strcmp(str, "const") || !strcmp(str, "var") || !strcmp(str, "procedure") || !strcmp(str, "call") 
	    || !strcmp(str, "if") || !strcmp(str, "then") || !strcmp(str, "else") || !strcmp(str, "while")
		|| !strcmp(str, "do") || !strcmp(str, "begin") || !strcmp(str,"end") || !strcmp(str, "read")
		|| !strcmp(str, "write") || !strcmp(str, "odd"))
			return true;
   return false;
   
}

void printtokens()
{
	int i;
	//printf("lex_index: %d", lex_index);
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