#include <bits/stdc++.h>

#include "registers.h"
#include "global_objects.h"

#include "memory.h"
#include "immediate.h"
#include "operation.h"
#include "alu.h"
#include "memory_global_object.h"

using namespace std;

// to correct the initialization of AL,AH,BL,BH and so on according to AX,BX and so on

// global object of memory
operation op;
alu al;
// immediate im;
// memory mem;

int random_val = rand();
// function prototypes
vector<string> preprocess_instruction(string &);
void process_instruction(vector<string> &);
void process_mov_instruction(vector<string> &);
void process_add_instruction(vector<string> &);
void process_sub_instruction(vector<string> &);
void process_adc_instruction(vector<string> &);
void process_sbb_instruction(vector<string> &);
void process_mul_instruction(vector<string> &);
void process_div_instruction(vector<string> &);
bool check_jump_instruction(string &);
bool process_jump_instruction(string &);
void process_cmp_instruction(vector<string> &);

void split_instruction_into_operation_and_operand(string &, vector<string> &);
void upper_case(char &);

// compare

void process_cmp_instruction(vector<string> &op)
{
    string op1 = op[1];
    string op2 = op[2];

    if (reg.is_register(op1) && reg.is_register(op2))
    {
        int x = reg.get_data(op1);
        int y = reg.get_data(op2);
        if (x - y < 0)
        {
            reg.update_flag("carry", 1);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else if (x - y > 0)
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 1);
            reg.update_flag("overflow", 0);
        }
    }
    else if (mem.is_memory(op1) && reg.is_register(op2))
    {
        int x = mem.get_data(op1);
        int y = reg.get_data(op2);
        if (x - y < 0)
        {
            reg.update_flag("carry", 1);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else if (x - y > 0)
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 1);
            reg.update_flag("overflow", 0);
        }
    }
    else if (mem.is_memory(op1) && mem.is_memory(op2))
    {
        int x = mem.get_data(op1);
        int y = mem.get_data(op2);
        if (x - y < 0)
        {
            reg.update_flag("carry", 1);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else if (x - y > 0)
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 1);
            reg.update_flag("overflow", 0);
        }
    }
    else
    {
        int x = reg.get_data(op1);
        int y = mem.get_data(op2);
        if (x - y < 0)
        {
            reg.update_flag("carry", 1);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else if (x - y > 0)
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 0);
            reg.update_flag("overflow", 0);
        }
        else
        {
            reg.update_flag("carry", 0);
            reg.update_flag("zero", 1);
            reg.update_flag("overflow", 0);
        }
    }
}

// spliting  instruction into operation & operands
void split_instruction_into_operation_and_operand(string &line, vector<string> &ans)
{
    int n = line.size();
    string curr = "";
    for (int i = 0; i < n; i += 1)
    {
        if (line[i] == ';')
            break;
        else if (line[i] == ' ' || line[i] == ',')
        {
            if (curr.size())
                ans.push_back(curr);
            curr = "";
        }
        else
        {
            curr.push_back(line[i]);
        }
    }

    if (curr.size())
        ans.push_back(curr);

    return;
}

// processing mov instruction
void process_mov_instruction(vector<string> &v)
{
    string operand1 = v[1];
    string operand2 = v[2];

    // memory <- memory (illegal)
    if (mem.is_memory(operand1) && mem.is_memory(operand2))
    {
        cout << "both opernads can not be memory location \n";
        return;
    }

    // imediate < -memory / register(illegal)
    else if (im.is_immediate(operand1))
    {
        cout << "the destination can not be imediate value";
    }
    else
    {
        // reg <- reg
        if (reg.is_register(operand1) && reg.is_register(operand2))
        {
            if (operand2[1] != operand1[1] && (operand1[1] == 'X' || operand2[1] == 'X'))
            {
                cout << "the registers in mov instruction should be of the same size --> error \n";
                return;
            }
            reg.set_value(operand1, reg.get_data(operand2));
        }
        // memory_location / register <- imediate
        else if (im.is_immediate(operand2))
        {
            if (reg.is_register(operand1))
                reg.set_value(operand1, im.get_data(operand2));
            else
                mem.set_memory_location(operand1, im.get_data(operand2));
        }
        else
        {
            // memory <- register
            if (mem.is_memory(operand1) && reg.is_register(operand2))
            {
                mem.set_memory_location(operand1, reg.get_data(operand2));
            }
            // register <- memory
            else if (reg.is_register(operand1) && mem.is_memory(operand2))
            {
                reg.set_value(operand1, mem.get_data(operand2));
            }

            // invalid
            else
            {
                cout << "illegal_instruction\n";
                return;
            }
        }
    }
}

// processing add instruction
void process_add_instruction(vector<string> &v)
{
    string operand1 = v[1];
    string operand2 = v[2];

    al.perform_addition(v[1], v[2]);
}

// processing sub instruction
void process_sub_instruction(vector<string> &v)
{
    string operand1 = v[1];
    string operand2 = v[2];

    al.perform_subtraction(v[1], v[2]);
}

// processing sbb instruction
void process_sbb_instruction(vector<string> &v)
{
    string operand1 = v[1];
    string operand2 = v[2];

    al.perform_subtraction_with_carry(v[1], v[2]);
}

// processing adc instruction
void process_adc_instruction(vector<string> &v)
{
    string operand1 = v[1];
    string operand2 = v[2];

    al.perform_addition_with_carry(v[1], v[2]);
}

// processing mul instruction
void process_mul_instruction(vector<string> &v)
{
    string operand1 = v[1];

    al.perform_multiplication(v[1]);
}

// processing mul instruction
void process_div_instruction(vector<string> &v)
{
    string operand1 = v[1];

    al.perform_division(v[1]);
}

// checking for jump instruction
bool check_jump_instruction(string &op)
{
    return (op == "JE" || op == "JNE" || op == "JZ" || op == "JNZ" || op == "JC" || op == "JNC");
}

// checking for validity of jump instruction
bool process_jump_instruction(string &op)
{
    if (op == "JZ" || op == "JE")
        return reg.get_flag_data("zero");
    else if (op == "JC")
        return reg.get_flag_data("carry");
    else if (op == "JNZ" || op == "JNE")
        return (reg.get_flag_data("zero") == 0);
    else if (op == "JNC")
        return (reg.get_flag_data("carry") == 0);
    else
        return false;
}

// preocessing instruction
void process_instruction(vector<string> &v)
{
    string opration = v[0];

    if (opration == "MOV")
    {
        process_mov_instruction(v);
    }
    else if (opration == "ADD")
    {
        process_add_instruction(v);
    }
    else if (opration == "SUB")
    {
        process_sub_instruction(v);
    }
    else if (opration == "MUL")
    {
        process_mul_instruction(v);
    }
    else if (opration == "ADC")
    {
        process_adc_instruction(v);
    }
    else if (opration == "SBB")
    {
        process_sbb_instruction(v);
    }
    else if (opration == "DIV")
    {
        process_div_instruction(v);
    }
    else if (opration == "CMP")
    {
        process_cmp_instruction(v);
    }
}

// preprocessing instruction
vector<string> preprocess_instruction(string &instruction)
{
    for (auto &i : instruction)
    {
        upper_case(i);
    }

    vector<string> v;
    split_instruction_into_operation_and_operand(instruction, v);

    int n = v.size();

    if (n == 0)
        return v;

    string opration = v[0];

    // int no_of_operands = op.get_number_of_operands(opration);

    // for (auto &i : v)
    // {
    //     cout << i << " ";
    // }
    // cout << '\n';

    return v;

    // if (no_of_operands != (n - 1) || no_of_operands == -1)
    // {
    //     cout << "illegal instruction\n";
    //     return;
    // }
}

void upper_case(char &c)
{
    if (c >= 'a' && c <= 'z')
        c -= 32;
}

int main(void)
{
    fstream inputfile;
    inputfile.open("input.txt", ios::in);

    map<int, vector<string>> instructions;
    map<string, int> symbol_table;

    int i = 1;
    if (inputfile.is_open())
    {
        string str;

        while (getline(inputfile, str))
        {
            cout << str << " ";
            vector<string> v;
            v = preprocess_instruction(str);

            if (v.size() && v[0].size() && v[0][v[0].size() - 1] == ':')
            {
                symbol_table[v[0].substr(0, v[0].size() - 1)] = i;
                v.erase(v.begin());
                instructions[i] = v;
            }
            else if (v.size())
            {
                instructions[i] = v;
            }
            i += 1;
        }
    }
    ofstream output;
    output.open("output.txt");

    for (int i = 1; i <= instructions.size(); i += 1)
    {
        if (instructions[i].size() == 0)
            continue;
        if (check_jump_instruction(instructions[i][0]))
        {
            if (process_jump_instruction(instructions[i][0]))
            {
                i = symbol_table[instructions[i][1]] - 1;
            }
        }
        else
        {
            process_instruction(instructions[i]);
        }
    }

    reg.print_register_map(output);

    output.close();

    ofstream symbol;
    symbol.open("symbol_table.txt");

    for (auto &i : symbol_table)
        symbol << i.first << " " << i.second << "\n";
}