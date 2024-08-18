#include "memory.h"

registers reg;

memory::memory()
{
    total_size = 1048575;
    memory_location = new (nothrow) int8_t[total_size];

    srand(unsigned(time(NULL)));
    uint8_t x = 1;

    for (int i = 0; i < total_size; i += 1)
    {
        memory_location[i] = (x % 256);
        if (x >= 255)
            x = 0;
        else
            x += 1;
    }
}

bool memory::is_memory(string &operand)
{
    for (auto &i : operand)
    {
        if (i == '[')
            return true;
    }
    return false;
}

uint16_t memory::convert_string_to_memory_location(string &mem_loc)
{
    uint16_t location = 0;

    string curr = "";
    vector<string> elements;
    for (auto &i : mem_loc)
    {
        if (i == ':')
        {
            curr = "";
            continue;
        }
        if (i == '[' || i == ']' || i == ' ' || i == '+' || i == '-')
        {
            if (curr.size())
            {
                elements.push_back(curr);
            }
            curr = "";
            if (i == '-')
            {
                curr.push_back('-');
            }
        }
        else
        {
            curr.push_back(i);
        }
    }
    if (curr.size())
    {
        elements.push_back(curr);
    }

    for (auto &i : elements)
    {
        if (reg.is_register(i))
        {
            location += reg.get_data(i);
        }
        else
        {
            if (check_if_string_hexadecimal(i))
                location += (convert_hex_string_to_decimal(i));
            else
                location += (stoi(i));
        }
    }
    return location;
}

void memory::upper_case(char &c)
{
    if (c >= 'a' && c <= 'z')
        c = char(c - 32);
}

uint16_t memory::convert_hex_string_to_decimal(string &hex_string)
{
    for (auto &i : hex_string)
        upper_case(i);

    if (hex_string.size() > 1 && hex_string.substr(0, 2) == "0X")
        hex_string = hex_string.substr(2);
    else
        hex_string = hex_string.substr(0, hex_string.size() - 1);

    uint16_t decimal_num = 0;

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

bool memory::check_if_string_hexadecimal(string &str)
{
    if (str.size() > 1 && ((str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X") || (str[str.size() - 1] == 'h' || str[str.size() - 1] == 'H')))
        return true;
    else
        return false;
}

uint32_t memory::string_to_effective_address(string &memory_address)
{
    uint16_t offset = convert_string_to_memory_location(memory_address);
    int n = memory_address.size();
    string base_address = "DS";
    for (int i = 0; i < n - 1; i += 1)
    {
        string str = memory_address.substr(i, 2);
        if (reg.is_segment_register(str))
        {
            base_address = str;
        }
    }

    uint32_t final_address = calculate_effective_address(base_address, offset);
    return final_address;
}

int8_t memory::get_data(string &memory_address)
{
    uint32_t final_address = string_to_effective_address(memory_address);
    if (final_address < total_size)
    {
        return memory_location[final_address];
    }
    else
    {
        cout << "the address of the memory location is out of range";
        cout << "the address should be within"
             << " " << total_size << '\n';
    }
}

uint32_t memory::calculate_effective_address(string &segment_register, uint16_t offset)
{
    uint32_t final_address = (reg.get_data(segment_register) * 0x10) + offset;
    return final_address;
}

void memory::set_memory_location(string &memory_address, uint8_t value)
{
    uint32_t final_address = string_to_effective_address(memory_address);

    if (final_address < total_size)
    {
        memory_location[final_address] = value;
    }
    else
    {
        cout << "the address of the memory location is out of range";
        cout << "the address should be within"
             << " " << total_size << '\n';
    }
}