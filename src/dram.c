#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"

typedef struct {
} memory_cells;

typedef struct {
    uint8_t cells[1024][1024];
} bank;

typedef struct {
    bank banks[8];
} chip;

typedef struct {
    chip chips[8];
} dimm;

typedef struct {
    uint32_t data[8];
} cache_block;

dimm dram;

/***************************************************************/
/*                                                             */
/* Procedure : init_dram                                       */
/*                                                             */
/* Purpose   : initialize dram memory cells to 0s               */
/*                                                             */
/***************************************************************/
void init_dram() {                                           
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++){
            for (int k = 0; j < 1024; k++){
                for (int l = 0; j < 1024; l++){
                    dram.chips[i].banks[j].cells[k][l] = 0;
                }
            }
        }
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : read_dram                                       */
/*                                                             */
/* Purpose   : read back 32 bytes of dram                      */
/*                                                             */
/***************************************************************/
cache_block read_dram(uint32_t address) {                                           
    uint32_t row_id = address & 0x3FF;
    uint32_t col_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    cache_block block;
    for (int i = 0; i < 8; i++) {
        block.data[i] = (dram.chips[i].banks[bank_id].cells[row_id][col_id+3] << 24) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+2] << 16) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+1] <<  8) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id] <<  0);
    }
    return block;
}

/***************************************************************/
/*                                                             */
/* Procedure : write_dram                                       */
/*                                                             */
/* Purpose   : write back 32 bytes to dram                      */
/*                                                             */
/***************************************************************/
void write_dram(uint32_t address, uint32_t value) {                                           
    uint32_t row_id = address & 0x3FF;
    uint32_t col_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    for (int i = 0; i < 8; i++) {
            dram.chips[i].banks[bank_id].cells[row_id][col_id+3] = (value >> 24) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+2] = (value >> 16) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+1] = (value >>  8) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+0] = (value >>  0) & 0xFF;
    }
}
