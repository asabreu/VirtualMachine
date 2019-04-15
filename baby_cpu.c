#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// ANSI colours
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define BOLD    "\033[1m"

// registers
unsigned int r0;
unsigned int r1;
unsigned int r2;
unsigned int r3;
unsigned int overflow;
unsigned int program_counter;

void dump_registers(){
    
    printf(RED"\nRegister Dump:"RESET"\n\nR0 : 0x%08x\nR1 : 0x%08x\nR2 : 0x%08x\nR3 : 0x%08x\nPC : 0x%08x\n",r0,r1,r2,r3,program_counter);
}

int execute_instruction(unsigned char opcode, unsigned char operand1, unsigned char operand2, unsigned char operand3){
    
    // detect which opcode
    switch (opcode){
            // ADD
        case 0x01:
            //printf("ADD (0x01) opcode detected\n");
            // new case statement for destination register
            switch (operand1){
                case 0x00:
                    // r0
                    r0 = operand2 + operand3;
                    printf(CYAN BOLD"ADD R0, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                case 0x01:
                    // r1
                    r1 = operand2 + operand3;
                    printf(CYAN BOLD"ADD R1, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                case 0x02:
                    // r2
                    r2 = operand2 + operand3;
                    printf(CYAN BOLD"ADD R2, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                default:
                    // UNKNOWN
                    printf(RED BOLD"ERROR: UNKNOWN REGISTER\n");
                    break;
            }
            break;
            // SUB
        case 0x02:
            //printf("SUB (0x02) opcode detected\n");
            // new case statement for destination register
            switch (operand1){
                case 0x00:
                    // r0
                    r0 = operand2 - operand3;
                    printf(CYAN BOLD"SUB R0, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                case 0x01:
                    // r1
                    r1 = operand2 - operand3;
                    printf(CYAN BOLD"SUB R1, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                case 0x02:
                    // r2
                    r2 = operand2 - operand3;
                    printf(CYAN BOLD"SUB R2, 0x%02x, 0x%02x\n",operand2,operand3);
                    break;
                default:
                    // UNKNOWN
                    printf(RED BOLD"ERROR: UNKNOWN REGISTER\n");
                    break;
            }
            break;
            // MOV
        case 0x03:
            switch (operand1){
                case 0x00:
                    // r0
                    r0 = operand2;
                    printf(CYAN BOLD"MOV R0, 0x%02x\n",operand2);
                    break;
                case 0x01:
                    // r1
                    r1 = operand2;
                    printf(CYAN BOLD"MOV R1, 0x%02x\n",operand2);
                    break;
                case 0x02:
                    // r2
                    r2 = operand2;
                    printf(CYAN BOLD"MOV R2, 0x%02x\n",operand2);
                    break;
                case 0x03:
                    // r3
                    r3 = operand2;
                    printf(CYAN BOLD"MOV R3, 0x%02x\n",operand2);
                    break;
                default:
                    // UNKNOWN
                    printf(RED BOLD"ERROR: UNKNOWN REGISTER\n");
                    break;
            }
            break;
        default:
            printf(RED BOLD"ERROR: UNKNOWN INSTRUCTION\n");
            break;
    }
    return 0;
    
}

void execute(int instructions[], int size){
    
    // instruction dissection
    unsigned char opcode;
    unsigned char operand1;
    unsigned char operand2;
    unsigned char operand3;
    
    // iterate over the instructions by incrementing PC (starts at 0 and increments by 1 every iteration)
    for (program_counter = 0; program_counter < size;program_counter++){
        // get 3 least significant bytes (operands)
        operand1 = (instructions[program_counter] >> 16) & 0xff;
        operand2 = (instructions[program_counter] >> 8) & 0xff;
        operand3 = instructions[program_counter] & 0xff;

        // get most significant byte (opcode)
        opcode = (instructions[program_counter] >> 24) & 0xff;
        
        execute_instruction(opcode,operand1,operand2,operand3);
        
        //printf("%02x\n",opcode);
    }
    printf(RESET BOLD"EXECUTION FINISHED\n"RESET);
}

void reset_cpu(){
    r0 = 0;
    r1 = 0;
    r2 = 0;
    r3 = 0;
    overflow = 0;
    program_counter = 0;
}

int main(){
    
    // clear register values
    reset_cpu();
    
    /* this is the program we want to execute:
       instruction set currently very limited, supporting ADD, SUB, MOV
       registers can only be used as the destination operand (first operand)
     
       instructions are represented *very* simply (for now)
       first byte (MSB) determines the opcode
       second byte determines destination register
       third byte determines first argument
       four byte (LSB) determines second argument (sometimes optional)
     */
    int instructions[] = {
        0x010001FE, // ADD R0, 0x1, 0xFE
        0x01014241, // ADD R1, 0x42, 0x41
        0x0202990F, // SUB R2, 0x99, 0x0F
        0x0303AA00, // MOV R3, 0xAA
    };

    printf("Program size: "BOLD GREEN"0x%lx\n"RESET,sizeof(instructions)/4);
    
    printf(BOLD"\nExecuting...\n");
    execute(instructions,sizeof(instructions)/4);
    
    // for debugging purposes
    dump_registers();
    
    return 0;
}
