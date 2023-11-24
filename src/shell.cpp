/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   For TGGS Computer Architecture                            */
/*                                                             */
/*   Credit: ECE 447                                           */
/*   Carnegie Mellon University                                */
/*                                                             */
/***************************************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*          DO NOT MODIFY THIS FILE!                            */
/*          You should only change sim.c!                       */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "shell.h"
#include "dram.h"

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/

#define MEM_DATA_START  0x10000000
#define MEM_DATA_SIZE   0x00100000
#define MEM_TEXT_START  0x00400000
#define MEM_TEXT_SIZE   0x00100000
#define MEM_STACK_START 0x7ff00000
#define MEM_STACK_SIZE  0x00100000
#define MEM_KDATA_START 0x90000000
#define MEM_KDATA_SIZE  0x00100000
#define MEM_KTEXT_START 0x80000000
#define MEM_KTEXT_SIZE  0x00100000

typedef struct {
    uint32_t start, size;
    uint8_t *mem;
} mem_region_t;

/* memory will be dynamically allocated at initialization */
mem_region_t MEM_REGIONS[] = {
    { MEM_TEXT_START, MEM_TEXT_SIZE, NULL },
    { MEM_DATA_START, MEM_DATA_SIZE, NULL },
    { MEM_STACK_START, MEM_STACK_SIZE, NULL },
    { MEM_KDATA_START, MEM_KDATA_SIZE, NULL },
    { MEM_KTEXT_START, MEM_KTEXT_SIZE, NULL }
};

#define MEM_NREGIONS (sizeof(MEM_REGIONS)/sizeof(mem_region_t))

/***************************************************************/
/* CPU State info.                                             */
/***************************************************************/

CPU_State CURRENT_STATE, NEXT_STATE;
int RUN_BIT;	/* run bit */
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure: mem_read_32                                      */
/*                                                             */
/* Purpose: Read a 32-bit word from memory                     */
/*                                                             */
/***************************************************************/
uint32_t mem_read_32(uint32_t address)
{
    int i;
    //loop over number of regions in memory
    for (i = 0; i < MEM_NREGIONS; i++) {
        //condition to check if the address is in region i then read from that region
        if (address >= MEM_REGIONS[i].start &&
                address < (MEM_REGIONS[i].start + MEM_REGIONS[i].size)) {
            uint32_t offset = address - MEM_REGIONS[i].start;

            return
                (MEM_REGIONS[i].mem[offset+3] << 24) |
                (MEM_REGIONS[i].mem[offset+2] << 16) |
                (MEM_REGIONS[i].mem[offset+1] <<  8) |
                (MEM_REGIONS[i].mem[offset+0] <<  0);
        }
    }

    return 0;
}

void reset()
{
  CURRENT_STATE.HI = 0;
  CURRENT_STATE.LO = 0;
  for (int i = 0; i < 32; i++)
  {
    CURRENT_STATE.REGS[i] = 0;
  }
  INSTRUCTION_COUNT = 0;
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure: mem_write_32                                     */
/*                                                             */
/* Purpose: Write a 32-bit word to memory                      */
/*                                                             */
/***************************************************************/
void mem_write_32(uint32_t address, uint32_t value)
{
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
        if (address >= MEM_REGIONS[i].start &&
                address < (MEM_REGIONS[i].start + MEM_REGIONS[i].size)) {
            uint32_t offset = address - MEM_REGIONS[i].start;

            MEM_REGIONS[i].mem[offset+3] = (value >> 24) & 0xFF;
            MEM_REGIONS[i].mem[offset+2] = (value >> 16) & 0xFF;
            MEM_REGIONS[i].mem[offset+1] = (value >>  8) & 0xFF;
            MEM_REGIONS[i].mem[offset+0] = (value >>  0) & 0xFF;
            return;
        }
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------MIPS ISIM Help------------------------\n");
  printf("go                    - run program to completion     \n");
  printf("run n                 - execute program for n instrs  \n");
  printf("mdump low high        - dump memory from low to high  \n");
  printf("rdump                 - dump the register & bus value \n");
  printf("input reg_num reg_val - set GPR reg_num to reg_val    \n");
  printf("high value            - set the HI register to value  \n");
  printf("low value             - set the LO register to value  \n");
  printf("?                     - display this help menu        \n");
  printf("quit                  - exit the program              \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  INSTRUCTION_COUNT++;

  if (INSTRUCTION_COUNT >= 5000)
  {
    CURRENT_STATE.REGS[15] = 0xffffffff;
    RUN_BIT = FALSE;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate MIPS for n cycles                      */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (RUN_BIT == FALSE) {
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate MIPS until HALTed                      */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (RUN_BIT)
    cycle();
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address;

  printf("\nMemory content [0x%08x..0x%08x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = start; address <= stop; address += 4)
    printf("  0x%08x (%d) : 0x%08x\n", address, address, mem_read_32(address));
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%08x..0x%08x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = start; address <= stop; address += 4)
    fprintf(dumpsim_file, "  0x%08x (%d) : 0x%08x\n", address, address, mem_read_32(address));
  fprintf(dumpsim_file, "\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %u\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%08x\n", CURRENT_STATE.PC);
  printf("Registers:\n");
  for (k = 0; k < MIPS_REGS; k++)
    printf("R%d: 0x%08x\n", k, CURRENT_STATE.REGS[k]);
  printf("HI: 0x%08x\n", CURRENT_STATE.HI);
  printf("LO: 0x%08x\n", CURRENT_STATE.LO);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %u\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%08x\n", CURRENT_STATE.PC);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < MIPS_REGS; k++)
    fprintf(dumpsim_file, "R%d: 0x%08x\n", k, CURRENT_STATE.REGS[k]);
  fprintf(dumpsim_file, "HI: 0x%08x\n", CURRENT_STATE.HI);
  fprintf(dumpsim_file, "LO: 0x%08x\n", CURRENT_STATE.LO);
  fprintf(dumpsim_file, "\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;
  int register_no, register_value;
  int hi_reg_value, lo_reg_value;

  printf("MIPS-SIM> ");

  if (scanf("%s", buffer) == EOF)
      exit(0);

  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    if (scanf("%i %i", &start, &stop) != 2)
        break;

    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;

  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    if (scanf("%d", &cycles) != 1) break;
	    run(cycles);
    }
    break;

  case 'I':
  case 'i':
   if (scanf("%i %i", &register_no, &register_value) != 2)
      break;
   CURRENT_STATE.REGS[register_no] = register_value;
   NEXT_STATE.REGS[register_no] = register_value;
   break;
  
  case 'H':
  case 'h':
   if (scanf("%i", &hi_reg_value) != 1)
      break;
   CURRENT_STATE.HI = hi_reg_value; 
   NEXT_STATE.HI = hi_reg_value; 
   break;
  
  case 'L':
  case 'l':
   if (scanf("%i", &lo_reg_value) != 1)
      break;
   CURRENT_STATE.LO = lo_reg_value;
   NEXT_STATE.LO = lo_reg_value;
   break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Allocate and zero memoryy                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
        MEM_REGIONS[i].mem = (uint8_t*) malloc(MEM_REGIONS[i].size);
        memset(MEM_REGIONS[i].mem, 0, MEM_REGIONS[i].size);
    }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word;
  char* wordd;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  printf("Opened: %s\n", program_filename);

  /* Read in the program. */


  ii = 0;
  while (fscanf(prog, "%x\n",&word) != EOF) {
    mem_write_32(MEM_TEXT_START + ii, word);
    ii += 4;
    // printf("got:"
    //   BYTE_TO_BINARY_PATTERN " " 
    //   BYTE_TO_BINARY_PATTERN " " 
    //   BYTE_TO_BINARY_PATTERN " " 
    //   BYTE_TO_BINARY_PATTERN " : %x\n", 
    //   BYTE_TO_BINARY(word>>24), 
    //   BYTE_TO_BINARY(word>>16), 
    //   BYTE_TO_BINARY(word>>8), 
    //   BYTE_TO_BINARY(word),
    //   word);
  }

  CURRENT_STATE.PC = MEM_TEXT_START;

  printf("Read %d words from program into memory.\n\n", ii/4);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) 
{ 
  int i;

  init_memory();
  init_dram(8, 8, 1024, 1024);
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  NEXT_STATE = CURRENT_STATE;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
void run_shell(int argc, char *argv[])
{
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("MIPS Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
  {
    get_command(dumpsim_file);
  }

}

#ifdef TEST_MODE
int test_file(char* name)
{
  char path[80];
  sprintf(path, "../tests/test_%s.x", name);
  initialize(path, 1);
  go();
  // printf("[%s]: %d\n", name, (CURRENT_STATE.REGS[15] == 0) ? 1 : 0);
  return (int) CURRENT_STATE.REGS[15];
}

char* instructions[] = {
  "j",      // 0
  "jal",    // 1
  "jr",     // 2
  "addi",   // 3
  "mthi",   // 4
  "mtlo",   // 5
  "beq",    // 6
  "bne",    // 7
  "ori",    // 8
  "lui",    // 9

  "blez",   // 10
  "bgtz",   // 11
  "addiu",  // 12
  "slti",   // 13
  "sltiu",  // 14
  "andi",   // 15
  "xori",   // 16
  "lb",     // 17
  "lh",     // 18
  "lw",     // 19
  "lbu",    // 20
  "lhu",    // 21
  "sb",     // 22
  "sh",     // 23
  "sw",     // 24
  "bltz",   // 25
  "bgez",   // 26
  "bltzal", // 27
  "bgezal", // 28
  "sll",    // 29
  "srl",    // 30
  "sra",    // 31
  "sllv",   // 32
  "srlv",   // 33
  "srav",   // 34
  "jalr",   // 35
  "add",    // 36
  "addu",   // 37
  "sub",    // 38
  "subu",   // 39
  "and",    // 40
  "or",     // 41
  "xor",    // 42
  "nor",    // 43
  "slt",    // 44
  "sltu",   // 45
  "mult",   // 46
  "mfhi",   // 47
  "mflo",   // 48
  "multu",  // 49
  "div",    // 50
  "divu"    // 51
};

int results[52] = {0};

int test_j()
{
  test_file("j");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1

  if (r8==0 && r9==0) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_jal()
{
  test_file("jal");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1
  uint32_t ra = CURRENT_STATE.REGS[31]; // $t1

  if (r8==0 && r9==0 && ra==0x00400004) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_beq()
{
  test_file("beq");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1

  if (r8==5 && r9==5) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_bne()
{
  test_file("bne");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1

  if (r8==5 && r9==6) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_ori()
{
  test_file("ori");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1

  if (r8==0xbeef && r9==0xabcd) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_lw()
{
  test_file("lw");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0

  if (r8==0) 
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_jr()
{
  test_file("jr");
  uint32_t r8 = CURRENT_STATE.REGS[8]; // $t0
  uint32_t r9 = CURRENT_STATE.REGS[9]; // $t1

  if (r8==255 && r9==0) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_mthi()
{
  test_file("mthi");

  if (CURRENT_STATE.HI == 0xbef) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_mtlo()
{
  test_file("mtlo");

  if (CURRENT_STATE.LO == 0xbef) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_lui()
{
  test_file("lui");

  if (CURRENT_STATE.REGS[8] == 0xffff0000) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int test_addi()
{
  test_file("addi");

  if (CURRENT_STATE.REGS[2] == 0xa) // Succeeded, jumped over.
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

void test()
{
  int count = 0;
  for (int i=0; i<52; i++)
  {
    reset();
    int res = 0;
    // overload some tests (sorry), these ones kinda need to work
    switch(i)
    {
      break; case 0: res = test_j();
      break; case 1: res = test_jal();
      break; case 2: res = test_jr();
      break; case 3: res = test_addi();
      break; case 4: res = test_mthi();
      break; case 5: res = test_mtlo();
      break; case 6: res = test_beq();
      break; case 7: res = test_bne();
      break; case 8: res = test_ori();
      break; case 9: res = test_lui();
      break; default: res = test_file(instructions[i]);
    }
    int v = (res == 0) ? 1 : 0;
    count += v;
    if (v==1) printf("[+]: %s\n", instructions[i]);
    else printf("[-]: %s\n", instructions[i]);
    results[i] = v;
    reset();
  }

  printf("=============================================\n");
  printf("|                TEST RESULT                |\n");
  printf("=============================================\n\n");
  fflush(stdout);

  printf("[ NEED TO FIX ]: \n");
  for (int i=0; i<10; i++)
  {
    if (results[i]==0)
    {
      printf("%s\n", instructions[i]);
    }
  }

  printf("[ Correct ]: %d\n", count);
  printf("[ Incorrect ]: \n");
  for (int i=0; i<52; i++)
  {
    if (results[i]==0)
    {
      printf("%s\n", instructions[i]);
    }
  }
}
#endif

int main(int argc, char *argv[]) {                              
#ifndef TEST_MODE
  run_shell(argc, argv);
#else
  test();
#endif
}
