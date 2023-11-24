#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"
#include "dram.h"
#include <vector>
#include <iostream>
using namespace std;


dimm dram;

void inspect_dram(uint32_t start_address, uint32_t end_address){
    printf("inspecting dram from %x to %x\n", start_address, end_address-1);
    uint32_t current_address = start_address;
        while(current_address < end_address){
            uint32_t col_id = current_address & 0x3FF;
            uint32_t row_id = (current_address >> 10) & 0x3FF;
            uint32_t bank_id = (current_address >> 20) & 0x7;
            printf("current address: %x\n", current_address);
                uint8_t byte = dram.chips[0].banks[bank_id].cells[row_id][col_id];
                printf("%x\n", byte);
            current_address++;
        }
}

uint32_t get_block_data_at_address(cache_block block){
    uint32_t first_part = (block.data[0] & 0xFF);
    uint32_t second_part = (block.data[1]  & 0xFF) << 8;
    uint32_t third_part = (block.data[2] & 0xFF) << 16;
    uint32_t fourth_part = (block.data[3] & 0xFF) << 24;
    uint32_t return_data = fourth_part |
        third_part |
        second_part |
        first_part;
    return  return_data;
}

/***************************************************************/
/*                                                             */
/* Procedure : init_dram                                       */
/*                                                             */
/* Purpose   : initialize dram memory cells to 0s               */
/*                                                             */
/***************************************************************/
void init_dram(int chip_per_rank, int bank_per_chip, int row_size, int col_size) {                                           
    for (int i = 0; i < chip_per_rank; i++) {
        chip c;
        dram.chips.push_back(c);
        for (int j = 0; j < bank_per_chip; j++){
            bank b;
            dram.chips[i].banks.push_back(b);
            for (int k = 0; k < row_size; k++){
                vector<uint8_t> row;
                dram.chips[i].banks[j].cells.push_back(row);
                for (int l = 0; l < col_size; l++){
                    dram.chips[i].banks[j].cells[k].push_back(0);
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
uint32_t read_dram(uint32_t address) {                                           
    printf("READ\n");
    uint32_t col_id = address & 0x3FF;
    uint32_t row_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    cache_block block;
    for (int i = 0; i < 8; i++) {
        block.data.push_back((dram.chips[i].banks[bank_id].cells[row_id][col_id+3] << 24) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+2] << 16) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+1] <<  8) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id] <<  0));
    }
    for(int i = 0; i < 8; i++){
        printf("%8x\n", block.data[i]);
    }
    uint32_t data = get_block_data_at_address(block);
    return data;
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
            dram.chips[i].banks[bank_id].cells[row_id][col_id+3] = value >> 24 & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+2] = value >> 16 & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id+1] = value >> 8 & 0xFF;
            dram.chips[i].banks[bank_id].cells[row_id][col_id] = value & 0xFF;
            value = value >> 8;
    }
}

/*
int main(void){
    init_dram(8, 8, 1024, 1024);
    cache_block block = read_dram(0x00000000);
    uint32_t data = get_block_data_at_address(block);
    printf("data: %x\n", data);
    write_dram(0x00000000, 0xFFFFFFFF);                                         
    block = read_dram(0x00000000);
    data = get_block_data_at_address(block);
    printf("data: %x\n", data);
    return 0;
}
*/
