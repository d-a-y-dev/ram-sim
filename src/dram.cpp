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
    uint32_t first_part = (block.data[0] & 0xF);
    uint32_t second_part = (block.data[1]  & 0xF) << 4;
    uint32_t third_part = (block.data[2] & 0xF) << 8;
    uint32_t fourth_part = (block.data[3] & 0xF) << 12;
    uint32_t fifth_part = (block.data[4] & 0xF) << 16;
    uint32_t sixth_part = (block.data[5] & 0xF) << 20;
    uint32_t seventh_part = (block.data[6] & 0xF) << 24;
    uint32_t eigth_part = (block.data[7] & 0xF) << 28;
    uint32_t return_data = eigth_part |
        seventh_part |
        sixth_part |
        fifth_part |
        fourth_part |
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
    uint32_t chip_id = address & 0x7;
    uint32_t col_id = (address >> 3) & 0x3FF;
    uint32_t row_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    cache_block block;
    for (int i = 0; i < 8; i++) {
        block.data.push_back((dram.chips[i].banks[bank_id].cells[row_id][col_id+3] << 24) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+2] << 16) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id+1] <<  8) |
            (dram.chips[i].banks[bank_id].cells[row_id][col_id] <<  0));
    }
    uint32_t data = get_block_data_at_address(block);
    return data >> (chip_id * 8);
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
    uint32_t col_id = (address >> 3) & 0x3FF;
    uint32_t row_id = (address >> 10) & 0x3FF;
    uint32_t bank_id = (address >> 20) & 0x7;
    for (int i = 0; i < 8; i++) {
            dram.chips[i].banks[bank_id].cells[row_id][col_id] = value & 0xF;
            value = value >> 4;
    }
}

