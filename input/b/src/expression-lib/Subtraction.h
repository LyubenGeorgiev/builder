#pragma once
#include "Operation.h"

class Subtraction : public Operation
{
public:
    Subtraction(int _precedence, char _association) : Operation(_precedence, _association)
    {
    }

    double apply(const double &a, const double &b) const override
    {
        return a - b;
    }
};