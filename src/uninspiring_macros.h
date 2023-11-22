#ifndef APPY_SACRILEGIOUS_MACROS
#define APPY_SACRILEGIOUS_MACROS

#include "utils.h"

/////////////////////////////////////
// NOTE(Appy): This file contains most of the macro abuse I used for no reason.

/////////////////////////////////////
// NOTE(Appy): Sizes

#define BYTE   8
#define WORD  16
#define DWORD 32

/////////////////////////////////////
// NOTE(Appy): Illegal macro helpers

#define CAT(x, s) x##s
#define CAT_HELPER(x, s) CAT(x, s)
#define PADDING CAT_HELPER(PAD, __COUNTER__)
#define BIT(x) (1 << (x))

/////////////////////////////////////
// NOTE(Appy): X-Macro for Supported Instructions.
//             This allows for some nice opcode automation, you can be lazy :)

#define OPCODES(x)\
  x(SPECIAL)      \
  x(REGIMM)       \
  x(J)            \
  x(JAL)          \
  x(BEQ)          \
  x(BNE)          \
  x(BLEZ)         \
  x(BGTZ)         \
  x(ADDI)         \
  x(ADDIU)        \
  x(SLTI)         \
  x(SLTIU)        \
  x(ANDI)         \
  x(ORI)          \
  x(XORI)         \
  x(LUI)          \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(LB)           \
  x(LH)           \
  x(PADDING)      \
  x(LW)           \
  x(LBU)          \
  x(LHU)          \
  x(PADDING)      \
  x(PADDING)      \
  x(SB)           \
  x(SH)           \
  x(PADDING)      \
  x(SW)           \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING)      \
  x(PADDING) 

/////////////////////////////////////
// NOTE(Appy): OPCODE AUTOMATION, DO NOT TOUCH.

#define ENUMERATE(OP) OP,
enum EOPCODES { OPCODES(ENUMERATE) NUMBER_OF_OPS };

/////////////////////////////////////
// NOTE(Appy): R-Type instructions

#define SLL     u32t(0x00)
#define SRL     u32t(0x02)
#define SRA     u32t(0x03)
#define JR      u32t(0x8)
#define JALR    u32t(0b001001)
#define MTHI    u32t(0x11)
#define MTLO    u32t(0x13)
#define MULT    u32t(0x18)
#define ADD     u32t(0x20)
#define ADDU    u32t(0x21)
#define SUB     u32t(0x22)
#define AND     u32t(0x24)
#define SUBU    u32t(0x23)
#define OR      u32t(0x25)
#define XOR     u32t(0x26)
#define NOR     u32t(0x27)
#define DIV     u32t(0x1A)
#define DIVU    u32t(0x1B)
#define SYSCALL u32t(0xC)
#define MFHI    u32t(0b010000)
#define MFLO    u32t(0b010010)
#define MULTU   u32t(0b011001)
#define SLLV    u32t(0b000100)
#define SLT     u32t(0b101010)
#define SLTU    u32t(0b101011)
#define SRAV    u32t(0b000111)
#define SRLV    u32t(0b000110)

/////////////////////////////////////
// NOTE(Appy): REGIMM ops

#define BLTZ   cast(u8, 0b00000)
#define BLTZAL cast(u8, 0b10000)
#define BGEZ   cast(u8, 0b00001)
#define BGEZAL cast(u8, 0b10001)

/////////////////////////////////////
// NOTE(Appy): Segment sizes

#define CD_SIZE    6
#define IM_SIZE    16
#define INSTR_SIZE 32
#define OP_SIZE    6
#define RD_SIZE    5
#define RS_SIZE    5
#define RT_SIZE    5
#define SA_SIZE    5
#define SHAMT_SIZE 5

/////////////////////////////////////
// NOTE(Appy): Segment positions
//
// RS    - Operand A in register file
// RT    - Operand B in register file
// RD    - Result destination in register file
// SHAMT - Shift amount
// CD    - Function code

#define OP_POS (INSTR_SIZE - OP_SIZE)
#define RS_POS (OP_POS - RS_SIZE)
#define RT_POS (RS_POS - RT_SIZE)
#define RD_POS (RT_POS - RD_SIZE)
#define SA_POS (RD_POS - SA_SIZE)
#define IM_POS (RT_POS - IM_SIZE)
#define CD_POS (0)
#define SHAMT_POS (6)

/////////////////////////////////////
// NOTE(Appy): Segment getters

#define GET_BLOCK(addr, start, size) ((addr >> (start)) & MASK(size))
#define GET(SEG, addr) GET_BLOCK(addr, SEG##_POS, SEG##_SIZE)

/////////////////////////////////////
// NOTE(Appy): Uninspiring helpers, these are only used for code to be readable, just ignore the implementation!

#define PASS_DOWN(OP) case OP
#define HANDLE(OP) case OP: { instr_##OP(INSTRUCTION_REGISTER); break; }
#define HANDLER(OP) void instr_##OP(uint32_t INSTRUCTION_REGISTER)
#define CALL_HANDLER(OP) instr_##OP(INSTRUCTION_REGISTER)
#define FORWARD_HANDLER(OP, TO) HANDLER(OP) { CALL_HANDLER(TO); }
#define DISPATCH(code) goto *jumpTable[code];
#define NEXT goto *jumpTable[u32t(NUMBER_OF_OPS)]
#define JUMPTABLE static const void *jumpTable[]
#define MK_LBL(OP) &&lbl_##OP,
#define LBL(OP) lbl_##OP

#endif /* APPY_SACRILEGIOUS_MACROS */
