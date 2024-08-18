#ifndef MEMORY_H
#define MEMORY_H

#include <bits/stdc++.h>
#include "registers.h"
#include "global_objects.h"

using namespace std;

class memory
{
private:
    int8_t *memory_location;
    int32_t total_size;

public:
    memory();
    bool is_memory(string &);
    uint16_t convert_string_to_memory_location(string &);
    uint16_t convert_hex_string_to_decimal(string &);
    void upper_case(char &);
    bool check_if_string_hexadecimal(string &);
    uint32_t string_to_effective_address(string &);
    int8_t get_data(string &);
    uint32_t calculate_effective_address(string &, uint16_t);
    void set_memory_location(string &, uint8_t);
};

#endif