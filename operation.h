#ifndef OPERAND_H
#define OPERAND_H

#include <bits/stdc++.h>
using namespace std;

class operation
{
private:
    map<string, int> number_of_operands;

public:
    operation();
    int get_number_of_operands(string &);
};

#endif