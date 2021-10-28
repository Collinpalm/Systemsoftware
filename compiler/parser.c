#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int level;
int lIndex;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();


int find_sym(lexeme *list, int kind){

}
void mar(){

}
void factor(lexeme *list){

}
void term(lexeme *list){

}
void expression(lexeme *list){
	if(list[lIndex].type == subsym){
		lIndex++;
		term(list);
		emit(2,0,1);
		while(list[lIndex].type == (addsym || subsym)){
			if(list[lIndex].type == addsym){
				lIndex++;
			}
		}
	}
}
void condition(lexeme *list){
	if(list[lIndex].type == oddsym){
		lIndex++;
		expression(list);
		emit(2,0,6);
	}else{
		expression(list);
		if(list[lIndex].type == eqlsym){
			lIndex++;
			expression(list);
			emit(2, 0, 8);
		}else if(list[lIndex].type == neqsym){
			lIndex++;
			expression(list);
			emit(2, 0, 9);
		}else if(list[lIndex].type == lsssym){
			lIndex++;
			expression(list);
			emit(2, 0, 10);
		}else if(list[lIndex].type == leqsym){
			lIndex++;
			expression(list);
			emit(2, 0, 11);
		}else if(list[lIndex].type == gtrsym){
			lIndex++;
			expression(list);
			emit(2, 0, 12);
		}else if(list[lIndex].type == geqsym){
			lIndex++;
			expression(list);
			emit(2, 0, 13);
		}else{
			printparseerror(19);
		}
	}
}
void statement(lexeme *list){
	if(list[lIndex].type == identsym){
		int symIdx = find_sym(list, 2);
		if(symIdx == -1){
			if(find_sym(list, 1) != find_sym(list, 3)){
				printparseerror(16);
			}else{
				printparseerror(17);
			}
		}
		lIndex++;
		if(list[lIndex].type != assignsym){
			printparseerror(18);
		}
		lIndex++;
		expression(list);
		emit(4,level-table[symIdx].level, table[symIdx].addr);
		return;		
	}
	if(list[lIndex].type == beginsym){
		do{
			lIndex++;
			statement(list);
		}while(list[lIndex].type == commasym);
		if(list[lIndex].type != endsym){
			if(list[lIndex].type == (identsym || beginsym || ifsym || whilesym || readsym || writesym || callsym)){
				printparseerror(19);
			}else{
				printparseerror(19);
			}
		}
		lIndex++;
		return;
	}
	if(list[lIndex].type == ifsym){
		lIndex++;
		condition(list);
		int jpcIdx = cIndex;
		emit(8, 0, jpcIdx);
		if(list[lIndex].type != thensym){
			printparseerror(19);
		}
		lIndex++;
		statement(list);
		if(list[lIndex].type == elsesym){
			int jmpIdx = cIndex;
			emit(7, 0, jmpIdx);
			code[jmpIdx].m = cIndex*3;
			statement(list);
			code[jmpIdx].m = cIndex*3;
		}else{
			code[jpcIdx].m = cIndex*3;
		}
		return;
	}
	if(list[lIndex].type == whilesym){
		lIndex++;
		int loopIdx = cIndex;
		condition(list);
		if(list[lIndex].type!=dosym){
			printparseerror(19);
		}
		lIndex++;
		int jpcIdx = cIndex;
		emit(8, 0, 0);
		statement(list);
		emit(7, 0, loopIdx*3);
		code[jpcIdx].m = cIndex*3;
		return;
	}
	if(list[lIndex].type == readsym){
		lIndex++;
		if(list[lIndex].type != identsym){
			printparseerror(19);
		}
		int symIdx = find_sym(list, 2);
		if(symIdx == -1){
			if(find_sym(list, 1) == find_sym(list, 3)){
				printparseerror(3);
			}else{
				printparseerror(4);
			}
		}
		lIndex++;
		//emit read, but IDK how to do thatemit()
		emit(4,level-table[symIdx].level, table[symIdx].addr);
		return;
	}
	if(list[lIndex].type == writesym){
		lIndex++;
		expression(list);
		//emit write emit()
		return;
	}
	if(list[lIndex].type == callsym){
		lIndex++;
		int symIdx = find_sym(list, 3);
		if(symIdx == -1){
			if(find_sym(list, 1) == find_sym(list, 3)){
				printparseerror(3);
			}else{
				printparseerror(4);
			}
		}
		lIndex++;
		emit(5, level-table[symIdx].level, symIdx);
	}
}
void proc_dec(lexeme *list){
	while(list[lIndex].type == procsym){
		lIndex++;
		if(list[lIndex].type != identsym){
			printparseerror(12);
		}
		int symidx = mult_dec(list[lIndex]);
		if(symidx != -1){
			printparseerror(13);
		}
		addToSymbolTable(3, list[lIndex].name, 0, level, 0, 0);
		lIndex++;
		if(list[lIndex].type == semicolonsym){
			printparseerror(14);
		}
		lIndex++;
		block(list);
		if(list[lIndex].type != semicolonsym){
			printparseerror(15);
		}
		lIndex++;
		emit(2,0,0);
	}
}
int var_dec(lexeme *list){
	int numVars = 0;
	if(list[lIndex].type == varsym){
		do{
			numVars++;
			lIndex++;
			if(list[lIndex].type == varsym){
				printparseerror(8);
			}
			int symidx = mult_dec(list[lIndex]);
			if(symidx != -1){
				printparseerror(9);
			}
			if(level==0){
				addToSymbolTable(2, list[lIndex].name, 0, level, numVars-1, 0);
			}else{
				addToSymbolTable(2, list[lIndex].name, 0, level, numVars+2, 0);
			}
			lIndex++;
		}while (list[lIndex].type == commasym);
		if(list[lIndex].type != semicolonsym){
			if(list[lIndex].type == identsym){
				printparseerror(10);
			}else{
				printparseerror(11);
			}
		}
		lIndex++;
	}
	return numVars;
}
void const_dec(lexeme *list){
	if(list[lIndex].type == constsym){
		do{
			lIndex++;
			if(list[lIndex].type != identsym){
				printparseerror(2);
			}
			int symindex = mult_dec(list[lIndex]);
			if(symindex != -1){
				printparseerror(3);
			}
			addToSymbolTable(1, list[lIndex].name, list[lIndex].value, level, tIndex, 0);
			tIndex++;
			lIndex++;
			if(list[lIndex].type != assignsym){
				printparseerror(4);
			}
			lIndex++;
			if(list[lIndex].type != numbersym){
				printparseerror(5);
			}
			addToSymbolTable(1, list[lIndex].name, list[lIndex].value, level, tIndex, 0);
			tIndex++;
			lIndex++;
		}while(list[lIndex].type == commasym);
		lIndex++;
		if(list[lIndex].type != semicolonsym){
			if(list[lIndex].type == identsym){
				printparseerror(6);
			}else{
				printparseerror(7);
			}
		}
		lIndex++;
	}
}
void block(lexeme *list){
	level += 1;
	int procedureIndex = tIndex-1;
	const_dec(list);
	int x = var_dec(list);
	proc_dec(list);
	table[procedureIndex].addr = lIndex*3;
	if (level == 0){
		emit(6,0,x);
	}else{
		emit(6,0,x+3);
	}
	statement(list);
	mark();
	level-=1;
}
void program(lexeme *list){
	lIndex = 0;
	emit(7, 0, 0);
	addToSymbolTable(3, "main", 0, 0, 0, 0);
	level = -1;
	block(list);
	if(list[lIndex].type != periodsym){
		printparseerror(1);
	}
	emit(9,0,3);
	/*
	for(int i = 0;i < sizeof(code);i++){
		if(code[i].opcode ){

		}
	}*/
	code[0].m = table[0].addr;
}


instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = NULL;
	code[cIndex].opcode = -1;

	return code;
}


void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}


void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}