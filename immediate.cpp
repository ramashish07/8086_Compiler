#include "immediate.h"

bool immediate::is_immediate(string &operand)
{
    if (reg.is_register(operand) == false && is_memory(operand) == false)
        return true;
    else
        return false;
}

bool immediate::is_memory(string &operand)
{
    for (auto &i : operand)
    {
        if (i == '[')
            return true;
    }
    return false;
}

int8_t immediate::get_data(string &operand)
{
    int8_t ans = 0;
    if (check_if_string_hexadecimal(operand))
    {
        ans += convert_hex_string_to_decimal(operand);
    }
    else
    {
        ans += int8_t(stoi(operand));
    }

    return ans;
}

bool immediate::check_if_string_hexadecimal(string &str)
{
    if (str.size() > 1 && (str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X"))
        return true;
    else
        return false;
}

int8_t immediate::convert_hex_string_to_decimal(string &hex_string)
{
    hex_string = hex_string.substr(2);

    for (auto &i : hex_string)
    {
        upper_case(i);
    }

    int8_t decimal_num = 0;

    uint16_t mul = 1;
    int i = hex_string.size() - 1;
    while (i >= 0)
    {
        if (hex_string[i] >= 'A')
            decimal_num += uint16_t(int(hex_string[i] - 'A' + 10) * mul);
        else
            decimal_num += uint16_t(int(hex_string[i] - '0') * mul);
        mul *= 16;

        i -= 1;
    }

    return decimal_num;
}

void immediate::upper_case(char &c)
{
    if (c >= 'a' && c <= 'z')
        c -= 32;
}