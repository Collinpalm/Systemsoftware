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
int count;

void printlexerror(int type);
void printtokens();
char* wordRunner(char *input);
void wordcheck(char* input);
void cutwhite(char* input);
//cuts out whitespaces
void cutwhite(char* input){
	while(input[count] == ' '){
		count++;
	}
}
//finds the word in the input array starting at count
char* wordRunner(char *input){
	int wordlen = 0;
	int wcount = count;
	if(input[count] == ' '){
		cutwhite(input);
	}
	while(isalpha(input[wcount])){
		wordlen++;
		wcount++;
	}
	if(wcount > 11){
		return NULL;
	}
	char *word[wordlen];
	word[wordlen] = '\0';
	for(int i = 0; i < wordlen; i++){
		word[i] = input[count+i]; 
	}
	return word;
}
//check if the word is reserved and do the thing for the word
void wordcheck(char *input){
	char* word = wordRunner(input);
	if(strcmp(word, "const")){
		list[lex_index].type = constsym;
		char* name = wordRunner(input);
		if(name == NULL){
			printlexerror(2);
			return NULL;
		}
		strcpy(list[lex_index].name, name);
		lex_index++;
	}else if(strcmp(word, "var")){
		list[lex_index].type = varsym;
		char* name = wordRunner(input);
		if(name == NULL){
			printlexerror(2);
			return NULL;
		}
		strcpy(list[lex_index].name, name);
		lex_index++;
	}else if(strcmp(word, "procedure")){
		list[lex_index].type = procsym;
		char* name = wordRunner(input);
		if(name == NULL){
			printlexerror(2);
			return NULL;
		}
		strcpy(list[lex_index].name, name);
		lex_index++;
	}else if(strcmp(word, "begin")){
		list[lex_index].type = beginsym;
		strcpy(list[lex_index].name, 'begin');
		lex_index++;
	}else if(strcmp(word, "end")){
		list[lex_index].type = endsym;
		strcpy(list[lex_index].name, 'end');
		lex_index++;
	}else if(strcmp(word, "while")){
		list[lex_index].type = whilesym;
		strcpy(list[lex_index].name, 'while');
		lex_index++;
	}else if(strcmp(word, "do")){
		list[lex_index].type = dosym;
		strcpy(list[lex_index].name, 'do');
		lex_index++;
	}else if(strcmp(word, "if")){
		list[lex_index].type = ifsym;
		strcpy(list[lex_index].name, 'if');
		lex_index++;
	}else if(strcmp(word, "then")){
		list[lex_index].type = thensym;
		strcpy(list[lex_index].name, 'then');
		lex_index++;
	}else if(strcmp(word, "else")){
		list[lex_index].type = elsesym;
		strcpy(list[lex_index].name, 'else');
		lex_index++;
	}else if(strcmp(word, "call")){
		list[lex_index].type = callsym;
		strcpy(list[lex_index].name, 'call');
		lex_index++;
	}else if(strcmp(word, "write")){
		list[lex_index].type = writesym;
		strcpy(list[lex_index].name, 'write');
		lex_index++;
	}else if(strcmp(word, "read")){
		list[lex_index].type = readsym;
		strcpy(list[lex_index].name, 'read');
		lex_index++;
	}else if(strcmp(word, "odd")){
		list[lex_index].type = oddsym;
		strcpy(list[lex_index].name, 'odd');
		lex_index++;
	}

}


lexeme *lexanalyzer(char *input){
	lex_index = 0;
	count = 0;
	//iterably loop through the input array
	while(input[count] != '\0'){
		switch(input[count]){
			case ';':
				list[lex_index].type = semicolonsym;
				strcpy(list[lex_index].name, ';');
				lex_index++;
				break;
			case '.':
				list[lex_index].type = periodsym;
				strcpy(list[lex_index].name, '.');
				lex_index++;
				break;
			case ',':
				list[lex_index].type = commasym;
				strcpy(list[lex_index].name, ',');
				lex_index++;
				break;
			case ')':
				list[lex_index].type = rparensym;
				strcpy(list[lex_index].name, ')');
				lex_index++;
				break;
			case '(':
				list[lex_index].type = lparensym;
				strcpy(list[lex_index].name, '(');
				lex_index++;
				break;
			case '>':
				if(input[count+1] == '='){
					list[lex_index].type = geqsym;
					strcpy(list[lex_index].name, '>=');
					lex_index++;
					count++;
				}else{
					list[lex_index].type = gtrsym;
					strcpy(list[lex_index].name, '>');
					lex_index++;
				}
				break;
			case '<':
				if(input[count+1] == '='){
					list[lex_index].type = leqsym;
					strcpy(list[lex_index].name, '<=');
					lex_index++;
					count++;
				}else{
					list[lex_index].type = lsssym;
					strcpy(list[lex_index].name, '<');
					lex_index++;
				}
				break;
			case '!':
				if(input[count+1] == '='){
					list[lex_index].type = leqsym;
					strcpy(list[lex_index].name, '!=');
					lex_index++;
					count++;
				}
				break;
			case '=':
				if(input[count+1] == '='){
					list[lex_index].type = leqsym;
					strcpy(list[lex_index].name, '==');
					lex_index++;
					count++;
				}
				break;
			case '%':
				list[lex_index].type = modsym;
				strcpy(list[lex_index].name, '%');
				lex_index++;
				break;
			case '/':
				if(input[count+1] == '/'){
					//call function to run to the next newline character
				}else{
					list[lex_index].type = divsym;
					strcpy(list[lex_index].name, '/');
					lex_index++;
				}
				break;
			case '*':
				list[lex_index].type = multsym;
				strcpy(list[lex_index].name, '*');
				lex_index++;
				break;
			case '-':
				list[lex_index].type = subsym;
				strcpy(list[lex_index].name, '-');
				lex_index++;
				break;
			case '+':
				list[lex_index].type = addsym;
				strcpy(list[lex_index].name, '+');
				lex_index++;
				break;
			case ':':
				if(input[count+1] == '='){
					list[lex_index].type = leqsym;
					strcpy(list[lex_index].name, ':=');
					lex_index++;
					count++;
				}
				break;
		}
	}
	
	
	
	return NULL;
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