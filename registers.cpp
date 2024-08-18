#include "registers.h"

registers::registers()
{
    regi["AX"] = &AX;
    regi["BX"] = &BX;
    regi["CX"] = &CX;
    regi["DX"] = &DX;
    regi["BP"] = &BP;
    regi["SP"] = &SP;
    regi["DI"] = &DI;
    regi["SI"] = &SI;
    regi["FLAG"] = &flag;

    regi["AL"] = &AL;
    regi["BL"] = &BL;
    regi["CL"] = &CL;
    regi["DL"] = &DL;

    regi["AH"] = &AH;
    regi["BH"] = &BH;
    regi["CH"] = &CH;
    regi["DH"] = &DH;

    regi["CS"] = &CS;
    regi["DS"] = &DS;
    regi["ES"] = &ES;
    regi["SS"] = &SS;

    int temp = rand();
    for (auto &i : regi)
    {
        *(i.second) = temp;
        temp++;
    }

    AH = ((AX & (0xFF00)) >> 8);
    AL = (AX & (0x00FF));
    BH = ((BX & (0xFF00)) >> 8);
    BL = (BX & (0x00FF));
    CH = ((CX & (0xFF00)) >> 8);
    CL = (CX & (0x00FF));
    DH = ((DX & (0xFF00)) >> 8);
    DL = (DX & (0x00FF));

    flag = 0;

    CS = 0;
    DS = 16384;
    ES = 2 * DS;
    SS = 3 * DS;
}

bool registers::is_segment_register(string &register_name)
{
    return (register_name == "CS" || register_name == "DS" || register_name == "ES" || register_name == "SS");
}

bool registers::is_register(string &register_name)
{
    return regi.count(register_name);
}

int32_t registers::get_data(string &register_name)
{
    if (regi.count(register_name) == 0)
    {
        cout << register_name << "\n\n";
        cout << "the given register name does not exist\n\n";
        return -1;
    }
    return *(regi[register_name]);
}

void registers::set_value(string &register_name, int32_t val)
{
    if (regi.count(register_name) == 0)
    {
        cout << register_name << "\n\n";
        cout << "the given register name does not exist\n\n";
        return;
    }
    *regi[register_name] = val;

    int n = register_name.size();
    char last_character = register_name[n - 1];

    if (last_character == 'H')
    {
        *regi[register_name.substr(0, 1) + "X"] &= (0x00FF);
        *regi[register_name.substr(0, 1) + "X"] |= (val << 8);
    }
    else if (last_character == 'L')
    {
        *regi[register_name.substr(0, 1) + "X"] &= (0xFF00);
        *regi[register_name.substr(0, 1) + "X"] |= (val);
    }
    else if (last_character == 'X')
    {
        *regi[register_name.substr(0, 1) + "L"] = 0;
        *regi[register_name.substr(0, 1) + "H"] = 0;
        *regi[register_name.substr(0, 1) + "L"] |= (val & 0x00FF);
        *regi[register_name.substr(0, 1) + "H"] |= (val >> 8);
    }
}

void registers::print_register_map(std::ofstream &output)
{
    for (auto &i : regi)
    {
        if (i.first != "flag")
            output << i.first << " " << *(i.second) << "\n";
    }

    output << "\n";

    output << "carry"
           << " " << get_flag_data("carry") << "\n";
    output << "zero"
           << " " << get_flag_data("zero") << "\n";
    output << "overflow"
           << " " << get_flag_data("overflow") << "\n";
}

void registers::print_reg_data_hex_format(string &register_name)
{
    int32_t data = get_data(register_name);
    stringstream ss;
    ss << hex << data;
    string ans = ss.str();

    cout << ans << '\n';

    return;
}

void registers::update_flag(string name_of_flag, bool val)
{
    if (name_of_flag == "carry")
    {
        flag |= val;
    }
    else if (name_of_flag == "overflow")
    {
        flag |= (val * 1ll << 11);
    }
    else if (name_of_flag == "zero")
    {
        flag |= (val * 1ll << 6);
    }
}

bool registers::get_flag_data(string flag_name)
{
    if (flag_name == "carry")
    {
        return ((flag & 1) != 0);
    }
    else if (flag_name == "overflow")
    {
        return ((flag & (1 << 11)) != 0);
    }
    else if (flag_name == "zero")
    {
        return ((flag & (1 << 6)) != 0);
    }
}