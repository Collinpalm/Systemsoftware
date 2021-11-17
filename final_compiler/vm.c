/*
Collin Palm and Ayden Rebhan

COP 3402 
FALL 2021 UCF
PROFESSOR= EURIPIDES MONTAGNE

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define MAX_PAS_LENGTH 500

typedef struct Instruction Instruction;

struct Instruction {
	int OP, // Opcode
	L, // Lexicographical level
	M,
	linecount;
	char opName[3]; // name of opcode
};

// Function Declarations
int base(int L, int BP, int pas[]);
void print_execution(int line, Instruction *IR, int PC, int BP, int SP, int DP, int *pas, int GP);

void execute_program(instruction* code, int printFlag) {
	int pas [MAX_PAS_LENGTH];
	for (int i = 0; i < MAX_PAS_LENGTH; i++){
		pas[i] = 0;
	}

	int Halt = 0, PC = 0, SP = 0, BP = 0, IC = 0, DP = 0, GP = 0, FREE = 0;
	int numLines = 0; 	// Holds how many lines there are in the file
	int codeLen = sizeof(code)/sizeof(instruction);
	Instruction IR;
	//load the program into pas
	while(IC < codeLen){
		pas[IC] = code[numLines].opcode;
		pas[IC+1] = code[numLines].l;
		pas[IC+2] = code[numLines].m;
		IC+=3;
		numLines++;
	}
	
	
	//set up the process address space
	GP = IC;
	DP = IC-1;
	FREE = IC + 40;
	BP = IC;
	PC = 0;
	SP = MAX_PAS_LENGTH;

	//set up boilerplate output
	if(printFlag != 0){
		printf("\n            \t\tPC\tBP\tSP\tDP\tdata\n");
		printf("Initial values\t\t%d\t%d\t%d\t%d		\n", PC, BP, SP, DP);
	}
	
	
	while (Halt == 0) {
		
		// Fetch
		IR.OP = pas[PC];
		IR.L = pas[PC+1];
		IR.M = pas[PC+2];
		IR.linecount = PC/3;
		switch (IR.OP) {
			case 1: // lit
				strcpy(IR.opName, "lit");
				break;
			case 2: // opr
				switch(IR.M){
					case 0:
						strcpy(IR.opName, "rtn");
						break;
					case 1:
						strcpy(IR.opName, "neg");
						break;
					case 2:
						strcpy(IR.opName, "add");
						break;
					case 3:
						strcpy(IR.opName, "sub");
						break;
					case 4:
						strcpy(IR.opName, "mul");
						break;
					case 5:
						strcpy(IR.opName, "div");
						break;
					case 6:
						strcpy(IR.opName, "odd");
						break;
					case 7:
						strcpy(IR.opName, "mod");
						break;
					case 8:
						strcpy(IR.opName, "eql");
						break;
					case 9:
						strcpy(IR.opName, "neq");
						break;
					case 10:
						strcpy(IR.opName, "lss");
						break;
					case 11:
						strcpy(IR.opName, "leq");
						break;
					case 12:
						strcpy(IR.opName, "gtr");
						break;
					case 13:
						strcpy(IR.opName, "geq");
						break;
				}
				break;
			case 3: // lod
				strcpy(IR.opName, "lod");
				break;
			case 4: // sto
				strcpy(IR.opName, "sto");
				break;
			case 5: // cal
				strcpy(IR.opName, "cal");
				break;
			case 6: // inc
				strcpy(IR.opName, "inc");
				break;
			case 7: // jmp
				strcpy(IR.opName, "jmp");
				break;
			case 8: // jpc
				strcpy(IR.opName, "jpc"); 
				break;
			case 9: // sio
				strcpy(IR.opName, "sys");
				break;
			default:
				break;
		}
		PC+=3; // Increase the Program Counter

		// Execute
		switch (IR.OP) {
			case 1: // LIT 0, M Push constant value M into the stack
				if(BP == GP){
					DP++;
					pas[DP] = IR.M;
				}else{
					SP = SP - 1;
					pas[SP] = IR.M;
				}
				break;
			case 2: // OPR 0, M Operation to be performed on the data at the top of the stack
				switch (IR.M) {
					case 0: // RET
						SP = BP + 1;
						BP = pas[SP - 3];
						PC = pas[SP - 4];
						break;
					case 1: // NEG
						if(BP == GP){
							pas[BP] = -1 * pas[BP];
						}else{
							pas[SP] = -1 * pas[SP];
						}
						break;
					case 2: // ADD
						if(BP == GP){
							DP = DP - 1;
							pas[DP] = pas[DP] + pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] + pas[SP-1];
						}
						break;
					case 3: // SUB
						if(BP==GP){
							DP = DP - 1;
							pas[SP] = pas[DP] - pas[DP+1];
						}else{
							SP = SP +1;
							pas[SP] = pas[SP] - pas[SP-1];
						}
						break;
					case 4: // MUL
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] * pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] * pas[SP-1];
						}
						break;
					case 5: // DIV
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] / pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] / pas[SP-1];
						}
						break;
					case 6: // ODD
						if(BP==GP){
							pas[DP] = pas[DP] % 2;
						}else{
							pas[SP] = pas[SP] % 2;
						}
						break;
					case 7: // MOD
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] % pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] % pas[SP-1];
						}
						break;
					case 8: // EQL
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] == pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] == pas[SP-1];
						}
						break;
					case 9: // NEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] != pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] != pas[SP-1];
						}
						break;
					case 10: // LSS
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] < pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] < pas[SP-1];
						}
						break;
					case 11: // LEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] <= pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] <= pas[SP-1];
						}
						break;
					case 12: // GTR
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] > pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] > pas[SP-1];
						}
						break;
					case 13: //GEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] >= pas[DP+1];
						}else{
							SP = SP+1;
							pas[SP] = pas[SP] >= pas[SP-1];
						}
						break;
				}
				break;
			case 3: // LOD 
				if(BP==GP){
					DP = DP+1;
					pas[DP] = pas[GP+IR.M];
			    }else{
					int ba = base(IR.L, BP, pas);
					if(ba==GP){
						SP = SP -1;
						pas[SP] = pas[GP + IR.M];
					}else{
						SP = SP - 1;
						pas[SP] = pas[ba-IR.M];
					}
				}
				break;
			case 4: // STO 
				if(BP==GP){
					pas[GP+IR.M] = pas[DP];
					DP = DP-1;
			    }else{
					int ba = base(IR.L, BP, pas);
					if(ba==GP){
						pas[GP+IR.M] = pas[SP];
						SP = SP + 1;
					}else{
						pas[ba - IR.M] = pas[SP];
						SP = SP + 1;
					}
				}
				break;
			case 5: // CAL 
				pas[SP-1] = 0;
				pas[SP-2] = base(IR.L, BP, pas);;
				pas[SP-3] = BP;
				pas[SP-4] = PC;
				BP = SP - 1;
				PC = IR.M;
				break;
			case 6: // INC 
				if(BP == GP){
					DP = DP + IR.M;
				}else{
					SP = SP - IR.M;
				}
				break;
			case 7: // JMP 
				PC = IR.M;
				break;
			case 8: // JPC
				if(BP==GP){
					if(pas[DP] == 0){
						PC = IR.M;
					}
					DP = DP-1;
				}else{
					if(pas[SP] == 0){
						PC = IR.M;
					}
					SP = SP+1;
				}
				break;
			case 9: // SIO 
				switch(IR.M){
					case 1:
						if(BP==GP){
							printf("Top of Stack Value: %d\n", pas[DP]);
							DP = DP - 1;
						}else{
							printf("Top of Stack Value: %d\n", pas[SP]);
							SP = SP + 1;
						}
						break;
					case 2:
						if(BP==GP){
							DP = DP + 1;
							printf("Please Enter an Integer: ");
							scanf("%d", &pas[DP]);
						}else{
							SP = SP - 1;
							printf("Please Enter an Integer: ");
							scanf("%d", &pas[SP]);
						}
						break;
					case 3:
						Halt = 1;
						break;
				}
				break;
		}
		//print function
		if(printFlag != 0){
			print_execution(IR.linecount, &IR, PC, BP, SP, DP, pas, GP);
		}
		

		
	}	



	return 0;
}

void print_execution(int line, Instruction *IR, int PC, int BP, int SP, int DP, int *pas, int GP){
	int i;
	//print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, IR->opName, IR->L, IR->M, PC, BP, SP, DP);

	//print data
	for(i=GP;i<=DP;i++){
		printf("%d ", pas[i]);
	}
	printf("\n");
	
	//print stack
	printf("\tstack : ");
	for(i=MAX_PAS_LENGTH-1;i>=SP;i--){
		printf("%d ", pas[i]);
	}
	printf("\n");
}


int base(int L, int BP, int pas[]) {
	int arb = BP;

	while (L > 0) {
		arb = pas[arb];
		L--;
	}

	return arb;
}