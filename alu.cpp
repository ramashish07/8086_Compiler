#include "alu.h"

memory mem;
immediate im;

int32_t alu::decode_operand(string &operand)
{
    if (reg.is_register(operand))
    {
        return reg.get_data(operand);
    }

    else if (mem.is_memory(operand))
    {
        return mem.get_data(operand);
    }

    else if (im.is_immediate(operand))
    {
        return int32_t(im.get_data(operand));
    }
}

void alu::perform_addition(string &operand1, string &operand2)
{
    int32_t val2 = decode_operand(operand2);
    int32_t val1 = decode_operand(operand1);

    int32_t ans = val1 + val2;

    if (reg.is_register(operand1))
    {
        reg.set_value(operand1, ans);
    }
    else if (mem.is_memory(operand1))
    {
        mem.set_memory_location(operand1, ans);
    }
    else
    {
        cout << "invalid operand in alu instruction";
    }

    update_flag("addtion", operand1, operand2, val1, val2, ans);

    return;
}

void alu::perform_subtraction(string &operand1, string &operand2)
{
    int32_t val2 = decode_operand(operand2);
    int32_t val1 = decode_operand(operand1);

    int32_t ans = val1 - val2;

    if (reg.is_register(operand1))
    {
        reg.set_value(operand1, ans);
    }
    else if (mem.is_memory(operand1))
    {
        mem.set_memory_location(operand1, ans);
    }
    else
    {
        cout << "invalid operand in alu instruction";
    }

    update_flag("subtraction", operand1, operand2, val1, val2, ans);
    return;
}

void alu::perform_subtraction_with_carry(string &operand1, string &operand2)
{
    int32_t val2 = decode_operand(operand2);
    int32_t val1 = decode_operand(operand1);

    int32_t ans = val1 - val2 - reg.get_flag_data("carry");

    if (reg.is_register(operand1))
    {
        reg.set_value(operand1, ans);
    }
    else if (mem.is_memory(operand1))
    {
        mem.set_memory_location(operand1, ans);
    }
    else
    {
        cout << "invalid operand in alu instruction";
    }

    update_flag("subtraction", operand1, operand2, val1, val2, ans);
    return;
}

void alu::perform_multiplication(string &operand1)
{
    long long ans = decode_operand(operand1);
    if (reg.is_register(operand1))
    {
        if (operand1[operand1.size() - 1] == 'X')
        {
            string operand2 = "AX";
            ans = reg.get_data(operand2) * reg.get_data(operand1);
            reg.set_value(operand1, ans & 0x0000FFFF);

            string upper_word = "DX";
            reg.set_value(upper_word, ans & 0xFFFF0000);
        }
        else
        {
            string operand2 = "AL";
            ans = reg.get_data(operand2) * reg.get_data(operand1);

            string destination = "AX";
            reg.set_value(destination, ans);
        }
    }
    else
    {
        cout << "invalid operand for multiplication";
    }
}

void alu::perform_addition_with_carry(string &operand1, string &operand2)
{
    int32_t val2 = decode_operand(operand2);
    int32_t val1 = decode_operand(operand1);

    int32_t ans = val1 + val2 + reg.get_flag_data("carry");

    if (reg.is_register(operand1))
    {
        reg.set_value(operand1, ans);
    }
    else if (mem.is_memory(operand1))
    {
        mem.set_memory_location(operand1, ans);
    }
    else
    {
        cout << "invalid operand in alu instruction";
    }

    update_flag("addtion", operand1, operand2, val1, val2, ans);

    return;
}

void alu::update_flag(string type, string &op1, string &op2, int32_t &val1, int32_t &val2, int32_t &ans)
{
    if (ans == 0)
    {
        reg.update_flag("zero", 1);
    }
    else
    {
        reg.update_flag("zero", 0);
    }

    int32_t sizeofans = 0;
    if (reg.is_register(op1))
    {
        if (op1[op1.size() - 1] == 'X')
            sizeofans = 16;
        else
            sizeofans = 8;
    }
    else
        sizeofans = 8;

    if (sizeofans == 16 && ((ans >= (1 << (sizeofans - 1))) || (ans < (-(1 << (sizeofans - 1))))))
    {
        reg.update_flag("overflow", 1);
    }
    else
    {
        reg.update_flag("overflow", 0);
    }

    if (sizeofans == 8 && (ans >= (1 << (sizeofans))))
    {
        reg.update_flag("carry", 1);
    }
    else
    {
        reg.update_flag("carry", 0);
    }
}

void alu::perform_division(string &operand)
{
    int32_t denominator = decode_operand(operand);

    string num_reg = "AX";
    string den_reg = operand;

    int32_t numerator = reg.get_data(num_reg);

    int32_t q = numerator / denominator;
    int32_t r = numerator % denominator;

    string dest_quotient = "AL";
    string dest_reminder = "AH";

    reg.set_value(dest_quotient, q);
    reg.set_value(dest_reminder, r);
}