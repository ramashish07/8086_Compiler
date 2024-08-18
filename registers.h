#ifndef REGISTERS_H
#define REGISTERS_H

#include <bits/stdc++.h>
using namespace std;

class registers
{
private:
    int32_t AX, BX, CX, DX;
    int32_t CS, DS, SS, ES;
    int32_t SP, BP, SI, DI;
    int32_t AL, BL, CL, DL;
    int32_t AH, BH, CH, DH;
    int32_t flag;
    map<string, int32_t *> regi;

public:
    registers();

    bool is_segment_register(string &);
    bool is_register(string &);
    int32_t get_data(string &);
    void set_value(string &, int32_t);
    void print_register_map(std::ofstream &);
    void print_reg_data_hex_format(string &);
    void update_flag(string, bool);
    bool get_flag_data(string);
};

#endif