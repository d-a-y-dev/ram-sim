#include <stdio.h>
#include "shell.h"
#include "uninspiring_macros.h"


/////////////////////////////////////
// NOTE(Appy): Sign extension

MAKE_SIGN_EXTEND(16)
MAKE_SIGN_EXTEND(8)

/////////////////////////////////////
// NOTE(Appy): Registers

#define R_V0 2
#define R_RA 31

int debug = 0;

void debugPrint(char* message){
    if(debug){
        printf("%s\n", message);
    }
}

/////////////////////////////////////
// NOTE(Appy): Handlers, basically where the code goes
//             when the first few bits denotes SPECIAL or REGIMM

typedef struct reg {
    u8 index;
    u32 value;
} reg;

typedef struct if_id {
    u32 instruction_register;
    u32 program_counter;
} if_id;
if_id if_id_buffer;

void reset_if_id(){
    if_id_buffer.instruction_register = 0;
    if_id_buffer.program_counter = 0;
}

typedef struct id_ex {
    reg rs;
    reg rt;
    reg rd;
    reg hi;
    reg lo;
    u32 instruction_register;
    u32 program_counter;
    u32 imm;
    u32 alu_src;
    u32 jumpAddress;
    u8 opcode;
    u8 reg_write;
    u8 reg_dst;
    u8 mem_to_rg;
    u8 alu_op;
    u8 mread;
    u8 mwrite;
    u8 jump;
    u8 branch;
    u8 run_bit;
} id_ex;
id_ex id_ex_buffer;

void reset_id_ex(){
    id_ex_buffer.rs.index = 0;
    id_ex_buffer.rs.value = 0;
    id_ex_buffer.rt.index = 0;
    id_ex_buffer.rt.value = 0;
    id_ex_buffer.rd.index = 0;
    id_ex_buffer.rd.value = 0;
    id_ex_buffer.hi.index = 0;
    id_ex_buffer.hi.value = 0;
    id_ex_buffer.lo.index = 0;
    id_ex_buffer.lo.value = 0;
    id_ex_buffer.instruction_register = 0;
    id_ex_buffer.program_counter = 0;
    id_ex_buffer.imm = 0;
    id_ex_buffer.alu_src = 0;
    id_ex_buffer.jumpAddress = 0;
    id_ex_buffer.opcode = 0;
    id_ex_buffer.reg_write = 0;
    id_ex_buffer.reg_dst = 0;
    id_ex_buffer.mem_to_rg = 0;
    id_ex_buffer.alu_op = 0;
    id_ex_buffer.mread = 0;
    id_ex_buffer.mwrite = 0;
    id_ex_buffer.jump = 0;
    id_ex_buffer.branch = 0;
}

typedef struct ex_mem {
    reg rs;
    reg rt;
    reg rd;
    reg hi;
    reg lo;
    u32 alu_src;
    u32 alu_result;
    u32 jumpAddress;
    u32 storeValue;
    u32 loadSize;
    u8 isLoadSigned;
    u8 branch;
    u8 jump;
    u8 branchCondition;
    u8 opcode;
    u8 mem_to_rg;
    u8 run_bit;
    u8 write_register;
    u8 mwrite;
    u8 mread;
    u8 reg_write;
} ex_mem;
ex_mem ex_mem_buffer;
void reset_ex_mem(){
    ex_mem_buffer.rs.index = 0;
    ex_mem_buffer.rs.value = 0;
    ex_mem_buffer.rt.index = 0;
    ex_mem_buffer.rt.value = 0;
    ex_mem_buffer.rd.index = 0;
    ex_mem_buffer.rd.value = 0;
    ex_mem_buffer.hi.index = 0;
    ex_mem_buffer.hi.value = 0;
    ex_mem_buffer.lo.index = 0;
    ex_mem_buffer.lo.value = 0;
    ex_mem_buffer.alu_src = 0;
    ex_mem_buffer.alu_result = 0;
    ex_mem_buffer.jumpAddress = 0;
    ex_mem_buffer.storeValue = 0;
    ex_mem_buffer.loadSize = 0;
    ex_mem_buffer.isLoadSigned = 0;
    ex_mem_buffer.branch = 0;
    ex_mem_buffer.jump = 0;
    ex_mem_buffer.branchCondition = 0;
    ex_mem_buffer.opcode = 0;
    ex_mem_buffer.mem_to_rg = 0;
    ex_mem_buffer.run_bit = 0;
    ex_mem_buffer.write_register = 0;
    ex_mem_buffer.mwrite = 0;
    ex_mem_buffer.mread = 0;
    ex_mem_buffer.reg_write = 0;
}

typedef struct mem_wb {
    reg hi;
    reg lo;
    u32 jumpAddress;
    u32 alu_result;
    u32 memory_data;
    u8 branch;
    u8 branchCondition;
    u8 jump;
    u8 reg_write;
    u8 mem_to_rg;
    u8 alu_src;
    u8 write_register;
    u8 run_bit;
} mem_wb;
mem_wb mem_wb_buffer;

void reset_mem_wb(){
    mem_wb_buffer.hi.index = 0;
    mem_wb_buffer.hi.value = 0;
    mem_wb_buffer.lo.index = 0;
    mem_wb_buffer.lo.value = 0;
    mem_wb_buffer.jumpAddress = 0;
    mem_wb_buffer.alu_result = 0;
    mem_wb_buffer.memory_data = 0;
    mem_wb_buffer.branch = 0;
    mem_wb_buffer.branchCondition = 0;
    mem_wb_buffer.jump = 0;
    mem_wb_buffer.reg_write = 0;
    mem_wb_buffer.mem_to_rg = 0;
    mem_wb_buffer.alu_src = 0;
    mem_wb_buffer.write_register = 0;
    mem_wb_buffer.run_bit = 0;
}


typedef struct staller {
    int status;
    int time;
} staller;

staller fetch_staller;
staller decode_staller;
staller execute_staller;
staller memory_staller;
staller writeback_staller;

int fetch_timer = 0;
int decode_timer = 0;
int fetch_status = 1;
int decode_status = 0;
int execute_status = 0;
int memory_status = 0;
int writeback_status = 0;


int registerStatus[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void printRegistersStatus(){
    if(!debug) {return;}
    for(int i = 0; i < 34; i++){
        printf("R%d: %d\n", i, registerStatus[i]);
    }
}

int dependency = 0;

void fetch()
{
  debugPrint("-----fetch-----");
  u32 instr = mem_read_32(CURRENT_STATE.PC);
  printf("%x\n", CURRENT_STATE.PC);
  if_id_buffer.instruction_register = instr;
  if_id_buffer.program_counter = CURRENT_STATE.PC;
  CURRENT_STATE.PC = CURRENT_STATE.PC + 4;
  printf("%x\n", CURRENT_STATE.PC);
}

int jump = 0;
int syscall = 0;
int load = 0;

void setup_registers(){
  id_ex_buffer.opcode = GET(OP, if_id_buffer.instruction_register);
  id_ex_buffer.rs.index = GET(RS, if_id_buffer.instruction_register);
  id_ex_buffer.rt.index = GET(RT, if_id_buffer.instruction_register);
  id_ex_buffer.rd.index = GET(RD, if_id_buffer.instruction_register);
  id_ex_buffer.rs.value = CURRENT_STATE.REGS[id_ex_buffer.rs.index];
  id_ex_buffer.rt.value = CURRENT_STATE.REGS[id_ex_buffer.rt.index];
  id_ex_buffer.rd.value = CURRENT_STATE.REGS[id_ex_buffer.rd.index];
  id_ex_buffer.hi.value = CURRENT_STATE.HI;
  id_ex_buffer.lo.value = CURRENT_STATE.LO;
  id_ex_buffer.imm = GET(IM, if_id_buffer.instruction_register);
  id_ex_buffer.program_counter = if_id_buffer.program_counter;
  id_ex_buffer.instruction_register = if_id_buffer.instruction_register;
}

void SetControlSignals(u8 jump, u8 branch, u8 mwrite, u8 mread, u8 reg_write, u8 reg_dst, u8 mem_to_rg, u8 alu_src, u8 alu_op){
  id_ex_buffer.jump = jump;
  id_ex_buffer.branch = branch;
  id_ex_buffer.mwrite = mwrite;
  id_ex_buffer.mread = mread;
  id_ex_buffer.reg_write = reg_write;
  id_ex_buffer.reg_dst = reg_dst;
  id_ex_buffer.mem_to_rg = mem_to_rg;
  id_ex_buffer.alu_src = alu_src;
  id_ex_buffer.alu_op = alu_op;
  id_ex_buffer.run_bit = TRUE;
}

void print_control_signals_decode(){
  printf("mwrite: %d\n",id_ex_buffer.mwrite);
  printf("mread: %d\n",id_ex_buffer.mread);
  printf("reg_write: %d\n",id_ex_buffer.reg_write);
  printf("reg_dst: %d\n",id_ex_buffer.reg_dst);
  printf("mem_to_rg: %d\n",id_ex_buffer.mem_to_rg);
  printf("alu_src: %d\n",id_ex_buffer.alu_src);
  printf("alu_op: %d\n",id_ex_buffer.alu_op);
}

void SetupControlSignals(){
  u8 opcode = GET(OP, if_id_buffer.instruction_register);
  switch(opcode)
  {
    // Rtypes
    case SPECIAL:
    {
        uint8_t code = GET(CD, if_id_buffer.instruction_register);
        switch (code) {
            case SYSCALL:
            {
                SetControlSignals(0, 0, 0, 0, 0, 0, 0, 0, opcode);
                break;
            } 
            case ADDU:
            case ADD: 
            case SUB: 
            case SUBU: 
            case OR: 
            case AND: 
            case XOR: 
            case NOR: 
            case SLL:
            case SLLV:
            case SRLV:
            case SRAV:
            case DIV:
            case DIVU:
            case MFHI:
            case MFLO:
            case SLT:
            case SLTU:
            case SRL:
            case SRA:
            {
                debugPrint("RTYPES");
                SetControlSignals(0, 0, 0, 0, 1, 1, 0, 0, opcode);
                break;
            } 
            case MULT:
            case MULTU:
            {
                debugPrint("MULT | MULTU");
                SetControlSignals(0, 0, 0, 0, 1, 1, 0, 0, opcode);
                id_ex_buffer.hi.index = 1;
                id_ex_buffer.lo.index = 1;
                break;
            } 
            case MTLO:
            {
                debugPrint("MTLO");
                SetControlSignals(0, 0, 0, 0, 1, 1, 0, 0, opcode);
                id_ex_buffer.rd.index = 33;
                break;
            } 
            case MTHI:
            {
                debugPrint("MTHI");
                SetControlSignals(0, 0, 0, 0, 1, 1, 0, 0, opcode);
                id_ex_buffer.rd.index = 32;
                break;
            } 
            case JR:
            {
                debugPrint("JR");
                SetControlSignals(1, 0, 0, 0, 0, 0, 0, 0, opcode);
                id_ex_buffer.jumpAddress = id_ex_buffer.rs.value;
                jump = 1;
                break;
            }
            case JALR:
            {
                debugPrint("JALR");
                SetControlSignals(1, 0, 0, 0, 1, 1, 0, 0, ADD);
                id_ex_buffer.jumpAddress = id_ex_buffer.rs.value;
                id_ex_buffer.rt.value = id_ex_buffer.program_counter;
                id_ex_buffer.rs.value = 4;
                jump = 1;
                break;
            } 
        }
        break;
    }
    // branch types
    case REGIMM:
    {
        u8 op = GET_BLOCK(if_id_buffer.instruction_register, 16, 5);
        switch (op) {
        case BGEZAL:
        case BLTZAL: {
                         debugPrint("BLTZAL | BGEZAL");
                         SetControlSignals(0, 1, 0, 0, 1, 0, 0, 1, opcode);
                         id_ex_buffer.rt.index = 31;
                         jump = 1;
                         break;
                     }
        case BGEZ:
        case BLTZ: {
                       debugPrint("BLTZ | BGEZ");
                       SetControlSignals(0, 1, 0, 0, 0, 0, 0, 1, opcode);
                       jump = 1;
                       break;
                   }
        }
        break;
    }

    case J:
    {
        debugPrint("J");
        SetControlSignals(1, 0, 0, 0, 0, 0, 0, 0, opcode);
        u32 label = GET_BLOCK(if_id_buffer.instruction_register, 0, 26);
        id_ex_buffer.jumpAddress = label << 2;
        jump = 1;
        break;
    }
    case JAL : {
        debugPrint("JAL");
        SetControlSignals(1, 0, 0, 0, 1, 1, 0, 0, ADD);
        id_ex_buffer.rd.index = 31;
        id_ex_buffer.rs.value = 4;
        id_ex_buffer.rt.value = id_ex_buffer.program_counter;
        u32 label = GET_BLOCK(if_id_buffer.instruction_register, 0, 26);
        id_ex_buffer.jumpAddress = label << 2;
        jump = 1;
        break;
    }

    // ITypes
    case ADDI:
    case ADDIU:
    case SLTI:
    case SLTIU:
    case ANDI:
    case ORI:
    case XORI:
    case LUI:
    {
        SetControlSignals(0, 0, 0, 0, 1, 0, 0, 1, opcode);
        break;
    }

    case BLEZ:
    case BGTZ:
    case BNE:
    case BEQ: {
        debugPrint("BRANCH TYPE");
        SetControlSignals(0, 1, 0, 0, 0, 0, 0, 0, opcode);
        jump = 1;
        break;
    }

    // Load
    case LW:
    case LB:
    case LH:
    case LBU:
    case LHU:
    {
        debugPrint("LOAD TYPE");
        SetControlSignals(0, 0, 0, 1, 1, 0, 1, 1, opcode);
        break;
    }

    // Store
    case SB:
    case SH:
    case SW:
    {
        debugPrint("STORE TYPE");
        SetControlSignals(0, 0, 1, 0, 0, 0, 0, 0, opcode);
        break;
    }
  }
}

void send_nop(){
  id_ex_buffer.opcode = 0;
  id_ex_buffer.rs.index = 0;
  id_ex_buffer.rt.index = 0;
  id_ex_buffer.rd.index = 0;
  id_ex_buffer.rs.value = 0;
  id_ex_buffer.rt.value = 0;
  id_ex_buffer.rd.value = 0;
  id_ex_buffer.hi.value = 0;
  id_ex_buffer.lo.value = 0;
  id_ex_buffer.imm = 0;
  id_ex_buffer.program_counter = 0;
  id_ex_buffer.instruction_register = 0;
  SetControlSignals(0, 0, 0, 0, 0, 0, 0, 0, -1);
}


void CheckDependencies(){
  u8 opcode = GET(OP, if_id_buffer.instruction_register);
  u8 rsi = GET(RS, if_id_buffer.instruction_register);
  u8 rti = GET(RT, if_id_buffer.instruction_register);
  u8 rdi = GET(RD, if_id_buffer.instruction_register);
  printf("opcode: %x\n", opcode);
  switch(opcode)
  {
    // Rtypes
    case SPECIAL:
    {
        u8 code = GET(CD, if_id_buffer.instruction_register);
        switch (code) {
            case SYSCALL:
            {
                debugPrint("Syscall");
                if(!registerStatus[2]){
                    dependency = 1;
                    syscall = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            } 
            case ADDU:
            case ADD: 
            case SUB: 
            case SUBU: 
            case OR: 
            case AND: 
            case XOR: 
            case NOR: 
            case SLL:
            case SLLV:
            case SRLV:
            case SRAV:
            case SLT:
            case SLTU:
            case SRL:
            case SRA:
            {
                printf("RTYPE\n");
                if(!registerStatus[rsi] || !registerStatus[rti]){
                debugPrint("Stall");
                dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            } 
            case DIV:
            case DIVU:
            case MULT:
            case MULTU:
            {
                debugPrint("MULT\n");
                    id_ex_buffer.hi.index = 1;
                    id_ex_buffer.lo.index = 1;
                if(!registerStatus[rsi] || !registerStatus[rti]){
                debugPrint("Stall");
                dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            }
            case MFHI:
            {
                printf("MFHI\n");
                if(!registerStatus[32]){
                debugPrint("Stall");
                dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            } 
            case MFLO:
            {
                printf("MFLO\n");
                if(!registerStatus[33]){
                debugPrint("Stall");
                dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            }
            case JR:
            {
                debugPrint("JR");
                if(!registerStatus[rsi]){
                    dependency = 1;
                }
                else{
                    fetch_status = 1;
                    dependency = 0;
                }
                break;
            }
            case JALR:
            {
                debugPrint("JALR");
                if(!registerStatus[rsi]){
                    dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            } 
            case MTLO:
            {
                if(!registerStatus[rsi]){
                    dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            }
            case MTHI:
            {
                if(!registerStatus[rsi]){
                    dependency = 1;
                }
                else{
                    dependency = 0;
                }
                break;
            }
        }
        break;
    }
    // branch types
    case REGIMM:
    {
        u8 op = GET_BLOCK(if_id_buffer.instruction_register, 16, 5);
        switch (op) {
        case BGEZAL:
        case BLTZAL: {
            break;
        }
        case BLTZ:
        case BGEZ: {
            debugPrint("Branch");
            if(!registerStatus[rsi]){
                debugPrint("Dependency Stall");
                dependency = 1;
            }
            else{
                debugPrint("Brach Stall");
                dependency = 0;
            }
            break;
        }
        }
        break;
    }

    case JAL:
    {
        debugPrint("JAL");
        if(!registerStatus[31]){
            debugPrint("31 Dependency");
            dependency = 1;
        }
        else{
            dependency = 0;
        }
        break;
    }
    case J:
    {
        debugPrint("J");
        break;
    }

    // ITypes
    case ADDI:
    case ADDIU:
    case SLTI:
    case SLTIU:
    case ANDI:
    case ORI:
    case XORI:
    {
        printf("ITYPE\n");
        if(!registerStatus[rsi]){
            dependency = 1;
        }
        else{
            dependency = 0;
        }
        break;
    }
    case LUI:{
        if(!registerStatus[rti]){
            dependency = 1;
        }
        else{
            dependency = 0;
        }
        break;
    }

    case LW:
    case LB:
    case LH:
    case LBU:
    case LHU:{
        debugPrint("Load");
        load = 1;
        if(!registerStatus[rsi] ){
            debugPrint("Dependency Stall");
            dependency = 1;
        }
        else{
            dependency = 0;
        }
        break;

    }

    case BLEZ:
    case BGTZ:
    case BNE:
    case BEQ: {
        debugPrint("Branch");
        if(!registerStatus[rsi] || !registerStatus[rti]){
            debugPrint("Dependency Stall");
            dependency = 1;
        }
        else{
            debugPrint("Brach Stall");
            dependency = 0;
        }
        break;
    }

    // Load

    // Store
    case SB:
    case SH:
    case SW:
    {
        debugPrint("Store");
        if(!registerStatus[rsi] || !registerStatus[rti]){
            debugPrint("Stall");
            dependency = 1;
        }
        else{
            dependency = 0;
        }
        break;
    }
  }
}


int cycle_no = 0;


void decode()
{
      if(load){
          debugPrint("HOT LOAD UGH");
          decode_timer = 2;
          fetch_timer = 2;
          fetch_status = 0;
          decode_status = 0;
          send_nop();
          load = 0;
          return;
      }
  debugPrint("-----decode-----");
  CheckDependencies();
  setup_registers();
  SetupControlSignals();
  if(dependency){
      u8 rti = id_ex_buffer.rt.index;
      u8 rsi = id_ex_buffer.rs.index;
      u8 ex_reg_dst = ex_mem_buffer.write_register;
      u8 mem_reg_dst = mem_wb_buffer.write_register;
      printf("rti: %d\nrsi: %d\nex_reg_dst: %d\nmem_reg_dst: %d\n", rti, rsi, ex_reg_dst, mem_reg_dst);
      if(ex_reg_dst == rsi && ex_reg_dst != 0){
          debugPrint("forward ex_mem to rs");
          id_ex_buffer.rs.value = ex_mem_buffer.alu_result;
          printf("alu_result: %x\n", ex_mem_buffer.alu_result);

      }
      else if(mem_reg_dst == rsi && mem_reg_dst != 0){
          debugPrint("forward mem_wb to rs");
          id_ex_buffer.rs.value = mem_wb_buffer.alu_result;
          printf("alu_result: %x\n", mem_wb_buffer.alu_result);
      }
      if(ex_reg_dst == rti && ex_reg_dst != 0){
          debugPrint("forward ex_mem to rt");
          id_ex_buffer.rt.value = ex_mem_buffer.alu_result;
          printf("alu_result: %x\n", ex_mem_buffer.alu_result);
      }
      else if(mem_reg_dst == rti && mem_reg_dst != 0){
          debugPrint("forward mem_wb to rt");
          id_ex_buffer.rt.value = mem_wb_buffer.alu_result;
          printf("alu_result: %x\n", mem_wb_buffer.alu_result);
      }
      if(ex_mem_buffer.hi.value != 0){
          id_ex_buffer.hi.value = ex_mem_buffer.hi.value;
      }
      else if(mem_wb_buffer.hi.value != 0){
          id_ex_buffer.hi.value = mem_wb_buffer.hi.value;
      }
      if(ex_mem_buffer.lo.value != 0){
          id_ex_buffer.lo.value = ex_mem_buffer.lo.value;
      }
      else if(mem_wb_buffer.lo.value != 0){
          id_ex_buffer.lo.value = mem_wb_buffer.lo.value;
      }
      if(syscall){
          id_ex_buffer.run_bit = FALSE;
          syscall = 0;
      }
  }
  if(id_ex_buffer.jump){
    SetupControlSignals();
    debugPrint("FLUSH");
    CURRENT_STATE.PC = id_ex_buffer.jumpAddress;
    reset_if_id();
  }
  if(id_ex_buffer.reg_write){
      u8 rti = id_ex_buffer.rt.index;
      u8 rdi = id_ex_buffer.rd.index;
      u8 regDst = id_ex_buffer.reg_dst == 0 ? rti : rdi;
      if(id_ex_buffer.hi.index || id_ex_buffer.lo.index){
          if(id_ex_buffer.hi.index){
            registerStatus[32] = 0;
          }
          if(id_ex_buffer.lo.index){
            registerStatus[33] = 0;
          }
          id_ex_buffer.hi.index = 0;
          id_ex_buffer.lo.index = 0;
      }
      else{
          registerStatus[regDst] = 0;
      }
      if(debug){
        printf("write at R%d\n", regDst);
      }
  }
}

void set_alu_result(u32 alu_result){
    ex_mem_buffer.alu_result = alu_result;
}

u32 get_alu_src(){
    return id_ex_buffer.alu_src == 0 ? id_ex_buffer.rt.value : id_ex_buffer.imm;
}

void set_ex_reg_write(){
    ex_mem_buffer.write_register = id_ex_buffer.reg_dst == 0 ? id_ex_buffer.rt.index : id_ex_buffer.rd.index;
}


void alu(){
    u32 operand_a = id_ex_buffer.rs.value;
    u32 operand_b = get_alu_src();
    switch(id_ex_buffer.alu_op){
        case SPECIAL:
        {
            uint8_t funct = GET(CD, id_ex_buffer.instruction_register);
            uint8_t sa = GET(SA, id_ex_buffer.instruction_register);
            switch(funct)
            {
                case SYSCALL:{
                     debugPrint("SYSCALL");
                    break;
                }
                case ADDU:{
                     debugPrint("ADDU");
                    set_alu_result(operand_a + sign_extend_16(operand_b));
                    break;
                }
                case ADD:{
                     debugPrint("ADD");
                    set_alu_result(operand_a + operand_b);
                    break;
                }
                case SUBU:
                case SUB: {
                     debugPrint("SUB");
                    set_alu_result(operand_a - operand_b);
                    break;
                }
                case OR: {
                     debugPrint("OR");
                    set_alu_result(operand_a | operand_b);
                    break;
                }
                case NOR: {
                     debugPrint("NOR");
                    set_alu_result(NOR_OP(operand_a, operand_b));
                    break;
                }
                case AND: {
                     debugPrint("AND");
                    set_alu_result(operand_a & operand_b);
                    break;
                }
                case MTLO: {
                     debugPrint("MTLO");
                   ex_mem_buffer.lo.index = 1;
                   ex_mem_buffer.lo.value = operand_a;
                   break;
                }
                case MTHI: {
                     debugPrint("MTHI");
                   ex_mem_buffer.hi.index = 1;
                   ex_mem_buffer.hi.value = operand_a;
                   break;
                }
                case MFHI: {
                     debugPrint("MFHI");
                   set_alu_result(ex_mem_buffer.hi.value);
                   break;
                }
                case MFLO: {
                     debugPrint("MFLO");
                   set_alu_result(ex_mem_buffer.lo.value);
                   break;
                }
                case MULT: {
                     debugPrint("MULT");
                   s32 rs     = operand_a;
                   s32 rt     = operand_b;
                   s64 result = rs * rt;
                   ex_mem_buffer.hi.index = 1;
                   ex_mem_buffer.lo.index = 1;
                   ex_mem_buffer.hi.value = cast(u32, (result >> 32));
                   ex_mem_buffer.lo.value = cast(u32, result);
                   break;
                }
                case MULTU: {
                     debugPrint("MULTU");
                    u64 result = cast(u64, operand_a) * cast(u64, operand_b);
                    ex_mem_buffer.hi.index = 1;
                    ex_mem_buffer.lo.index = 1;
                    ex_mem_buffer.hi.value = cast(u32, (result >> 32));
                    ex_mem_buffer.lo.value = cast(u32, result);
                    break;
                }
                case SLLV:
                {
                     debugPrint("SLLV");
                    u8 shift_amount = (operand_a & MASK(5));
                    set_alu_result(operand_b << shift_amount);
                    break;
                }
                case XOR:
                {
                     debugPrint("XOR");
                    set_alu_result(operand_a ^ operand_b);
                    break;
                }
                case SLT: {
                     debugPrint("SLT");
                    s32 rt = operand_b;
                    s32 rs = operand_a;
                    set_alu_result((rs < rt));
                    break;
                }
                case SLTU:
                {
                     debugPrint("SLTU");
                    u32 rt = operand_b;
                    u32 rs = operand_a;
                    set_alu_result((rs < rt));
                    break;
                }
                case SRAV:
               {
                     debugPrint("SRAV");
                    u8 shift_amount = (operand_a & MASK(5));
                    u32 rt = operand_b;
                    u8 sign_bit = ((rt >> 31) & 1);
                    u32 res = (rt >> shift_amount);
                    if (sign_bit==1) res |= (MASK(shift_amount) << (32-shift_amount));
                    set_alu_result(res);
                    break;
                }
                case SRLV:
                {
                     debugPrint("SRLV");
                   u8 shift_amount = (operand_a & MASK(5));
                    set_alu_result(operand_b >> shift_amount);
                    break;
                }
                case DIVU:{
                     debugPrint("DIVU");
                      u32 numerator   = operand_a;
                      u32 denominator = operand_b;
                      if (denominator != 0) {
                          ex_mem_buffer.lo.value = u32t(numerator / denominator);
                          ex_mem_buffer.hi.value = u32t(numerator % denominator);
                          ex_mem_buffer.hi.index = 1;
                          ex_mem_buffer.lo.index = 1;
                      }
                      break;
                }
                case DIV: {
                     debugPrint("DIV");
                      s32 numerator   = s32t(operand_a);
                      s32 denominator = s32t(operand_b);
                      if (denominator != 0) {
                          ex_mem_buffer.lo.value = u32t(numerator / denominator);
                          ex_mem_buffer.hi.value = u32t(numerator % denominator);
                          ex_mem_buffer.hi.index = 1;
                          ex_mem_buffer.lo.index = 1;
                      }
                      break;
                }
                case SLL: {
                     debugPrint("SLL");
                      set_alu_result(operand_b << sa);
                      break;
                } // Shift Left
                case SRL: {
                     debugPrint("SRL");
                      set_alu_result(operand_b >> sa);
                      break;
                }         // Shirt Right
                case SRA: // Shift Right Addition
                {
                     debugPrint("SRA");
                      uint32_t operand = operand_b;

                      int need_extend = (operand >> 31) & 1;
                      uint32_t result = (operand >> sa);

                      if (need_extend) {
                          /* Sign extension */
                          result = result | MASK1(sa, INSTR_SIZE - sa);
                      }

                      set_alu_result(result);
                      break;
                }
            }
            break;
        }

        case REGIMM:
        {
              u8 op = GET_BLOCK(id_ex_buffer.instruction_register, 16, 5);
              switch (op) {
              case BLTZ: {
                debugPrint("BLTZ");
                u32 addr = id_ex_buffer.program_counter + (sign_extend_16(operand_b) << 2);
                int branch  = operand_a >> 31;
                if (branch == 1){
                    debugPrint("TAKE");
                    ex_mem_buffer.branchCondition  = 1;
                    ex_mem_buffer.jumpAddress = addr;
                }
                else{
                    ex_mem_buffer.branchCondition  = 0;
                    ex_mem_buffer.jumpAddress = addr;
                }
                break;
              }
              case BLTZAL: {
                u32 addr = id_ex_buffer.program_counter + (sign_extend_16(operand_b) << 2);
                int branch  = operand_a >> 31;
                //printf("branch: %d\n", branch);
                set_alu_result(id_ex_buffer.program_counter + 4);
                if (branch == 1){
                    ex_mem_buffer.branchCondition  = 1;
                    ex_mem_buffer.jumpAddress = addr;
                }
                else{
                    ex_mem_buffer.branchCondition  = 0;
                    ex_mem_buffer.jumpAddress = addr;
                }
                break;
              }
              case BGEZ: {
                u32 addr = id_ex_buffer.program_counter + (sign_extend_16(operand_b) << 2);
                int branch = operand_a >> 31;
                if (branch == 0){
                    ex_mem_buffer.branchCondition  = 1;
                    ex_mem_buffer.jumpAddress = addr;
                }
                else{
                    ex_mem_buffer.branchCondition  = 0;
                    ex_mem_buffer.jumpAddress = addr;
                }
                break;
              }
              case BGEZAL: {
                u32 addr = id_ex_buffer.program_counter + (sign_extend_16(operand_b) << 2);
                int branch  = operand_a >> 31;
                set_alu_result(id_ex_buffer.program_counter + 4);
                if (branch == 0){
                    ex_mem_buffer.branchCondition  = 1;
                    ex_mem_buffer.jumpAddress = addr;
                }
                else{
                    ex_mem_buffer.branchCondition  = 0;
                    ex_mem_buffer.jumpAddress = addr;
                }
                break;
              }
              }
            break;
        }
        case ADDI:
        case ADDIU:{
            printf("ADDI\n");
            u32 result = sign_extend_16(operand_b);
            set_alu_result(operand_a + result);
            break;
        }
      case XORI : {
        set_alu_result(operand_a ^ u32t(operand_b));
        break;
      }
      case ANDI : {
        set_alu_result(operand_a & u32t(operand_b));
        break;
      }
      case ORI : {
        printf("ORI\n");
        printf("operand_a: %x\n", operand_a);
        printf("operand_b: %x\n", operand_b);
        set_alu_result(operand_a | u32t(operand_b));
        break;
      }
      case LUI:{
            set_alu_result(operand_b << 16);
            break;
      }
    case BEQ: {
        u32 rs = id_ex_buffer.rs.value;
        u32 rt = id_ex_buffer.rt.value;
        u32 addr = id_ex_buffer.program_counter + (id_ex_buffer.imm << 2);
        printf("BEQ rs: {%x}, rt: {%x}\n", rs, rt);
        if (rs == rt) {
          ex_mem_buffer.branchCondition = 1;
          ex_mem_buffer.jumpAddress = addr;
        }
        else{
            ex_mem_buffer.branchCondition  = 0;
            ex_mem_buffer.jumpAddress = addr;
        }
        break;
    }
    case BNE: {
        u32 rs = id_ex_buffer.rs.value;
        u32 rt = id_ex_buffer.rt.value;
        u32 addr = id_ex_buffer.program_counter + (id_ex_buffer.imm << 2);
        printf("BNE rs: {%x}, rt: {%x}\n", rs, rt);
        if (rs != rt) {
          ex_mem_buffer.branchCondition = 1;
          ex_mem_buffer.jumpAddress = addr;
        }
        else{
            ex_mem_buffer.branchCondition  = 0;
            ex_mem_buffer.jumpAddress = addr;
        }
        break;
    }


      /* Default case. */
      case SB : {
        u32 imm = id_ex_buffer.imm;
        uint32_t offset = sign_extend_16(imm);
        uint32_t vAddr = offset + operand_a;
        uint32_t r_rt = id_ex_buffer.rt.value;
        uint32_t last_byte = (GET_BLOCK(r_rt, 0, 8));
        ex_mem_buffer.storeValue = last_byte;
        set_alu_result(vAddr);
        break;
      }
      case SH : {
        u32 imm = id_ex_buffer.imm;
        u32 rs = id_ex_buffer.rs.value;
        u32 rt = id_ex_buffer.rt.value;
        uint32_t offset = sign_extend_16(imm);
        uint32_t address = offset + rs;
        uint32_t r_rt = rt;
        uint32_t last_byte = (GET_BLOCK(r_rt, 0, 16));
        ex_mem_buffer.storeValue = last_byte;
        set_alu_result(address);
        break;
      }
        case SW:{
            u32 imm = id_ex_buffer.imm;
            u32 rs = id_ex_buffer.rs.value;
            u32 rt = id_ex_buffer.rt.value;
            u32 vAddr = rs + sign_extend_16(imm);
            ex_mem_buffer.storeValue = id_ex_buffer.rt.value;
            set_alu_result(vAddr);
            break;
        }
      case SLTI : {
            set_alu_result((cast(s32, operand_a) < cast(s32, sign_extend_16(operand_b))) ? 1 : 0);
            break;
      }
      case SLTIU : {
        u32 result = operand_a - sign_extend_16(operand_b);
        set_alu_result((operand_a < result) ? 1 : 0);
        break;
      }
      case LW:{
        u32 vAddr = operand_a + sign_extend_16(operand_b);
        ex_mem_buffer.loadSize = 32;
        ex_mem_buffer.isLoadSigned = 0;
        set_alu_result(vAddr);
        break;
      }
      case LHU:{
        u32 vAddr = operand_a + sign_extend_16(operand_b);
        ex_mem_buffer.loadSize = 16;
        ex_mem_buffer.isLoadSigned = 0;
        set_alu_result(vAddr);
        break;
      }
      case LH:{
        u32 vAddr = operand_a + sign_extend_16(operand_b);
        ex_mem_buffer.loadSize = 16;
        ex_mem_buffer.isLoadSigned = 1;
        set_alu_result(vAddr);
        break;
      }
      case LBU:{
        u32 vAddr = operand_a + sign_extend_16(operand_b);
        ex_mem_buffer.loadSize = 8;
        ex_mem_buffer.isLoadSigned = 0;
        set_alu_result(vAddr);
        break;
      }
      case LB: {
        u32 vAddr = operand_a + sign_extend_16(operand_b);
        ex_mem_buffer.loadSize = 8;
        ex_mem_buffer.isLoadSigned = 1;
        set_alu_result(vAddr);
        break;
      }
      case BLEZ : {
        u32 rs = operand_a;
        u32 addr = id_ex_buffer.program_counter  + (sign_extend_16(id_ex_buffer.imm) << 2);
        if (rs == 0 || ((rs >> 31) == 1)) {
          ex_mem_buffer.branchCondition = 1;
          ex_mem_buffer.jumpAddress = addr;
        }
        else{
            ex_mem_buffer.branchCondition  = 0;
            ex_mem_buffer.jumpAddress = addr;
        }
        break;
      }
      case BGTZ : {
        u32 rs = operand_a;
        u32 addr = id_ex_buffer.program_counter  + (sign_extend_16(id_ex_buffer.imm) << 2);
        if (rs != 0 && ((rs >> 31) == 0)) {
          ex_mem_buffer.branchCondition = 1;
          ex_mem_buffer.jumpAddress = addr;
        }
        else{
            ex_mem_buffer.branchCondition  = 0;
            ex_mem_buffer.jumpAddress = addr;
        }
        break;
      }
}
}

void execute(){
  debugPrint("-----execute-----");
    ex_mem_buffer.hi = id_ex_buffer.hi;
    ex_mem_buffer.lo = id_ex_buffer.lo;
    ex_mem_buffer.jump = id_ex_buffer.jump;
    ex_mem_buffer.branch = id_ex_buffer.branch;
    ex_mem_buffer.jumpAddress = id_ex_buffer.jumpAddress;
    ex_mem_buffer.opcode = id_ex_buffer.opcode;
    ex_mem_buffer.mread = id_ex_buffer.mread;
    ex_mem_buffer.mwrite = id_ex_buffer.mwrite;
    ex_mem_buffer.mem_to_rg = id_ex_buffer.mem_to_rg;
    ex_mem_buffer.reg_write = id_ex_buffer.reg_write;
    ex_mem_buffer.run_bit = id_ex_buffer.run_bit;
    alu();
    set_ex_reg_write();
    if(ex_mem_buffer.branch && ex_mem_buffer.branchCondition){
            debugPrint("FLUSH");
            CURRENT_STATE.PC = ex_mem_buffer.jumpAddress;
            reset_id_ex();
            reset_if_id();
    }
}
u32 SignExtend(u8 isSigned, u32 content, u32 loadSize){
    if(isSigned){
        if(loadSize == 16){
            return sign_extend_16(content); 
        }
        return sign_extend_8(content);
    }
    return content;
}
void read_data_from_memory(){
    if (ex_mem_buffer.mread){
        u32 vAddr = ex_mem_buffer.alu_result;
        u32 content = mem_read_32(vAddr);
        if(ex_mem_buffer.loadSize != 32){
            content = GET_BLOCK(content, 0 , ex_mem_buffer.loadSize);
        }
        content = SignExtend(ex_mem_buffer.isLoadSigned, content, ex_mem_buffer.loadSize);
        mem_wb_buffer.memory_data = content;
    }
}

void store_data_to_memory(){
    if (ex_mem_buffer.mwrite){
        mem_write_32(ex_mem_buffer.alu_result, ex_mem_buffer.storeValue);
    }
}

void memory(){
  debugPrint("-----memory-----");
    mem_wb_buffer.lo = ex_mem_buffer.lo;
    mem_wb_buffer.hi = ex_mem_buffer.hi;
    mem_wb_buffer.jump = ex_mem_buffer.jump;
    mem_wb_buffer.branch = ex_mem_buffer.branch;
    mem_wb_buffer.branchCondition = ex_mem_buffer.branchCondition;
    mem_wb_buffer.jumpAddress = ex_mem_buffer.jumpAddress;
    //read from memeory
    read_data_from_memory();
    store_data_to_memory();
    mem_wb_buffer.mem_to_rg = ex_mem_buffer.mem_to_rg;
    mem_wb_buffer.reg_write = ex_mem_buffer.reg_write;
    mem_wb_buffer.run_bit = ex_mem_buffer.run_bit;
    
    //forward write_register
    mem_wb_buffer.write_register = ex_mem_buffer.write_register;
    printf("write_register: {%d}\n", mem_wb_buffer.write_register);
    //forward alu_result
    mem_wb_buffer.alu_result =  ex_mem_buffer.alu_result;
}


void write_back(){
  debugPrint("-----writeback-----");
    RUN_BIT = mem_wb_buffer.run_bit;
    if (mem_wb_buffer.reg_write) {
    if (mem_wb_buffer.mem_to_rg){
            CURRENT_STATE.REGS[mem_wb_buffer.write_register] = mem_wb_buffer.memory_data;
            registerStatus[mem_wb_buffer.write_register] = 1;
    }
    else{
        if (mem_wb_buffer.hi.index) {
            CURRENT_STATE.HI = mem_wb_buffer.hi.value;
            registerStatus[32] = 1;
        }
        if (mem_wb_buffer.lo.index) {
            CURRENT_STATE.LO = mem_wb_buffer.lo.value;
            registerStatus[33] = 1;
        }
        if (!(mem_wb_buffer.hi.index) && !(mem_wb_buffer.lo.index)) {
            CURRENT_STATE.REGS[mem_wb_buffer.write_register] = mem_wb_buffer.alu_result;
            printf("%d\n",mem_wb_buffer.write_register);
            if(ex_mem_buffer.write_register != mem_wb_buffer.write_register){
                registerStatus[mem_wb_buffer.write_register] = 1;
            }
        }
    }
    }
    if (mem_wb_buffer.jump){
        debugPrint("JUMP");
    }
    else if (mem_wb_buffer.branch){
        debugPrint("BRANCH");
    }
    else{
        CURRENT_STATE.PC = CURRENT_STATE.PC;
        dependency = 0;
    }
    printf("RUN_BIT: %d\n", RUN_BIT);
}

void check_stall(){
    if(fetch_timer > 0){
        fetch_timer--;
        if(fetch_timer == 0){
            fetch_status = 1;
        }
    }
    if(decode_timer > 0){
        decode_timer--;
        if(decode_timer == 0){
            decode_status = 1;
        }
    }
}

void reset_staller(){
    fetch_staller.time = 0;
    fetch_staller.status = 0;
    decode_staller.time = 0;
    decode_staller.time = 0;
    execute_staller.status = 0;
    execute_staller.status = 0;
    memory_staller.time = 0;
    memory_staller.status = 0;
    writeback_staller.time = 0;
    writeback_staller.status = 0;
}

void reset_state(){
    debugPrint("reset_state");
    for(int i = 0; i < 34; i++){
        registerStatus[i] = 1;
    }
    jump = 0;
    dependency = 0;
    syscall = 0;
    load = 0;
    fetch_status = 1;
    decode_status = 0;
    execute_status = 0;
    memory_status = 0;
    writeback_status = 0;
    reset_if_id();
    reset_id_ex();
    reset_ex_mem();
    reset_mem_wb();
}



void process_instruction() 
{

    debug = 1;
    printRegistersStatus();

    check_stall();


    if(writeback_status){
        write_back();
        writeback_status = 0;
    }
    if(memory_status){
        writeback_status = 1;
        memory();
        memory_status = 0;
    }
    if(execute_status){
        memory_status = 1;
        execute();
        execute_status = 0;
    }
    if(decode_status){
        execute_status = 1;
        decode();
        decode_status = 0;
    }
    if(fetch_status){
        decode_status = 1;
        fetch();
    }

    if(RUN_BIT == FALSE){
        reset_state();
    }

/* Undefine guards */
#undef END_LABEL

#ifdef RS
#undef RS
#endif

#ifdef RT
#undef RT
#endif

#ifdef IMM
#undef IMM
#endif
}
