#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"

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

void inspect_dram(uint32_t start_address, uint32_t end_address){
    printf("inspecting dram from %x to %x\n", start_address, end_address-1);
    uint32_t current_address = start_address;
        while(current_address < end_address){
            uint32_t col_id = current_address & 0x3FF;
            uint32_t row_id = (current_address >> 10) & 0x3FF;
            uint32_t bank_id = (current_address >> 20) & 0x7;
            printf("current address: %x\n", current_address);
                uint8_t byte = dram.chips[0].banks[bank_id].cells[row_id][col_id+3];
                printf("%x\n", byte);
            current_address++;
        }
}

void print_dram_data(cache_block block){
    for(int i = 0; i < 8; i++){
        printf("%x\n", block.data[i]);
    }
}

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
            for (int k = 0; k < 1024; k++){
                for (int l = 0; l < 1024; l++){
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
    printf("READ\n");
    uint32_t col_id = address & 0x3FF;
    uint32_t row_id = (address >> 10) & 0x3FF;
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
    printf("WRITE\n");
    uint32_t col_id = address & 0x3FF;
    uint32_t row_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    for (int i = 0; i < 8; i++) {
            dram.chips[i].banks[bank_id].cells[row_id][col_id+3] = (value >> 24) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+2] = (value >> 16) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+1] = (value >>  8) & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+0] = (value >>  0) & 0xFF;
    }
}

int main(void){
    init_dram();
    cache_block block = read_dram(0x00000000);
    //inspect_dram(0x00000000, 0x00000009);
    print_dram_data(block);
    write_dram(0x00000000, 0xFFFFFFFF);                                         
    //inspect_dram(0x00000000, 0x00000009);
    block = read_dram(0x00000000);
    print_dram_data(block);
    return 0;
}
