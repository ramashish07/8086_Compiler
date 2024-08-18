#include "operation.h"

operation::operation()
{
    number_of_operands["MOV"] = 2;
    number_of_operands["ADD"] = 2;
    number_of_operands["ADC"] = 2;
    number_of_operands["SUB"] = 2;
    number_of_operands["MUL"] = 1;
    number_of_operands["DIV"] = 2;
    number_of_operands["SBB"] = 2;
}

int operation::get_number_of_operands(string &operation)
{
    if (number_of_operands.count(operation))
        return number_of_operands[operation];
    else
    {
        cout << "the operation_type does not exist \n";
        return -1;
    }
}