/*Michael Frenk and Rebecca Cannon*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_pas_LENGTH 500

int pas[MAX_pas_LENGTH];
int IC = 0;
int base(int L, int BP); 					//function that he gave us
											//modified to include BP as a pointer because
											//I was struggling to get BP to work as a global var
void print_execution(int line, char *opname, int L, int M, int PC, int BP, int SP, int DP, int *pas, int GP);
											//function that he gave us
int main(int argc, char **argv) {
	
	FILE* input = fopen(argv[1], "r");
	
	int OP, L, M;
	
	while (!feof(input)) { 												//begin while 				
		fscanf(input, "%d %d %d", &pas[IC], &pas[IC+1], &pas[IC+2]);	//scans all of the info from the input file into the PAS array 
																		//I did not use a stack. All of the info is stored as it was read in
																		//so it is all in sequential order in the PAS
		IC+=3;
	} 																	//end while
	
	//these variables have to be initialized AFTER the info from the input is read
	//depends on the value of the IC. The IC changes as the input is read into the PAS. 
	int GP = IC;					//pts to data segment
	int DP = IC-1;  				//for variables in main
	int FREE = IC+40; 				//pts to heap
	int BP = IC;
	int PC = 0;
	int SP = MAX_pas_LENGTH;
	int halt = 1;
	int line = PC;
	
	//I could of just done opcode[4]; and the initalized them differently in the switch.
	//but I thought of that after I did strcpy
	char opcode[4] = "OPP";
	
	//sets up the beginning of the output
	//he checks our code against diff -w so while the white space matches to my personal formatting it shouldnt matter
	printf("\t\t\t\t%s\t%s\t%s\t%s\t%s", "PC", "BP", "SP", "DP", "data");
	printf("\nInitial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, DP);
	
	while (PC <= IC && halt == 1) { //begin while loop
		//fetch stuffz 
		OP = pas[PC]; //gets the first value of the PAS
		L = pas[PC+1]; //gets the following value
		M = pas[PC+2]; //...
		PC+=3; //VERY IMPORTANT the PC has to be incremented right after getting the info for the OP, L, M. 
		
		//execute stuffz
		
		//all of this is written directly from the pseudocode that he gave us
		//the reason for asking if bp==gp is to determine if we want the values to be pulled from data or stack
		switch (OP) {
			case 1: 				//LIT
				strcpy(opcode, "LIT");
				if (BP==GP) {
					DP+=1;
					pas[DP] = M;
				} else {
					SP-=1;
					pas[SP] = M;
				}
				break;
			case 2: 				//OPR -- further swithces (rtn, neg, add, sub, mul, div, odd, mod, eql, neq, lss, LEQ, GTR, GEQ)
				if (M == 0) {
					strcpy(opcode, "RTN");
					SP = BP+1;
					BP = pas[SP-2];
					PC = pas[SP-3];
				}
				else if(M == 1) {
					strcpy(opcode, "NEG");
					if (BP == GP) {
						pas[DP]*=-1;
					} else {
						pas[SP]*=-1;
					}
				} else if (M == 2) {
					strcpy(opcode, "ADD");
					if (BP == GP) {
						DP-=1;
						pas[DP]+=pas[DP+1];
					} else {
						SP+=1;
						pas[SP]+=pas[SP-1];
					}
				} else if (M == 3) {
					strcpy(opcode, "SUB");
					if (BP == GP) {
						DP-=1;
						pas[DP]-=pas[DP+1];
					} else {
						SP+=1;
						pas[SP]-=pas[SP-1];
					}
				} else if (M == 4) {
					strcpy(opcode, "MUL");
					if (BP == GP) {
						DP-=1;
						pas[DP]*=pas[DP+1];
					} else {
						SP+=1;
						pas[SP]*=pas[SP-1];
					}
				} else if (M == 5) {
					strcpy(opcode, "DIV");
					if (BP == GP) {
						DP-=1;
						pas[DP]/=pas[DP+1];
					} else {
						SP+=1;
						pas[SP]/=pas[SP-1];
					}
				} else if (M == 6) {
					strcpy(opcode, "ODD");
					if (BP == GP) {
						pas[DP]=pas[DP]%2;
					} else {
						pas[SP]=pas[SP]%2;
					}
				} else if (M == 7) {
					strcpy(opcode, "MOD");
					if (BP == GP) {
						DP-=1;
						pas[DP]= pas[DP]%(pas[DP+1]);
					} else {
						SP+=1;
						pas[SP]=pas[SP]%pas[SP-1];
					}
				} else if (M == 8) {
					strcpy(opcode, "EQL");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] == pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] == pas[SP-1]);
					}
				} else if (M == 9) {
					strcpy(opcode, "NEQ");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] != pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] != pas[SP-1]);
					}
				} else if (M == 10) {
					strcpy(opcode, "LSS");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] < pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] < pas[SP-1]);
					}
				} else if (M == 11) {
					strcpy(opcode, "LEQ");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] <= pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] <= pas[SP-1]);
					}
				} else if (M == 12) {
					strcpy(opcode, "GTR");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] > pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] > pas[SP-1]);
					}
				} else if (M == 13) {
					strcpy(opcode, "GEQ");
					if (BP == GP) {
						DP-=1;
						pas[DP] = (pas[DP] >= pas[DP+1]);
					} else {
						SP+=1;
						pas[SP] = (pas[SP] >= pas[SP-1]);
					}
				}
				break;
			case 3: 				//LOD
				strcpy(opcode, "LOD");
				if (BP==GP) {
					DP+=1;
					pas[DP] = pas[GP+M];
				} else {
					if (base(L, BP) == GP) {
						SP-=1;
						pas[SP] = pas[GP+M];
					} else {
						SP-=1;
						pas[SP]	= pas[base(L, BP) -M];
					}
				}
				break;
			case 4: 				//STO
				strcpy(opcode, "STO");
				if (BP==GP) {
					pas[GP+M] = pas[DP];
					DP-=1;
				} else {
					if (base(L, BP) == GP) {
						pas[GP+M] = pas[SP];
						SP+=1;
					} else {
						pas[base(L, BP) - M] = pas[SP];
						SP+=1;
					}
				}
				break;
			case 5: 				//CAL
				strcpy(opcode, "CAL");
				pas[SP-1] = base(L, BP);
				pas[SP-2] = BP;
				pas[SP-3] = PC;
				BP = SP-1;
				PC = M;
				break;
			case 6: 				//INC
				strcpy(opcode, "INC");
				if (BP == GP) {
					DP += M;
				}
				else {
					SP-=M;
				}
				break;
			case 7: 				//JMP
				strcpy(opcode, "JMP");
				PC = M;
				break;
			case 8: 				//JPC    
				strcpy(opcode, "JPC");
				if (BP == GP) {
					if (pas[DP] == 0) {
						PC = M;
					} 
					DP-=1; 
				} else {
					if (pas[SP] == 0)
						PC = M;
					SP+=1;
				}	
				break;
			case 9: 				//SYS -- has further switches
				strcpy(opcode, "SYS");
				if (M == 1) {
					if (BP == GP) {
						printf("Top of Stack Value: %d\n", pas[DP]);
						DP-=1;
					} else {
						printf("Top of Stack Value: %d\n", pas[SP]);
						SP+=1;
					}
					
				} else if (M == 2) {
					if (BP == GP) {
						DP+=1;
						printf("Please enter an Integer: ");
						scanf("%d", &pas[DP]);
						//printf("%d\n", pas[DP]);
					} else {
						SP-=1;
						printf("Please enter an Integer: ");
						scanf("%d", &pas[SP]);
						//printf("%d\n", pas[SP]);
					}
				} else if (M == 3) {
					//end program
					halt = 0;
				}
				break;
			//just a funny line of code that I left in because I hate a switch without a default
			default: printf("this better never run"); 
		}
			
		//line is divided by three because it is set to PC, and PC points to the op, l, and m which are three values for one line of Instruction
		//however you cant just do pc/3 because the value of pc is used throughout the switch
		print_execution(line/3, opcode, L, M, PC, BP, SP, DP, pas, GP);
		//line is then reset to equalling pc because pc wont be messed with until the next call
		//you cant just do line+=3 because of the jmp and whatnot. the pc wouldve incrememnted/decremented more than 3
		line=PC;
	}
	return 0;
	
}

//function that he gave us
int base(int L, int BP){
	
    int arb = BP; // arb = activation record base
    while (L > 0) // find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}


//function that he gave us
void print_execution(int line, char *opname, int L, int M, int PC, int BP, int SP, int DP, int *pas, int GP) {
	
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, L, M, PC, BP, SP, DP);

	// print data section
	for (i = GP; i <= DP; i++)
		printf("%d ", pas[i]);
	printf("\n");

	// print stack
	printf("\tstack : ");
	for (i = MAX_pas_LENGTH - 1; i >= SP; i--)
		printf("%d ", pas[i]);
	printf("\n");
}