#include "shell.h"
#include <stdint.h>
#include <vector>
#include <iostream>
using namespace std;

typedef struct {
    vector<vector<uint8_t>> cells;
} bank;

typedef struct {
    vector<bank> banks;
} chip;

typedef struct {
    vector<chip> chips;
} dimm;

typedef struct {
    vector<uint32_t> data;
} cache_block;

void init_dram(int chip_per_rank, int bank_per_chip, int row_size, int col_size);                                          
uint32_t get_block_data_at_address(cache_block block);
uint32_t read_dram(uint32_t address);                                           
void write_dram(uint32_t address, uint32_t value);                                           
