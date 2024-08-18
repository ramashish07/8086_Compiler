#ifndef IMMEDIATE_H
#define IMMEDIATE_H

#include <bits/stdc++.h>
using namespace std;

#include "global_objects.h"

class immediate
{
public:
    bool is_immediate(string &);
    bool is_memory(string &);
    int8_t get_data(string &);
    bool check_if_string_hexadecimal(string &);
    int8_t convert_hex_string_to_decimal(string &);
    void upper_case(char &);
};

#endif