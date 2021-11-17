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
#include <unistd.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;
int count, flag, varCount;
char varnames[MAX_NUMBER_TOKENS][MAX_IDENT_LEN];

void printlexerror(int type);
void printtokens();
char* wordRunner(char *input);
void wordcheck(char* input);
void cutwhite(char* input);
int checkvalid(char* word);
int getnum(char* input);
void runtonewline(char* input);
void addtovarlist(char* word);

//add as used word
void addtovarlist(char* word){
	strcpy(varnames[varCount], word);
	varCount++;
}


//run to new line
void runtonewline(char* input){
	while(!iscntrl(input[count])){
		count++;
	}
}

//checks if a word is not reserved
int checkvalid(char* word){
	if(strcmp(word, "const") == 0){
		return 1;
	}else if(strcmp(word, "var") == 0){
		return 1;
	}else if(strcmp(word, "procedure") == 0){
		return 1;
	}else if(strcmp(word, "do") == 0){
		return 1;
	}else if(strcmp(word, "od") == 0){
		return 1;
	}else if(strcmp(word, "while") == 0){
		return 1;
	}else if(strcmp(word, "do") == 0){
		return 1;
	}else if(strcmp(word, "when") == 0){
		return 1;
	}else if(strcmp(word, "elsedo") == 0){
		return 1;
	}else if(strcmp(word, "call") == 0){
		return 1;
	}else if(strcmp(word, "write") == 0){
		return 1;
	}else if(strcmp(word, "read") == 0){
		return 1;
	}else if(strcmp(word, "odd") == 0){
		return 1;
	}else{
		//checks if the variable name has been used, if so it returns that its valid
		for(int i = 0; i < varCount; i++){
			if(varnames[i]==NULL){
				continue;
			}else if(strcmp(word, varnames[i]) == 0){
				return 2;
			}
		}
	}
	return 0;
}

//cuts out whitespaces
void cutwhite(char* input){
	while(input[count] == ' '){
		count++;
	}
}
//finds the word in the input array starting at count
char* wordRunner(char *input){
	
	if(input[count] == ' '){
		cutwhite(input);
	}
	int wordlen = 0;
	int wcount = count;
	//find how long the word is based on if it detects an alphabet character
	while(isalpha(input[count]) != 0 || isdigit(input[count])){
		wordlen++;
		count++;
		
	}
	
	//if the word is too long 
	if(wordlen > MAX_IDENT_LEN){
		printlexerror(4);
		flag = 1;
	}
	//pull the word out
	
	char *word = malloc(sizeof(char)*wordlen);
	for(int i = 0; i < wordlen; i++){
		word[i] = input[wcount+i]; 
	}
	return word;
}
//check if the word is reserved and do the thing for the word
void wordcheck(char *input){
	//get the word
	char* word = wordRunner(input);
	//null check
	
	if(word == NULL){
		printlexerror(2);
		flag = 1;
		return;
	}
	
	//if\else to find the word
	
	if(strcmp(word, "const") == 0){
		//add the reserved word to the list
		list[lex_index].type = constsym;
		strcpy(list[lex_index].name, "const");
		list[lex_index].value = 1;
		lex_index++;
	}else if(strcmp(word, "var") == 0){
		list[lex_index].type = varsym;
		strcpy(list[lex_index].name, "var");
		list[lex_index].value = 2;
		lex_index++;
	}else if(strcmp(word, "procedure") == 0){
		list[lex_index].type = procsym;
		strcpy(list[lex_index].name, "procedure");
		list[lex_index].value = 3;
		lex_index++;
	}else if(strcmp(word, "do") == 0){
		list[lex_index].type = dosym;
		strcpy(list[lex_index].name, "do");
		list[lex_index].value = 4;
		lex_index++;
	}else if(strcmp(word, "od") == 0){
		list[lex_index].type = odsym;
		strcpy(list[lex_index].name, "od");
		list[lex_index].value = 5;
		lex_index++;
	}else if(strcmp(word, "while") == 0){
		list[lex_index].type = whilesym;
		strcpy(list[lex_index].name, "while");
		list[lex_index].value = 6;
		lex_index++;
	}else if(strcmp(word, "do") == 0){
		list[lex_index].type = dosym;
		strcpy(list[lex_index].name, "do");
		list[lex_index].value = 7;
		lex_index++;
	}else if(strcmp(word, "when") == 0){
		list[lex_index].type = whensym;
		strcpy(list[lex_index].name, "when");
		list[lex_index].value = 8;
		lex_index++;
	}else if(strcmp(word, "elsedo") == 0){
		list[lex_index].type = elsedosym;
		strcpy(list[lex_index].name, "elsedo");
		list[lex_index].value = 10;
		lex_index++;
	}else if(strcmp(word, "call") == 0){
		list[lex_index].type = callsym;
		strcpy(list[lex_index].name, "call");
		list[lex_index].value = 11;
		lex_index++;
	}else if(strcmp(word, "write") == 0){
		list[lex_index].type = writesym;
		strcpy(list[lex_index].name, "write");
		list[lex_index].value = 12;
		lex_index++;
	}else if(strcmp(word, "read") == 0){
		list[lex_index].type = readsym;
		strcpy(list[lex_index].name, "read");
		list[lex_index].value = 13;
		lex_index++;
	}else if(strcmp(word, "odd") == 0){
		list[lex_index].type = oddsym;
		strcpy(list[lex_index].name, "odd");
		list[lex_index].value = 28;
		lex_index++;
	}else{
		if(checkvalid(word) == 0){
			list[lex_index].type = identsym;
			strcpy(list[lex_index].name, word);
			list[lex_index].value = 14;
			lex_index++;
			addtovarlist(word);
		}else if(checkvalid(word) == 2){
			list[lex_index].type = identsym;
			strcpy(list[lex_index].name, word);
			list[lex_index].value = 14;
			lex_index++;
		}
	}

}

//function to get a number out of the input
int getnum(char* input){
	//start at count
	int initial = count;
	//find the length of the count
	while(isdigit(input[count])){
		count++;
	}
	char* str = malloc(sizeof(char)* (count-initial));
	//if the length of the digit is short enough count it and return it
	if(count-initial < MAX_NUMBER_LEN){
		for(int i = 0; i< count-initial; i++){
			str[i] = input[initial+i];
		}
		if(isalpha(input[count+1]) != 0){
			printlexerror(2);
			flag = 1;
			return 0;
		}
		return atoi(str);
	}
	//if the word is too long return an impossible value
	return -1;
	
}

lexeme *lexanalyzer(char *input, int printFlag){
	lex_index = 0;//set the list counter to the doing
	count = 0;//set the input counter to the doing
	flag = 0;//set the flag to no error(1-error, 0-noerror)
	varCount = 0;
	list = calloc(sizeof(lexeme), MAX_NUMBER_TOKENS);
	//iterably loop through the input array
	
	while(input[count] != '\0'){
		//printf("Loop count: %d, current char: %c\n", count, input[count]);
		//check if its a control character and skip it
		if(iscntrl(input[count])){
			count++;
		//check if its a number and count it
		}else if(isdigit(input[count])!=0){
			int temp = getnum(input);
			if(temp == -1){
				flag = 1;
				printlexerror(3);
			}
			list[lex_index].type = numbersym;
			//strcpy(list[lex_index].name, temp);
			list[lex_index].value = temp;
			lex_index++;
		//check if its an alphabet character and check for word
		}else if(isalpha(input[count]) != 0){
			wordcheck(input);

		//check if its one of the char operators
		}else{
			switch(input[count]){
				case ';':
					list[lex_index].type = semicolonsym;
					strcpy(list[lex_index].name, ";");
					list[lex_index].value = 33;
					lex_index++;
					count++;
					break;
				case '.':
					list[lex_index].type = periodsym;
					strcpy(list[lex_index].name, ".");
					list[lex_index].value = 32;
					lex_index++;
					count++;
					break;
				case ',':
					list[lex_index].type = commasym;
					strcpy(list[lex_index].name, ",");
					list[lex_index].value = 31;
					lex_index++;
					count++;
					break;
				case ')':
					list[lex_index].type = rparensym;
					strcpy(list[lex_index].name, ")");
					list[lex_index].value = 30;
					lex_index++;
					count++;
					break;
				case '(':
					list[lex_index].type = lparensym;
					strcpy(list[lex_index].name, "(");
					list[lex_index].value = 29;
					lex_index++;
					count++;
					break;
				case '>':
					if(input[count+1] == '=' ){
						list[lex_index].type = geqsym;
						strcpy(list[lex_index].name, ">=");
						list[lex_index].value = 27;
						lex_index++;
						count+=2;
					}else{
						list[lex_index].type = gtrsym;
						strcpy(list[lex_index].name, ">");
						list[lex_index].value = 26;
						lex_index++;
						count++;
					}
					break;
				case '<':
					if(input[count+1] == '='){
						list[lex_index].type = leqsym;
						strcpy(list[lex_index].name, "<=");
						list[lex_index].value = 25;
						lex_index++;
						count+=2;
					}else{
						list[lex_index].type = lsssym;
						strcpy(list[lex_index].name, "<");
						list[lex_index].value = 24;
						lex_index++;
						count++;
					}
					break;
				case '!':
					if(input[count+1] == '='){
						list[lex_index].type = neqsym;
						strcpy(list[lex_index].name, "!=");
						list[lex_index].value = 23;
						lex_index++;
						count+=2;
					}else{
						printlexerror(1);
						flag = 1;
					}
					break;
				case '=':
					if(input[count+1] == '='){
						list[lex_index].type = eqlsym;
						strcpy(list[lex_index].name, "==");
						list[lex_index].value = 22;
						lex_index++;
						count+=2;
					}
					break;
				case '%':
					list[lex_index].type = modsym;
					strcpy(list[lex_index].name, "%");
					list[lex_index].value = 21;
					lex_index++;
					count++;
					break;
				case '/':
					if(input[count+1] == '/'){
						//call function to run to the next newline character
						runtonewline(input);
					}else{
						list[lex_index].type = divsym;
						strcpy(list[lex_index].name, "/");
						list[lex_index].value = 20;
						lex_index++;
						count++;
					}
					break;
				case '*':
					list[lex_index].type = multsym;
					strcpy(list[lex_index].name, "*");
					list[lex_index].value = 19;
					lex_index++;
					count++;
					break;
				case '-':
					list[lex_index].type = subsym;
					strcpy(list[lex_index].name, "-");
					list[lex_index].value = 18;
					lex_index++;
					count++;
					break;
				case '+':
					list[lex_index].type = addsym;
					strcpy(list[lex_index].name, "+");
					list[lex_index].value = 17;
					lex_index++;
					count++;
					break;
				case ':':
					if(input[count+1] == '='){
						list[lex_index].type = assignsym;
						strcpy(list[lex_index].name, ":=");
						list[lex_index].value = 16;
						lex_index++;
						count+=2;
					}else{
						printlexerror(6);
						flag = 1;
					}
					break;
				case ' ':
					cutwhite(input);
					break;
			}
		}
		//if flag is set to error, return null
		if(flag == 1){
			return NULL;
		}		
	
	}
	
	if(printFlag != 0){
		printtokens();
	}
	
	return list;
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
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case odsym:
				printf("%11s\t%d", "od", odsym);
				break;
			case whensym:
				printf("%11s\t%d", "when", whensym);
				break;
			case elsedosym:
				printf("%11s\t%d", "elsedo", elsedosym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
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