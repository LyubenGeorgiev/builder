#pragma once
#include "Operation.h"
class Multiplication : public Operation
{
public:
    Multiplication(int _precedence, char _association) : Operation(_precedence, _association)
    {
    }

    double apply(const double &a, const double &b) const override
    {
        return a * b;
    }
};