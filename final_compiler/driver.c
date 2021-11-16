/*
	This is the driver for the UCF Fall 2021 Systems Software Project
	If you choose to alter this, you MUST make a note of that in your
	readme file, otherwise you will lose 5 points.
*/

#include <stdlib.h>
#include <stdio.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 3000

int main(int argc, char *argv[])
{
	FILE *ifp;
	char *input;
	char c;
	lexeme *list;
	instruction *code;
	int i;
	int tokens = 0, symbols = 0, codes = 0, outputs = 0;
	
	if (argc < 2)
	{
		printf("Error : please include the file name\n");
		return 0;
	}
	
	ifp = fopen(argv[1], "r");
	input = malloc(MAX_CODE_LENGTH * sizeof(char));
	i = 0;
	
	c = fgetc(ifp);
	while (1)
	{
		input[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	input[i] = '\0';
	
	for (i = 2; i < argc; i++)
	{
		if (argv[i][1] == 'l')
			tokens = 1;
		else if (argv[i][1] == 's')
			symbols = 1;
		else if (argv[i][1] == 'a')
			codes = 1;
		else if (argv[i][1] == 'v')
			outputs = 1;
		else
		{
			printf("Error : unrecognized directive\n");
			free(input);
			return 0;
		}
	}
	
	list = lexanalyzer(input, tokens);
	if (list == NULL)
	{
		free(input);
		return 0;
	}
	
	code = parse(list, symbols, codes);
	if (code == NULL)
	{
	 	free(input);
	 	free(list);
	 	return 0;
	}
	
	execute_program(code, outputs);
	
	free(input);
	free(list);
	free(code);
	return 0;
}