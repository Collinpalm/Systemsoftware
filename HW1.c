#include <stdio.h>
#include <stdlib.h>

#define MAX_PAS_LENGTH 500
#define PUSH(stack, sp, value) stack[sp] = value

typedef struct Instruction Instruction;

struct Instruction {
	int OP, // Opcode
	L, // Lexicographical level
	M, // Depends on the operator
	lineNumber; // Line number of instruction from the input file
	char* opName; // name of opcode
};

// Function Declarations
int base(int L, int BP, int pas[]);


int main() {
	FILE * input = fopen("input.txt", "r");
	FILE * output = fopen("output.txt", "w");
	int pas [MAX_PAS_LENGTH];
	for (int i = 0; i < MAX_PAS_LENGTH; i++){
		pas[i] = 0;
	}

	int Halt = 0, PC = 0, SP = 0, BP = 0, IC = 0, DP = 0, GP = 0, FREE = 0;
	int numLines; 	// Holds how many lines there are in the file
	int numActivationRecords = 1;	// Variable to manage the printing of the stack
									// by keeping track of how many activation records
								 	// are open

	Instruction IR;


	// Read in the information from the file into mem
	for (; IC < MAX_PAS_LENGTH; IC+=3) {
		fscanf(input,"%d %d %d", &(pas[IC]), &(pas[IC+1]), &(pas[IC+2]));

		if (feof(input)) {
			numLines = IC;
			break;
		}
	}
	IC -= 3;
	//set up the process address space
	GP = IC;
	DP = IC-1;
	FREE = IC + 40;
	BP = IC;
	PC = 0;
	SP = MAX_PAS_LENGTH;

	// Close input file
	fclose(input);

	fprintf(output, "Line	OP    L    M\n");
	
	for (int i = 0; i < numLines; i+=3) {
		fprintf(output, "%d   	%s    %d    %d\n", i/3, IR.opName, pas[i+1], pas[i+2]);
	}
	
	fprintf(output, "\n                 PC    BP    SP	DP 	data\n");
	fprintf(output, "Initial values         %d     %d     %d	%d		\n", PC, BP, SP, DP);

	while (!Halt) {
		// Fetch
		IR.OP = pas[PC];
		IR.L = pas[PC+1];
		IR.M = pas[PC+2];
		IR.lineNumber = PC%3;
		switch (IR.OP) {
			case 1: // lit
				IR.opName = "lit";
			case 2: // opr
				IR.opName = "opr";
			case 3: // lod
				IR.opName = "lod";
			case 4: // sto
				IR.opName = "sto";
			case 5: // cal
				IR.opName = "cal";
			case 6: // inc
				IR.opName = "inc";
			case 7: // jmp
				IR.opName = "jmp";
			case 8: // jpc
				IR.opName = "jpc"; 
			case 9: // sio
				IR.opName = "sio";
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
				}
				break;
			case 2: // OPR 0, M Operation to be performed on the data at the top of the stack
				switch (IR.M) {
					case 0: // RET
						SP = BP + 1;
						BP = pas[SP - 2];
						PC = pas[SP - 3];
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
							SP = SP+2;
							pas[SP] = pas[SP] * pas[SP-1];
						}
						break;
					case 5: // DIV
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] / pas[DP+1];
						}else{
							SP = SP+2;
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
							SP = SP+2;
							pas[SP] = pas[SP] % pas[SP-1];
						}
						break;
					case 8: // EQL
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] == pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] == pas[SP-1];
						}
						break;
					case 9: // NEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] != pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] != pas[SP-1];
						}
					case 10: // LSS
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] < pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] < pas[SP-1];
						}
					case 11: // LEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] <= pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] <= pas[SP-1];
						}
					case 12: // GTR
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] > pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] > pas[SP-1];
						}
					case 13: //GEQ
						if(BP==GP){
							DP = DP-1;
							pas[DP] = pas[DP] >= pas[DP+1];
						}else{
							SP = SP+2;
							pas[SP] = pas[SP] >= pas[SP-1];
						}
						break;
				}
				break;
			case 3: // LOD L, M Load value to top of the stack from the stack location at offset M
					// from L lexicographical levels down
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
			case 4: // STO L, M Store value at top of the stack at
					// offset M from L lexicographical levels down
				if(BP==GP){
					pas[GP+IR.M] = pas[DP];
					DP = DP-1;
			    }else{
					int ba = base(IR.L, BP, pas);
					if(ba==GP){
						pas[GP+IR.M] = pas[SP];
						SP = SP - 1;
					}else{
						pas[ba - IR.M] = pas[SP];
						SP = SP + 1;
					}
				}
				break;
			case 5: // CAL L, M Call procedure at code index M (generates new Activation Record)
				pas[SP-1] = base(IR.L, BP, pas);
				pas[SP-2] = BP;
				pas[SP-3] = PC;
				BP = SP - 1;
				PC = IR.M;
				break;
			case 6: // INC 0, M Allocate M locals (increment sp by M)
				if(BP == GP){
					DP = DP + IR.M;
				}else{
					SP = SP - IR.M;
				}
				break;
			case 7: // JMP 0, M Jump to instruction M
				PC = IR.M;
				break;
			case 8: // JPC Jump to instruction M if top stack element is 0
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
			case 9: // SIO 0, 1 Write the top stack element to the screen
				switch(IR.M){
					case 1:
						if(BP==GP){
							printf("%d", pas[DP]);
							DP = DP - 1;
						}else{
							printf("%d", pas[SP]);
							SP = SP + 1;
						}
						break;
					case 2:
						if(BP==GP){
							DP = DP + 1;
							//scanf("%d", &pas[DP]);
							printf("scanf is cringe");
						}else{
							SP = SP - 1;
							//ddddddscanf("%d", &pas[SP]);
							printf("scanf is cringe");
						}
						break;
					case 3:
						Halt = 1;
						break;
				}
				break;
		}

		fprintf(output, "%d	%s	%d	%d	%d	%d	%d	%d", IR.lineNumber, IR.opName, IR.L, IR.M, PC, BP, SP, DP);
		/*
		if (!Halt) { // If the program is ended the stack should not be printed
			if (numActivationRecords > 1){
                for (int i = 1; i < numActivationRecords; i++) {
					for (int j = base(i, BP, pas); j >= SP; j--) {
						fprintf(output, "%d ", pas[j]);
					}

					// Was never able to get the "|" to
				 // to be printed in the correct spot
				}
			}else{
				for (int j = BP; j >= SP; j--) {
					fprintf(output, "%d ", pas[j]);
				}
			}
		}
		*/

		fprintf(output, "\n");
	}	

	//Closes output file
	fclose(output);


	return 0;
}

void print_execution(int line, Instruction IR, int PC, int BP, int SP, int DP, int *pas, int GP){
	int i;
	//print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, IR.opName, IR.L, IR.M, PC, BP, SP, DP);

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
	int b1 = BP;

	while (L > 0) {
		b1 = pas[b1];
		L--;
	}

	return b1;
}